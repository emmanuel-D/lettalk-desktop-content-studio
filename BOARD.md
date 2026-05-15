# Project Board — LetTalk Desktop Content Studio

## Done

- [x] Project setup (CMake, Qt 6, C++17)
- [x] Layered architecture (entities, repositories, services, ui, database)
- [x] DatabaseManager with SQLite connection
- [x] Posts table created (id, created_at)
- [x] Post entity with id and createdAt fields
- [x] MainWindow showing DB connection status
- [x] Build & run script (`run.sh`)

## Sprint 1 — Database & Environment (In Progress)

- [X] Add remaining columns to posts table (title, content, category, updated_at)
- [X] Add matching fields to Post entity (`post.h`)
- [X] Implement PostRepository methods (save, update, remove, findById, findAll)
- [X] Wire PostService to call PostRepository
- [ ] Create tags table and Tag entity
- [ ] Create post_tags junction table
- [ ] Test CRUD operations via debug output

## Sprint 2 — CRUD & Base UI

- [ ] Design dashboard UI (sidebar + main view) in `mainwindow.ui`
- [ ] Display list of posts in sidebar
- [ ] Create new post from UI
- [ ] Edit existing post from UI
- [ ] Delete post from UI
- [ ] Auto-save drafts to database

## Sprint 3 — Markdown & Search

- [ ] Integrate Markdown editor (QTextEdit + formatting)
- [ ] Live preview of Markdown content
- [ ] Search posts by keyword
- [ ] Filter posts by tags or categories
- [ ] Assign tags to posts

## Sprint 4 — Finalization & Quality

- [ ] "Publish" button (simulated with logging)
- [ ] Error handling and user feedback
- [ ] Unit tests for Repository and Service layers
- [ ] Code cleanup and documentation
- [ ] Final code review (BMW standard)
