#include "frmunit.h"
#include "ui_frmunit.h"
#include "dtm.h"
#include "db_design.h"
#include "mnexception.h"
#include "mntablemodel.h"
#include "mnmapper.h"


FrmUnit::FrmUnit(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::FrmUnit)
{
    ui->setupUi(this);
    MnTable *tbl = new MnTable(dtm->connMain(),main_unite_def,"",{},this);
    if(!tbl->open()){
        qDebug() << dtm->connMain()->errorMessage();
        throw MNException("frmUnite cant open table unite "+dtm->connMain()->errorMessage());
    }
    MnTableModel *model = new MnTableModel(tbl,this);
    MnMapper * mapper = new MnMapper(model,this);
    ui->tableView->setModel(model);
    ui->dbNav->setMapper(mapper);
    mapper->addDbCtrl(ui->tableView);
}

FrmUnit::~FrmUnit()
{
    delete ui;
}

FrmUnit *frmUnite()
{
    if(!_frmUnite)
        _frmUnite =  new FrmUnit(mainWin);
    return _frmUnite;
}
