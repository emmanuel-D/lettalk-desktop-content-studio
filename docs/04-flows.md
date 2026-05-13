# 4. Runtime Flows — Following the Data

The class diagram in [`03-class-diagram.md`](03-class-diagram.md) shows the **static** picture.
This document shows the **dynamic** picture: how a user action travels through the layers
at runtime.

Every flow below uses the same color code:
- 🟦 UI · 🟩 Service · 🟨 Repository · 🟥 Database · 🟪 Entity

---

## 4.1 Flow A — Application startup

```mermaid
sequenceDiagram
    autonumber
    participant U as 👤 User
    participant Main as main.cpp
    participant DB as 🟥 DatabaseManager
    participant SQL as 🟥 SQLite (lettalk.db)
    participant Win as 🟦 MainWindow

    U->>Main: ./LetTalkContentStudio
    Main->>DB: new DatabaseManager()
    DB->>DB: QSqlDatabase::addDatabase("QSQLITE")
    Main->>DB: connect("lettalk.db")
    DB->>SQL: open file
    SQL-->>DB: ok
    DB->>SQL: PRAGMA foreign_keys = ON
    DB->>SQL: CREATE TABLE posts (...)
    SQL-->>DB: ok
    DB-->>Main: true
    Main->>Win: new MainWindow(&dbManager)
    Win->>Win: ui->setupUi(this)
    Win->>DB: isConnected()?
    DB-->>Win: true
    Win->>Win: statusBar()->showMessage("Database connected", 5000)
    Win-->>U: window appears
```

What you actually see on screen: a window pops up with the title bar
*"LetTalk Content Studio"* and a green status message at the bottom for 5 seconds.

---

## 4.2 Flow B — Creating a new post (the most important flow)

This is the flow Daniel should know **by heart**. Every other write operation looks similar.

```mermaid
sequenceDiagram
    autonumber
    participant U as 👤 User
    participant Win as 🟦 MainWindow
    participant Svc as 🟩 PostService
    participant Repo as 🟨 PostRepository
    participant SQL as 🟥 SQLite

    U->>Win: types title + content, clicks "Save"
    Win->>Win: build local Post{ title, content, category }
    Win->>Svc: createPost(post)

    Note over Svc: 🧠 Business rules go here
    Svc->>Svc: if (post.title.isEmpty()) return false
    Svc->>Svc: now = QDateTime::currentDateTimeUtc()
    Svc->>Svc: post.createdAt = now
    Svc->>Svc: post.updatedAt = now

    Svc->>Repo: save(post)
    Note over Repo: 💾 SQL goes here, nowhere else
    Repo->>Repo: query.prepare("INSERT INTO posts ...")
    Repo->>Repo: bindValue(":title", ...) etc.
    Repo->>SQL: query.exec()
    SQL-->>Repo: lastInsertId = 42
    Repo->>Repo: post.id = 42
    Repo-->>Svc: true

    Svc-->>Win: true
    Win-->>U: refresh sidebar, show "Saved ✓"
```

> 💡 **Why is `post` passed by reference (`Post &post`) instead of by value?**
> Because the repository writes the new database-generated `id` back into the object
> (`post.id = query.lastInsertId().toInt()`). If we passed by value, the caller would
> never see the new id — they'd hold a copy with `id = -1`. Passing by reference is a
> deliberate "out parameter" pattern here.

> 📘 **Want to see this with real values at every step?** Open
> [`05-end-to-end-example.md`](05-end-to-end-example.md) — it follows one specific post
> ("My first Linux post") from the form on screen all the way into the SQL tables, showing
> exactly what the `Post` struct contains at each layer.

---

## 4.3 Flow C — Listing all posts (read flow)

```mermaid
sequenceDiagram
    autonumber
    participant U as 👤 User
    participant Win as 🟦 MainWindow
    participant Svc as 🟩 PostService
    participant Repo as 🟨 PostRepository
    participant SQL as 🟥 SQLite

    U->>Win: opens the app / clicks "Refresh"
    Win->>Svc: getAllPosts()
    Svc->>Repo: findAll()
    Repo->>SQL: SELECT * FROM posts ORDER BY updated_at DESC
    SQL-->>Repo: rows[0], rows[1], rows[2], ...

    loop for each row
        Repo->>Repo: mapToPost(query) → Post
    end

    Repo-->>Svc: QList<Post>
    Svc-->>Win: QList<Post>

    loop for each Post
        Win->>Win: sidebar->addItem(post.title)
    end

    Win-->>U: sidebar populated
```

Notice the Service is a **pass-through** here — no business rule applies to a simple list.
That's fine. Not every Service method needs to be clever; the value of the Service is *being
the consistent entry point*, even when it has nothing extra to do today.

---

## 4.4 Flow D — Updating an existing post

```mermaid
sequenceDiagram
    autonumber
    participant U as 👤 User
    participant Win as 🟦 MainWindow
    participant Svc as 🟩 PostService
    participant Repo as 🟨 PostRepository
    participant SQL as 🟥 SQLite

    U->>Win: edits the post, clicks "Save"
    Win->>Svc: updatePost(post)
    Svc->>Svc: if (post.id <= 0) return false
    Svc->>Svc: updatedPost = post<br/>updatedPost.updatedAt = now
    Svc->>Repo: update(updatedPost)
    Repo->>SQL: UPDATE posts SET title=:title, ... WHERE id=:id
    SQL-->>Repo: rowsAffected = 1
    Repo-->>Svc: true
    Svc-->>Win: true
    Win-->>U: status "Updated ✓"
```

> 💡 **Why does the Service create a local copy `updatedPost`?**
> Because the parameter is declared `const Post &post` — promising the caller "I won't
> modify your object". To set `updatedAt`, we make a non-const copy. This is a common C++
> pattern: signal const-correctness at the interface, but mutate internally.

---

## 4.5 Flow E — Deleting a post

```mermaid
sequenceDiagram
    autonumber
    participant U as 👤 User
    participant Win as 🟦 MainWindow
    participant Svc as 🟩 PostService
    participant Repo as 🟨 PostRepository
    participant SQL as 🟥 SQLite

    U->>Win: right-click post → Delete
    Win->>Win: confirmation dialog "Are you sure?"
    U->>Win: confirms
    Win->>Svc: deletePost(id)
    Svc->>Svc: if (id <= 0) return false
    Svc->>Repo: remove(id)
    Repo->>SQL: DELETE FROM posts WHERE id = :id
    SQL-->>Repo: rowsAffected = 1
    Note over SQL: With ON DELETE CASCADE on post_tags<br/>(once added), the linked rows in<br/>post_tags would also be deleted here.
    Repo-->>Svc: true
    Svc-->>Win: true
    Win-->>U: post disappears from sidebar
```

---

## 4.6 Flow F — Tagging a post (planned, Sprint 3)

This flow does **not exist in code yet**. It is shown here to illustrate how *one user action*
can require **multiple Repository calls** orchestrated by the Service. This is exactly the
kind of complexity the Service layer was invented for.

```mermaid
sequenceDiagram
    autonumber
    participant U as 👤 User
    participant Win as 🟦 MainWindow
    participant Svc as 🟩 PostService
    participant TagRepo as 🟨 TagRepository<br/>[NOT IMPL YET]
    participant LinkRepo as 🟨 PostTagRepository<br/>[NOT IMPL YET]
    participant SQL as 🟥 SQLite

    U->>Win: types "qt" in tag input, presses Enter
    Win->>Svc: addTagToPost(postId=2, tagName="qt")

    Note over Svc: 1) ensure the tag exists
    Svc->>TagRepo: findByName("qt")
    TagRepo->>SQL: SELECT * FROM tags WHERE name = :name
    SQL-->>TagRepo: empty
    TagRepo-->>Svc: Tag{ id = -1 }
    Svc->>TagRepo: save(Tag{ name = "qt" })
    TagRepo->>SQL: INSERT INTO tags(name) VALUES (:name)
    SQL-->>TagRepo: lastInsertId = 7
    TagRepo-->>Svc: Tag{ id = 7, name = "qt" }

    Note over Svc: 2) link the tag to the post
    Svc->>LinkRepo: link(postId=2, tagId=7)
    LinkRepo->>SQL: INSERT INTO post_tags(post_id, tag_id) VALUES (:p, :t)
    SQL-->>LinkRepo: ok
    LinkRepo-->>Svc: true

    Svc-->>Win: true
    Win-->>U: tag chip "qt" appears under the post
```

Watch what the Service is doing: **two repositories, one transaction-like operation**, all
hidden from the UI. The UI only knows "addTagToPost". This is the *real* payoff of the
layered design.

---

## 4.7 Flow G — A failure path (what happens when things go wrong?)

```mermaid
sequenceDiagram
    autonumber
    participant U as 👤 User
    participant Win as 🟦 MainWindow
    participant Svc as 🟩 PostService
    participant Repo as 🟨 PostRepository
    participant SQL as 🟥 SQLite

    U->>Win: clicks "Save" with empty title
    Win->>Svc: createPost(post) — title = ""
    Svc->>Svc: post.title.isEmpty() → true
    Svc->>Svc: qWarning() << "Title is empty"
    Svc-->>Win: false

    Note over Win: ❌ No SQL was ever executed.<br/>The repository was never touched.

    Win-->>U: ❗ "Title is required" toast message
```

The Service guards the Repository from invalid data. The Repository never has to think
about "is this title empty?" — by the time a `Post` reaches it, it has already passed
validation.

---

## 4.8 Putting it all together — the mental model

When you read the code and feel lost, ask yourself the **three questions of layered architecture**:

1. **Where is this happening?** Which folder, which class? UI / Service / Repository / DB?
2. **What does this layer owe to the layer above?** (UI owes "render+events"; Service owes "validated business operations"; Repository owes "row ↔ entity"; DB owes "persistence".)
3. **Where does the data go next?** A method call to the layer below, or a return up to the layer above. Never sideways, never skipping.

If you can answer those three questions for any line of code, you understand the architecture.

---

➡️ Done with the docs! Head back to the [index](README.md) — or open [`BOARD.md`](../BOARD.md) and pick the next task.
