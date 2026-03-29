# Projektdossier: LetTalk Desktop Content Studio

**Programm:** LetTalk IT Excellence
**Kandidat:** Daniel Kameni
**Mentor:** Emmanuel Dadem (BMW AG / TH Ulm)

---

## 1. Kontext und Zielsetzung

Das Ziel dieses Projekts ist es, Daniel Kameni von seinem aktuellen Status als Absolvent zu einem einsatzbereiten Software-Ingenieur nach Industriestandard zu entwickeln. Daniel hat bereits Erfahrung mit C++ und QNX bei der Infoteam Software AG gesammelt.

Um seine Herausforderungen bei technischen Tests und Bewerbungsgesprächen zu meistern, wird er das "LetTalk Desktop Content Studio" entwickeln. Dies ist ein professionelles Tool für Content-Ersteller des sozialen Netzwerks LetTalk. Das Projekt demonstriert seine Fähigkeit, komplexe Softwarearchitekturen zu entwerfen und industrielle Qualitätsstandards (wie sie bei Unternehmen wie BMW gefordert werden) einzuhalten.

---

## 2. Tech-Stack

| Kategorie           | Technologie                          |
|---------------------|--------------------------------------|
| Sprache             | C++17                                |
| UI-Framework        | Qt 6 (Widgets)                       |
| Datenbank           | SQLite (via Qt SQL)                  |
| Build-System        | CMake                                |
| Versionsverwaltung  | Git + GitHub (Feature Branches, PRs) |
| Projektmanagement   | Agile/Scrum (siehe `BOARD.md`)       |
| Qualitätssicherung  | Clean Code, Unit-Tests               |

---

## 3. Architektur — Layered Architecture

Dieses Projekt folgt einer **geschichteten Architektur** (Layered Architecture), wie sie auch in Frameworks wie Spring Boot (Java) oder NestJS (TypeScript) verwendet wird. Jede Schicht hat eine klare Verantwortung und kennt nur die Schicht direkt unter ihr.

```
┌─────────────────────────────────────────────────┐
│                    UI Layer                      │
│            (src/ui/ — Qt Widgets)                │
│  Zeigt Daten an, nimmt Benutzereingaben entgegen │
└──────────────────────┬──────────────────────────┘
                       │ ruft auf
┌──────────────────────▼──────────────────────────┐
│                 Service Layer                    │
│          (src/services/ — Geschäftslogik)        │
│  Validierung, Regeln, Koordination               │
└──────────────────────┬──────────────────────────┘
                       │ ruft auf
┌──────────────────────▼──────────────────────────┐
│               Repository Layer                   │
│        (src/repositories/ — Datenzugriff)        │
│  SQL-Queries, CRUD-Operationen                   │
└──────────────────────┬──────────────────────────┘
                       │ liest/schreibt
┌──────────────────────▼──────────────────────────┐
│                   Database                       │
│         (src/database/ — Verbindung)             │
│  SQLite-Verbindung, Tabellenerstellung           │
└─────────────────────────────────────────────────┘

        ┌──────────────────────────┐
        │       Entity Layer       │
        │  (src/entities/ — Daten) │
        │  Reine Datenstrukturen   │
        └──────────────────────────┘
         ↑ wird von allen Schichten verwendet
```

### Was macht jede Schicht?

| Schicht        | Ordner              | Aufgabe                                                        | Analogie (Spring Boot)  |
|----------------|---------------------|----------------------------------------------------------------|-------------------------|
| **Entity**     | `src/entities/`     | Reine Datenstrukturen (`struct`). Keine Logik, kein SQL.       | `@Entity`               |
| **Repository** | `src/repositories/` | SQL-Zugriff. Jede Methode = eine Query. Nutzt `QSqlQuery`.    | `@Repository`           |
| **Service**    | `src/services/`     | Geschäftslogik. Ruft Repository auf. Validiert Daten.          | `@Service`              |
| **UI**         | `src/ui/`           | Qt Widgets. Zeigt Daten, reagiert auf Klicks. Ruft Service auf.| `@Controller`           |
| **Database**   | `src/database/`     | Verbindungsaufbau und Schema-Migration (CREATE TABLE).         | `DataSource` / Config   |

### Die goldene Regel

> **Jede Schicht darf nur die Schicht direkt unter ihr aufrufen.**
>
> - UI ruft Service auf (nie Repository direkt)
> - Service ruft Repository auf (nie QSqlQuery direkt)
> - Repository nutzt Entity als Datenstruktur
> - Entity kennt niemanden — es ist nur Daten

### Warum diese Trennung?

1. **Testbarkeit** — Jede Schicht kann einzeln getestet werden
2. **Wartbarkeit** — Änderungen in der DB betreffen nur Repository, nicht die UI
3. **Wiederverwendbarkeit** — Der Service funktioniert unabhängig von der UI
4. **Industriestandard** — Genau so arbeiten Teams bei BMW, Bosch, SAP etc.

---

## 4. Funktionale Anforderungen

| ID  | Anforderung        | Beschreibung                                                          |
|-----|--------------------|-----------------------------------------------------------------------|
| R1  | Notiz-Management   | CRUD (Erstellen, Lesen, Aktualisieren, Löschen) von Beitragsentwürfen |
| R2  | Persistenz         | Automatische Speicherung in SQLite-Datenbank                          |
| R3  | Organisation       | Tags und Kategorien zur thematischen Sortierung                       |
| R4  | Benutzeroberfläche | Texteditor mit Markdown-Unterstützung und Live-Vorschau               |
| R5  | Konnektivität      | Modul für zukünftige API-Integration (HTTP)                           |

---

## 5. User Stories

Jede Story wird durch einen Code-Review-Prozess auf GitHub validiert.

| Story | Titel                | Beschreibung                                                                                       |
|-------|----------------------|----------------------------------------------------------------------------------------------------|
| US#1  | Datenbank-Setup      | SQLite-Datenbank und QtSql konfigurieren, um die Datenpersistenz zu ermöglichen                    |
| US#2  | Dashboard-UI         | Übersichtliche Hauptoberfläche (Sidebar + Hauptansicht) zur Navigation zwischen Entwürfen          |
| US#3  | Content-Editor       | Texte mit Markdown-Unterstützung verfassen und das finale Layout als Vorschau sehen                |
| US#4  | Suche & Filter       | Notizen nach Schlagworten oder Tags filtern                                                        |
| US#5  | Synchronisations-Mock| "Veröffentlichen"-Button, der den Versand an LetTalk simuliert (Logging + Fehlerbehandlung)        |

---

## 6. Roadmap (Sprints)

| Sprint   | Dauer     | Hauptziel                                              |
|----------|-----------|--------------------------------------------------------|
| Sprint 1 | 2 Wochen  | Datenbankdesign und Konfiguration der Qt-Umgebung      |
| Sprint 2 | 2 Wochen  | Implementierung der CRUD-Logik und der Basis-UI        |
| Sprint 3 | 2 Wochen  | Integration des Markdown-Moduls und der Suchfunktionen |
| Sprint 4 | 2 Wochen  | Finalisierung, Unit-Tests und Code-Review              |

Detaillierter Fortschritt: siehe **`BOARD.md`**

---

## 7. Workflow und Qualitätsstandards

- **GitFlow:** Kein direktes Pushen auf `main`. Nutzung von `feature/`-Branches.
- **Code Review:** Jede Pull Request wird vom Mentor (Senior Engineer) geprüft.
- **Dokumentation:** `README.md` erklärt die Architektur und den Build-Prozess.

---

## 8. SQLite-Datenbank öffnen und anzeigen

Die Datenbank wird als Datei `lettalk.db` im Projektverzeichnis erstellt. Um sie zu öffnen und die Daten zu visualisieren:

### Option A — DB Browser for SQLite (empfohlen)

1. Download: https://sqlitebrowser.org/dl/
2. Öffnen: `File > Open Database > lettalk.db`
3. Tabellen und Daten können direkt angezeigt und bearbeitet werden

### Option B — Terminal

```bash
# SQLite CLI installieren (falls nicht vorhanden)
brew install sqlite

# Datenbank öffnen
sqlite3 lettalk.db

# Tabellen anzeigen
.tables

# Daten einer Tabelle anzeigen
SELECT * FROM posts;

# Beenden
.quit
```

### Option C — Qt Creator

Qt Creator hat ein eingebautes SQL-Plugin. Unter `Tools > SQL` kann die `lettalk.db` direkt geöffnet werden.

---

## 9. Nützliche Ressourcen

- [Qt 6 Dokumentation](https://doc.qt.io/qt-6/)
- [Qt SQL Modul](https://doc.qt.io/qt-6/qtsql-index.html)
- [SQLite Dokumentation](https://www.sqlite.org/docs.html)
- [C++17 Reference](https://en.cppreference.com/w/)
- [CMake Tutorial](https://cmake.org/cmake/help/latest/guide/tutorial/index.html)
