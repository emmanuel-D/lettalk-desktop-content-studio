#include "mainwindow.h"

#include <QApplication>
#include <QtSql>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");

    db.setDatabaseName("Driver={MySQL ODBC 9.6 Unicode Driver};"
                       "Server=localhost;"
                       "Database=qt_project_db;"
                       "UID=root;"
                       "PWD=MySecureRoot123!;"
                       );

    if(!db.open())
    {
        qDebug() << "Db error:" << db.lastError().text();
    }else{
        qDebug() << "Connected to db!";

        QSqlQuery query;
        query.exec(
            "CREATE TABLE IF NOT EXIST users("
            "id INT AUTO_INCREMENT PRIMARY KEY,"
            "name VARCHAR(100),"
            "email VARCHAR(100))"
        );
    }
    //MainWindow w;
    //w.show();
    return a.exec();
}
