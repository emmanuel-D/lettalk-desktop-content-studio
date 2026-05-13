# 5. End-to-End Example — From UI Form to SQL Row

This document follows **one single post** through the entire application. You will see exactly
what data lives in the UI, what the C++ entity looks like at each step, and what the SQL
tables contain after each operation.

If you ever feel disconnected from "what does the data actually look like?", come back here.

---

## 5.1 Setting the scene

> 👤 Daniel opens LetTalk Content Studio at **10:42 UTC on 2026-05-13**.
> The database is empty (just-installed app).

State of the database right now:

```sql
SELECT * FROM posts;      -- 0 rows
SELECT * FROM tags;       -- 0 rows
SELECT * FROM post_tags;  -- 0 rows
```

---

## 5.2 Step 1 — Daniel fills in the form

He clicks "New Post" in the sidebar and the editor opens. He types:

```
┌──────────────────────────────────────────────────────────────────┐
│  LetTalk Content Studio                                     ─ □ ×│
├──────────────────────────────────────────────────────────────────┤
│  [Posts]                                                          │
│  ───────                                                          │
│  (empty list)            ┌─ Editor ─────────────────────────────┐ │
│                          │ Title:    [ My first Linux post    ] │ │
│                          │ Category: [ Tech                   ] │ │
│                          │                                      │ │
│                          │ Content:                             │ │
│                          │ ┌──────────────────────────────────┐ │ │
│                          │ │ After ten years on macOS, I gave │ │ │
│                          │ │ Linux another try. Here is what  │ │ │
│                          │ │ surprised me…                    │ │ │
│                          │ └──────────────────────────────────┘ │ │
│                          │                                      │ │
│                          │           [ Cancel ]   [   Save   ]  │ │
│                          └──────────────────────────────────────┘ │
└──────────────────────────────────────────────────────────────────┘
```

He clicks **Save**.

---

## 5.3 Step 2 — The UI builds a `Post` object

Inside `MainWindow::on_savePost_clicked()` (Sprint 2 code, not yet written), the values
from the widgets are read into a fresh `Post` struct:

```cpp
Post post;
post.title    = ui->titleLineEdit->text();          // "My first Linux post"
post.category = ui->categoryLineEdit->text();        // "Tech"
post.content  = ui->contentTextEdit->toPlainText();  // "After ten years on macOS..."
// post.id        = -1   (default — not yet known)
// post.createdAt =      (default-constructed, invalid QDateTime)
// post.updatedAt =      (default-constructed, invalid QDateTime)

m_postService.createPost(post);
```

Visualised, the `Post` object in memory looks like this **before the service touches it**:

```text
Post {
    id        = -1
    title     = "My first Linux post"
    category  = "Tech"
    content   = "After ten years on macOS, I gave Linux another try…"
    createdAt = <invalid>
    updatedAt = <invalid>
}
```

---

## 5.4 Step 3 — `PostService::createPost()` validates and stamps timestamps

```cpp
bool PostService::createPost(Post &post)
{
    if (post.title.isEmpty()) return false;          // ✅ passes ("My first Linux post")
    const auto now = QDateTime::currentDateTimeUtc();// 2026-05-13 10:42:17 UTC
    post.createdAt = now;
    post.updatedAt = now;
    return m_repository.save(post);
}
```

The same `Post` object, **after the service runs** (just before it reaches the repository):

```text
Post {
    id        = -1
    title     = "My first Linux post"
    category  = "Tech"
    content   = "After ten years on macOS, I gave Linux another try…"
    createdAt = 2026-05-13 10:42:17 UTC
    updatedAt = 2026-05-13 10:42:17 UTC
}
```

> 💡 The id is still `-1` — only the database can decide what the real id will be, because
> it owns the `AUTOINCREMENT` counter.

---

## 5.5 Step 4 — `PostRepository::save()` builds the SQL

```cpp
bool PostRepository::save(Post &post)
{
    QSqlQuery query;
    query.prepare(
        "INSERT INTO posts(title, content, category, created_at, updated_at) "
        "VALUES (:title, :content, :category, :created_at, :updated_at)"
    );

    auto values = toMap(post, /*isUpdate=*/false);
    for (auto it = values.begin(); it != values.end(); ++it)
        query.bindValue(it.key(), it.value());

    if (!query.exec()) { /* log + return false */ }

    post.id = query.lastInsertId().toInt();          // ← gets back the new id
    return true;
}
```

After `bindValue()` runs, the prepared statement is effectively:

```sql
INSERT INTO posts(title, content, category, created_at, updated_at)
VALUES (
    'My first Linux post',
    'After ten years on macOS, I gave Linux another try…',
    'Tech',
    '2026-05-13T10:42:17.000',
    '2026-05-13T10:42:17.000'
);
```

> 💡 SQLite never actually sees the question marks or `:name` placeholders — the Qt SQL
> driver substitutes the bound values safely (escaping quotes, blocking injection). The form
> above is what conceptually executes.

---

## 5.6 Step 5 — State of the database right after the INSERT

**`posts` table:**

| id | title                | content                            | category | created_at              | updated_at              |
|----|----------------------|------------------------------------|----------|-------------------------|-------------------------|
| 1  | My first Linux post  | After ten years on macOS, I gave…  | Tech     | 2026-05-13T10:42:17.000 | 2026-05-13T10:42:17.000 |

**`tags` and `post_tags`:** still empty.

---

## 5.7 Step 6 — The id is written back into the C++ object

Back inside `save()`:

```cpp
post.id = query.lastInsertId().toInt();   // SQLite returned 1
```

The `Post` object now looks like this — this is what the UI ultimately sees:

```text
Post {
    id        = 1                              ← assigned by the database
    title     = "My first Linux post"
    category  = "Tech"
    content   = "After ten years on macOS, I gave Linux another try…"
    createdAt = 2026-05-13 10:42:17 UTC
    updatedAt = 2026-05-13 10:42:17 UTC
}
```

The UI uses that `id` to refer to the post for any later operation (update, delete, tagging).

---

## 5.8 Step 7 — Daniel adds two tags: `linux` and `macos`

(Sprint 3 feature — pseudocode for now.)

In the UI he types `linux` into the tag input and presses Enter, then `macos` and Enter.
Each Enter triggers:

```cpp
m_postService.addTagToPost(post.id, "linux");
m_postService.addTagToPost(post.id, "macos");
```

Inside the service, each call does **two repository operations**:

1. Find or create the tag → returns the tag's `id`.
2. Insert the link row into `post_tags`.

After both calls finish, the database looks like this:

**`posts` table** (unchanged):

| id | title               | category | created_at              | updated_at              |
|----|---------------------|----------|-------------------------|-------------------------|
| 1  | My first Linux post | Tech     | 2026-05-13T10:42:17.000 | 2026-05-13T10:42:17.000 |

**`tags` table** (two new rows):

| id | name  |
|----|-------|
| 1  | linux |
| 2  | macos |

**`post_tags` table** (two new junction rows):

| post_id | tag_id |
|---------|--------|
| 1       | 1      |
| 1       | 2      |

In C++, the post object viewed *with* its tags loaded would look like (after Sprint 3 adds the field):

```text
Post {
    id        = 1
    title     = "My first Linux post"
    category  = "Tech"
    content   = "After ten years on macOS..."
    createdAt = 2026-05-13 10:42:17 UTC
    updatedAt = 2026-05-13 10:42:17 UTC
    tags      = [
        Tag{ id=1, name="linux" },
        Tag{ id=2, name="macos" }
    ]
}
```

---

## 5.9 Step 8 — The whole picture, side by side

```mermaid
flowchart LR
    subgraph UI["🟦 What the user sees"]
        FORM["📝 Form<br/>title: 'My first Linux post'<br/>category: 'Tech'<br/>content: 'After ten years...'<br/>tags: linux, macos"]
    end

    subgraph MEM["🟪 What lives in C++ memory"]
        OBJ["Post {<br/>  id=1<br/>  title='My first Linux post'<br/>  category='Tech'<br/>  content='After ten years...'<br/>  createdAt=2026-05-13T10:42:17Z<br/>  updatedAt=2026-05-13T10:42:17Z<br/>  tags=[Tag{1,'linux'}, Tag{2,'macos'}]<br/>}"]
    end

    subgraph DB["🟥 What lives in SQLite"]
        T1["posts<br/>──────────<br/>1 | My first Linux post | Tech | ..."]
        T2["tags<br/>──────────<br/>1 | linux<br/>2 | macos"]
        T3["post_tags<br/>──────────<br/>1 | 1<br/>1 | 2"]
    end

    FORM ==>|MainWindow + PostService<br/>(in-memory build)| OBJ
    OBJ  ==>|PostRepository.save()<br/>+ TagRepository<br/>(SQL INSERTs)| T1
    OBJ  ==> T2
    OBJ  ==> T3
```

The same logical post — "My first Linux post tagged linux & macos" — appears in **three
different shapes**: a form on screen, a `Post` struct in memory, and three rows split
across three tables on disk.

---

## 5.10 Reading it back — round trip

When Daniel later reopens the app, `MainWindow` calls `PostService::getAllPosts()`, which
calls `PostRepository::findAll()`:

```sql
SELECT * FROM posts ORDER BY updated_at DESC;
```

SQLite returns one row:

```
1 | My first Linux post | After ten years... | Tech | 2026-05-13T10:42:17.000 | 2026-05-13T10:42:17.000
```

`mapToPost(query)` walks the columns and rebuilds the C++ object:

```cpp
Post post;
post.id        = query.value("id").toInt();           // 1
post.title     = query.value("title").toString();     // "My first Linux post"
post.content   = query.value("content").toString();   // "After ten years..."
post.category  = query.value("category").toString();  // "Tech"
post.createdAt = query.value("created_at").toDateTime();
post.updatedAt = query.value("updated_at").toDateTime();
return post;
```

Once Sprint 3 adds tag loading, a second query
`SELECT t.* FROM tags t JOIN post_tags pt ON pt.tag_id = t.id WHERE pt.post_id = 1`
will populate `post.tags`. The UI then re-renders the editor pre-filled with everything —
identical to what Daniel typed yesterday.

The **round trip is complete**: UI ➝ entity ➝ SQL ➝ entity ➝ UI.

---

## 5.11 The mental anchor

If you remember nothing else from these docs, remember this picture:

| Shape          | Lives in         | Looks like                                  |
|----------------|------------------|---------------------------------------------|
| **Form fields**| `mainwindow.ui` (Qt widgets) | A title, category, content, tag chips on screen |
| **C++ object** | RAM, `Post` struct | `Post { id, title, …, tags }`              |
| **SQL rows**   | `lettalk.db` file | `posts(1, …)`, `tags(1, "linux")`, `post_tags(1, 1)` |

The **Repository** is the bridge between rows 2 and 3.
The **UI + Service** are the bridge between rows 1 and 2.
That's the entire architecture, expressed as one table.

---

⬅️ Back to the [index](README.md).
