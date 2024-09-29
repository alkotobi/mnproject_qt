#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mndb_types.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    MnTableDef groupDef;
private slots:
    void on_pushButton_clicked();


    void on_btn_open_db_file_clicked();

    void on_btn_save_dot_h_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
