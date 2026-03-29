#include "src/ui/mainwindow.h"
#include "src/database/databasemanager.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    DatabaseManager dbManager;
    dbManager.connect("lettalk.db");

    MainWindow window(&dbManager);
    window.show();

    return app.exec();
}
