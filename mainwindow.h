#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

public slots:
   static void edited();
       static void edited2();
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_assign_function_to_signal_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
