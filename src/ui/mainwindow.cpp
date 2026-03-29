#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../database/databasemanager.h"

MainWindow::MainWindow(DatabaseManager *dbManager, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_dbManager(dbManager)
{
    ui->setupUi(this);
    setWindowTitle("LetTalk Content Studio");

    if (m_dbManager->isConnected()) {
        statusBar()->showMessage("Database connected", 5000);
    } else {
        statusBar()->showMessage("Database not connected: " + m_dbManager->lastError());
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
