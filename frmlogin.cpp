#include "frmlogin.h"
#include "ui_frmlogin.h"
#include "dtm.h"
#include "db_design.h"

FrmLogin::FrmLogin(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::FrmLogin)
{
    ui->setupUi(this);
}

FrmLogin::~FrmLogin()
{
    delete ui;
}

void FrmLogin::on_btnCancel_clicked()
{

    done(0);
}


void FrmLogin::on_btnLogin_clicked()
{
    if(dtm->tblUser()->find(main_users_name,ui->edtName->text())){
        if(dtm->tblUser()->fieldByName(main_users_pass) == ui->edtPass->text()){
          done(1);
            return;
        }
    }
    return;


}

