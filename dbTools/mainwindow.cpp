#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "mntable.h"
#include "mnconnection_sqlite.h"
#include "mnfiles.h"
#include "mndb_types.h"
#include "db_design.h"
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    this->groupDef =group_def ;
    ui->edtDbPath->setText(stringFromFile("./dbPath.txt"));
    ui->edtDotH->setText(stringFromFile("./dotHPath.txt"));
}

MainWindow::~MainWindow() {
    delete ui;
}


QString fromStrIntToStrBool(QString strInt) {
    if (strInt.toInt() == 0) {
        return "false";
    } else
        return "true";
}
QString emptyStrTo0str(QString str){
    if (str == "")
        return "0";
    else return str;
}

void MainWindow::on_pushButton_clicked() {
    if(ui->edtDbPath->text().isEmpty()){
        on_btn_open_db_file_clicked();
        if(ui->edtDbPath->text().isEmpty()){
            QMessageBox::critical(nullptr, "Error", "No file selected.");
            return;
        }
    }

    QString output = "#pragma once\n";
    output += "#include <QString>\n";
    output += "#include <mndb_types.h>\n";

    mnconnection_sqlite conn(ui->edtDbPath->text());
    if (!conn.connect()) {
        qDebug() << conn.errorMessage() + "\n";
    }
    MnTable tbls(&conn, "select * from tables");
    if (!tbls.open()) {
        qDebug() << conn.errorMessage() + "\n";
    }
    tbls.priorFirst();
    while (tbls.next()) {
        output = output + "//-------------------------------------------------------------------\n";
        output = output + "//--------------------------------" + tbls.fieldByName("table_name") +
                 "--------------------------\n";
        output = output + "//-------------------------------------------------------------------\n";
        output = output + "QString " + tbls.fieldByName("table_name") + "_table_name=\"" +
                 tbls.fieldByName("table_name") + "\";\n";
        MnTable fields(&conn, "select * from fields where id_table = " + tbls.fieldByName("id"));
        if (!fields.open()) {
            qDebug() << conn.errorMessage() + "\n";
        }
        QString fld_defs ="";
        fields.priorFirst();
        while (fields.next()) {
            output = output + "QString " + tbls.fieldByName("table_name") + "_" +
                     fields.fieldByName("field_name") + "=\"" + fields.fieldByName("field_name") + "\";\n";
            QString f = QString("const struct MnFieldDef ")+tbls.fieldByName("table_name") +"_"+fields.fieldByName("field_name") + "_def = {"+"\n" +
                        ".field_name = \"" + fields.fieldByName("field_name") + "\",\n" +
                        ".field_type = " + fields.fieldByName("field_type") + ",\n" +
                        ".field_length = " + emptyStrTo0str(fields.fieldByName("field_length")) + ",\n" +
                        ".is_unique = " + fromStrIntToStrBool(fields.fieldByName("is_unique")) + ",\n" +
                        ".is_not_null = " + fromStrIntToStrBool(fields.fieldByName("is_not_null")) + ",\n" +
                        ".is_indexed = " + fromStrIntToStrBool(fields.fieldByName("is_indexed")) + ",\n" +
                        ".default_value = \"" + fields.fieldByName("default_value") + "\",\n" +
                        ".description = \"" + fields.fieldByName("description") + "\",\n" +
                        ".is_required = " + fromStrIntToStrBool(fields.fieldByName("is_required")) + ",\n" +
                        ".is_visible = " + fromStrIntToStrBool(fields.fieldByName("is_visible")) + ",\n" +
                        ".is_read_only = " + fromStrIntToStrBool(fields.fieldByName("is_read_only")) + ",\n" +
                        ".display_width = " + emptyStrTo0str(fields.fieldByName("display_width")) + ",\n" +
                        ".display_label = \"" + fields.fieldByName("display_label") + "\",\n" +
                        ".is_calculated = " + fromStrIntToStrBool(fields.fieldByName("is_calculated")) + ",\n" +
                        ".ind = " + emptyStrTo0str(fields.fieldByName("ind")) + "\n" +
                        "};\n";
            fld_defs +=tbls.fieldByName("table_name") +"_"+fields.fieldByName("field_name") + "_def,";
            output +=f;
        }
        QString t = "const MnTableDef "+tbls.fieldByName("table_name")+"_def ={\n"
                    "        .table_name = \""+tbls.fieldByName("table_name")+"\",\n"
                    "        .fields={"+fld_defs+"},\n"
                    "        .default_data = \""+tbls.fieldByName("default_data")+"\",\n"
                    "        .description = \""+tbls.fieldByName("description")+"\",\n"
                    "        .insert_params_count ="+emptyStrTo0str(tbls.fieldByName("description"))+",\n"
                    "        .is_view="+fromStrIntToStrBool(tbls.fieldByName("description"))+",\n"
                    "        .create_sql = \""+tbls.fieldByName("create_sql")+"\",\n"
                    "};\n ";
        output +=t;

    }
    if(ui->edtDotH->text().isEmpty()){

        ui->edtDotH->setText(pathFromFilePath(conn.db_name)+ "db_design.h");
        stringToFile(ui->edtDotH->text(),"./dotHPath.txt");
    }
    stringToFile(output, ui->edtDotH->text());
}





void MainWindow::on_btn_open_db_file_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(nullptr, "Select Database File", "", "Database Files (*.db)");
    if (filePath.isEmpty()) {
        QMessageBox::critical(nullptr, "Error", "No file selected.");
    } else {
        ui->edtDbPath->setText(filePath);
        stringToFile(filePath,"./dbPath.txt");
    }
}


void MainWindow::on_btn_save_dot_h_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(nullptr, "Save.h File", "", "Header Files (*.h)");
    if (!filePath.isEmpty()) {
        QFile file(filePath);
        if (file.exists()) {
            QMessageBox msgBox;
            msgBox.setWindowTitle("Confirm Overwrite");
            msgBox.setText("The file already exists. Do you want to overwrite it?");
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            msgBox.setIcon(QMessageBox::Question);
            int ret = msgBox.exec();
            if (ret == QMessageBox::Yes) {
                ui->edtDotH->setText(filePath);
            } else {
                QMessageBox::critical(nullptr, "Error", "No file selected.");
                return;
            }
        } else {
            ui->edtDotH->setText(filePath);
        }
    } else {
        QMessageBox::critical(nullptr, "Error", "No file selected.");
        return;
    }
    stringToFile(filePath,"./dotHPath.txt");
}

