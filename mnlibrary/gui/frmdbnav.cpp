#include "frmdbnav.h"
#include "ui_frmdbnav.h"
#include "grafics.h"
#include <QMessageBox>

FrmDbNav::FrmDbNav(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FrmDbNav)
{
    ui->setupUi(this);
    ui->btnCancel->setIcon(iconFromSvg(":/svg/svg/cancel_grad200_48px.svg",colorIcon));
    ui->btnDelete->setIcon(iconFromSvg(":/svg/svg/delete_48px.svg",colorIcon));
    ui->btnFirst->setIcon(iconFromSvg(":/svg/svg/first_page_48px.svg",colorIcon));
    ui->btnLast->setIcon(iconFromSvg(":/svg/svg/last_page_48px.svg",colorIcon));
    ui->btnLock->setIcon(iconFromSvg(":/svg/svg/lock_48px.svg",colorIcon));
    ui->btnNext->setIcon(iconFromSvg(":/svg/svg/navigate_next_48px.svg",colorIcon));
    ui->btnPrior->setIcon(iconFromSvg(":/svg/svg/navigate_before_48px.svg",colorIcon));
    ui->btnRefresh->setIcon(iconFromSvg(":/svg/svg/refresh_48px.svg",colorIcon));
    ui->btnSave->setIcon(iconFromSvg(":/svg/svg/file_save_grad200_48px.svg",colorIcon));

}

FrmDbNav::~FrmDbNav()
{
    delete ui;
}



void FrmDbNav::on_btnFirst_clicked()
{
    table()->first();
    if(tableView()){
        tableView()->setCurrentIndex(tableView()->model()->index(
            0,
            tableView()->currentIndex().column()
            ));
    }
    updateBtns();
}

void FrmDbNav::updateBtns()
{

    ui->btnSave->setEnabled(table() && table()->state() == stEdit || table()->state() == stInsert);
    ui->btnCancel->setEnabled(ui->btnSave->isEnabled());
    ui->btnDelete->setEnabled(table() && !table()->isEmpty());
    ui->btnPrior->setEnabled(table() && table()->rowNo() != 0);
    ui->btnFirst->setEnabled(ui->btnPrior->isEnabled());
    ui->btnNext->setEnabled(table() && table()->rowNo() != table()->recordCount()-1);
    ui->btnLast->setEnabled(ui->btnNext->isEnabled());
    ui->btnRefresh->setEnabled(!ui->btnSave->isEnabled());
}

MnTableView *FrmDbNav::tableView() const
{
    return _tableView;
}

void FrmDbNav::setTableView(MnTableView *newTableView)
{
    _tableView = newTableView;
}

MnTable *FrmDbNav::table() const
{
    return _table;
}

void FrmDbNav::setTable(MnTable *newTable)
{
    _table = newTable;
    updateBtns();
}


void FrmDbNav::on_btnPrior_clicked()
{
    table()->prior();
    if(tableView()){
        tableView()->setCurrentIndex(tableView()->model()->index(
            tableView()->currentIndex().row()-1,
            tableView()->currentIndex().column()
            ));
    }
    updateBtns();
}


void FrmDbNav::on_btnNext_clicked()
{
    table()->next();
    if(tableView()){
        tableView()->setCurrentIndex(tableView()->model()->index(
            tableView()->currentIndex().row()+1,
            tableView()->currentIndex().column()
            ));
    }
    updateBtns();
}


void FrmDbNav::on_btnLast_clicked()
{
    table()->last();
    if(tableView()){
        tableView()->setCurrentIndex(tableView()->model()->index(
            tableView()->model()->rowCount()-1,
            tableView()->currentIndex().column()
            ));
    }
    updateBtns();
}


void FrmDbNav::on_btnCancel_clicked()
{
    table()->cancel();
    updateBtns();
}


void FrmDbNav::on_btnDelete_clicked()
{
    if(QMessageBox::question(nullptr, "CONFIRM SUPPRESSION", "EST CE QUE VOUS ETE SURE DE VOULOIRE SUPPRIMER?", QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes)
        return;
    table()->remove();
    updateBtns();
}


void FrmDbNav::on_btnSave_clicked()
{
    table()->post();
    updateBtns();
}


void FrmDbNav::on_btnRefresh_clicked()
{
    table()->refresh();
    updateBtns();
}

