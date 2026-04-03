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

### Architecture

```
UI  →  Service  →  Repository  →  Database
         ↑              ↑
       Entity          Entity
```

Each layer only depends on the layer below it.
