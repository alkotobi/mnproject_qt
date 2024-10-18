#include "frmdbnav.h"
#include "ui_frmdbnav.h"
#include "grafics.h"
#include <QMessageBox>
#include "mnmapper.h"


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
    ui->btnAdd->setIcon(iconFromSvg(":/svg/svg/plus.svg",colorIcon));

}

FrmDbNav::~FrmDbNav()
{
    delete ui;
}



void FrmDbNav::on_btnFirst_clicked()
{

}
bool $updateBtns(QObject* sender,void* receiver){
    auto *frm = (FrmDbNav *)receiver;
    QAbstractItemModel *currentModel = frm->mapper()->model();
    frm->ui->btnSave->setEnabled(frm->mapper()->model()->table() && (frm->mapper()->model()->table()->state() == stEdit || frm->mapper()->model()->table()->state() == stInsert));
    frm->ui->btnCancel->setEnabled(frm->ui->btnSave->isEnabled());
    frm->ui->btnDelete->setEnabled(frm->mapper()->model()->table() && !frm->mapper()->model()->table()->isEmpty());
    frm->ui->btnPrior->setEnabled(frm->mapper()->model()->table() && frm->mapper()->model()->table()->rowNo() != 0);
    frm->ui->btnFirst->setEnabled(frm->ui->btnPrior->isEnabled());
    frm->ui->btnNext->setEnabled(frm->mapper()->model()->table() && frm->mapper()->model()->table()->rowNo() != frm->mapper()->model()->table()->recordCount()-1);
    frm->ui->btnLast->setEnabled(frm->ui->btnNext->isEnabled());
    frm->ui->btnRefresh->setEnabled(!frm->ui->btnSave->isEnabled());
    frm->ui->btnAdd->setEnabled(frm->mapper()->model()->table()->state() != stInsert);
    return true;
}
void FrmDbNav::updateBtns()
{
    $updateBtns(nullptr,this);
//    ui->btnSave->setEnabled(table() && table()->state() == stEdit || table()->state() == stInsert);
//    ui->btnCancel->setEnabled(ui->btnSave->isEnabled());
//    ui->btnDelete->setEnabled(table() && !table()->isEmpty());
//    ui->btnPrior->setEnabled(table() && table()->rowNo() != 0);
//    ui->btnFirst->setEnabled(ui->btnPrior->isEnabled());
//    ui->btnNext->setEnabled(table() && table()->rowNo() != table()->recordCount()-1);
//    ui->btnLast->setEnabled(ui->btnNext->isEnabled());
//    ui->btnRefresh->setEnabled(!ui->btnSave->isEnabled());
}





void FrmDbNav::on_btnPrior_clicked()
{

}


void FrmDbNav::on_btnNext_clicked()
{

}


void FrmDbNav::on_btnLast_clicked()
{

}


void FrmDbNav::on_btnCancel_clicked()
{

}


void FrmDbNav::on_btnDelete_clicked()
{

}


void FrmDbNav::on_btnSave_clicked()
{

}


void FrmDbNav::on_btnRefresh_clicked()
{

}

MnMapper *FrmDbNav::mapper() const
{
    return _mapper;
}

void FrmDbNav::setMapper(MnMapper *newMapper)
{
    if (_mapper == newMapper){
        return;
    }
    if (_mapper){
        _mapper->removeDbCtrl(this);
    }
    _mapper = newMapper;
    _mapper->addDbCtrl(this);
    this->updateBtns();
}

QPushButton *FrmDbNav::btnFirst() {
    return ui->btnFirst;
}

QPushButton *FrmDbNav::btnNext() {
    return ui->btnNext;
}

QPushButton *FrmDbNav::btnPrior() {
    return ui->btnPrior;
}

QPushButton *FrmDbNav::btnLast() {
    return ui->btnLast;
}

QPushButton *FrmDbNav::btnRefresh() {
    return ui->btnRefresh;
}

QPushButton *FrmDbNav::btnCancel() {
    return ui->btnCancel;
}

QPushButton *FrmDbNav::btnDelete() {
    return ui->btnDelete;
}

QPushButton *FrmDbNav::btnSave() {
    return ui->btnSave;
}

QPushButton *FrmDbNav::btnAdd()
{
    return ui->btnAdd;
}

