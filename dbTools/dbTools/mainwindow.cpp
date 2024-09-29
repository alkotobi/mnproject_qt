#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "mntable.h"
#include "mnconnection_sqlite.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    mnconnection_sqlite conn("/Users/mac/dev/qt/mnproject/mnproject/dbTools/dbTools/db_design.db");
    conn.connect();
    MnTable tbls(&conn,"select * from tables");
    tbls.open();
    MnTable tbl(&conn,"select * from fields where id_table = 1");
}

