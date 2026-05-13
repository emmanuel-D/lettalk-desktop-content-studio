# LetTalk Desktop Content Studio

A desktop content management tool for LetTalk content creators.
Built with **C++17**, **Qt 6 Widgets** and **SQLite**.

## Prerequisites

- **Qt 6.5+** (installed via Qt Online Installer)

## Setup (one-time)

Add Qt tools to your PATH by adding this line to your `~/.zshrc`:

```bash
export PATH="$HOME/Qt/Tools/CMake/CMake.app/Contents/bin:$HOME/Qt/6.9.1/macos/bin:$PATH"
```

For Ubuntu Linux users, please add this line instead to your `~/.bashrc`:

```bash
export PATH="$HOME/Qt/Tools/CMake/bin:$HOME/Qt/6.11.0/gcc_64/bin:$PATH"
```

Then reload: `source ~/.zshrc` oder `source ~/.bashrc` for linux user.

## Build & Run

```bash
./run.sh
```

This script configures, builds and starts the app in one step.

## Project Structure

```
├── main.cpp                         Entry point
├── run.sh                           Build & run script
├── BOARD.md                         Project board (tasks tracking)
├── docs/                            Developer documentation (UML + flows)
│   ├── README.md                    Start here ← read this first
│   ├── 01-architecture-overview.md  Layered architecture
│   ├── 02-data-model.md             Entities, ER diagram, sample data
│   ├── 03-class-diagram.md          Full UML class diagram
│   ├── 04-flows.md                  Sequence diagrams per operation
│   └── 05-end-to-end-example.md     One post: UI form → C++ entity → SQL rows
└── src/
    ├── database/                    DB connection & schema setup
    │   └── databasemanager.h/cpp
    ├── entities/                    Data models (like @Entity)
    │   └── post.h
    ├── repositories/                DB access layer (like @Repository)
    │   └── postrepository.h/cpp
    ├── services/                    Business logic (like @Service)
    │   └── postservice.h/cpp
    └── ui/                          Qt Widgets (like @Controller)
        └── mainwindow.h/cpp/ui
```

📖 **New to the project?** Start with [`docs/README.md`](docs/README.md) — it walks through
the architecture, data model and runtime flows with UML and Mermaid diagrams.

### Architecture

```
UI  →  Service  →  Repository  →  Database
         ↑              ↑
       Entity          Entity
```

Each layer only depends on the layer below it.
