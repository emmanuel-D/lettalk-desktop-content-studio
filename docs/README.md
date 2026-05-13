# LetTalk Content Studio — Developer Documentation

Welcome, Daniel! 👋

This folder contains the **deep-dive documentation** for the LetTalk Desktop Content Studio.
It is meant to give you a complete mental model of the project before you keep building features.

The codebase is small on purpose. The architecture, however, is the same one used in
**production systems at BMW, Bosch, SAP** etc. Understanding it here means you understand it
everywhere.

---

## How to read these docs

Read them **in order** the first time. Each file builds on the previous one.

| # | File | What you'll learn |
|---|------|-------------------|
| 1 | [`01-architecture-overview.md`](01-architecture-overview.md) | What the layers are, why they exist, and the one rule that holds them together. |
| 2 | [`02-data-model.md`](02-data-model.md) | Entities, tables, the many-to-many relation between posts and tags — with concrete sample data. |
| 3 | [`03-class-diagram.md`](03-class-diagram.md) | The full UML class diagram. Who owns what, who calls what, multiplicities. |
| 4 | [`04-flows.md`](04-flows.md) | Sequence diagrams showing how a user click travels from the UI down to SQLite and back. |
| 5 | [`05-end-to-end-example.md`](05-end-to-end-example.md) | One post followed all the way: UI form → C++ entity → SQL rows → back to the UI. |

---

## Conventions used in these docs

- **Mermaid diagrams** are used throughout. GitHub renders them automatically. In your IDE,
  install the *Markdown Preview Mermaid Support* extension (VS Code) or use the built-in
  Markdown viewer in Qt Creator.
- Code snippets are **C++17** unless stated otherwise.
- **Real-world examples** appear inside `> 💡 Example:` blockquotes.
- `[NOT IMPLEMENTED YET]` marks classes or fields that are designed but still on the
  [`BOARD.md`](../BOARD.md) backlog.

---

## What this project is, in one paragraph

LetTalk Content Studio is a **desktop tool for content creators** of the LetTalk social
network. A creator launches it, sees their list of post drafts in a sidebar, opens one to
edit it in Markdown, tags it ("tech", "linux", "qt"), saves it, and later "publishes" it.
Everything lives locally in a **SQLite** file (`lettalk.db`). The UI is built with **Qt 6
Widgets**, and the business logic is split across the **layers** described in these docs.

---

## If you only remember three things

1. **The UI never talks to the database directly.** It calls a Service. The Service calls a Repository. The Repository talks to SQLite.
2. **An Entity is just data.** No SQL, no Qt widgets, no logic — only fields. That's why the same `Post` struct can live in the UI layer and the repository layer without creating a tangled dependency mess.
3. **Layers protect you from change.** If LetTalk ever swaps SQLite for PostgreSQL, only the Repository changes. The UI and Service don't even know.

Now jump into [`01-architecture-overview.md`](01-architecture-overview.md). 🚀
