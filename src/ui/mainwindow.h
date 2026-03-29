#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class DatabaseManager;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(DatabaseManager *dbManager, QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    Ui::MainWindow *ui;
    DatabaseManager *m_dbManager;
};

#endif // MAINWINDOW_H
