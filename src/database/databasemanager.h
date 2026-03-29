#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QString>

class DatabaseManager
{
public:
    DatabaseManager();
    ~DatabaseManager();

    bool connect(const QString &dbPath);

    bool isConnected() const;
    QString lastError() const;

private:
    QSqlDatabase m_db;

    bool createTables();
};

#endif // DATABASEMANAGER_H
