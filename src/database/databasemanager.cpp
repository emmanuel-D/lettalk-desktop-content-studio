#include "databasemanager.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

DatabaseManager::DatabaseManager()
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
}

DatabaseManager::~DatabaseManager()
{
    if (m_db.isOpen()) {
        m_db.close();
    }
}

bool DatabaseManager::connect(const QString &dbPath)
{
    m_db.setDatabaseName(dbPath);

    if (!m_db.open()) {
        qWarning() << "DB connection failed:" << m_db.lastError().text();
        return false;
    }

    // Enable foreign keys (SQLite has them off by default)
    QSqlQuery query;
    query.exec("PRAGMA foreign_keys = ON");

    qInfo() << "Connected to database:" << dbPath;
    return createTables();
}

bool DatabaseManager::isConnected() const
{
    return m_db.isOpen();
}

QString DatabaseManager::lastError() const
{
    return m_db.lastError().text();
}

bool DatabaseManager::createTables()
{
    QSqlQuery query;
    bool success = query.exec(
        "CREATE TABLE IF NOT EXISTS posts ("
        "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  title TEXT,"
        "  content TEXT,"
        "  category TEXT,"
        "  created_at DATETIME DEFAULT CURRENT_TIMESTAMP,"
        "  updated_at DATETIME DEFAULT CURRENT_TIMESTAMP"
        ")"
    );

    if (!success) {
        qWarning() << "Failed to create 'posts' table:" << query.lastError().text();
        return false;
    }

    success = query.exec(
        "CREATE TABLE IF NOT EXISTS tags ("
        "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  name TEXT,"
        "  created_at DATETIME DEFAULT CURRENT_TIMESTAMP"
        ")"
    );

    if (!success) {
        qWarning() << "Failed to create 'tags' table:" << query.lastError().text();
        return false;
    }

    qInfo() << "Tables created successfully.";
    return true;
}
