#include "frmunit.h"
#include "ui_frmunit.h"
#include "dtm.h"
#include "db_design.h"
#include "mnexception.h"
#include "mntablemodel.h"
#include "mnmapper.h"
#include <QKeyEvent>

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
    ui->tableView->setMapper(mapper);
    ui->dbNav->setMapper(mapper);
    //ui->tableView->viewport()->installEventFilter(this);
}

// bool FrmUnit::eventFilter(QObject* object, QEvent* event) {
//     if (object == ui->tableView->viewport()) {
//         if (event->type() == QEvent::KeyPress) {
//             QKeyEvent *e =static_cast<QKeyEvent*>(event);
//             qDebug()<< e->key();
//         }

//     }
//     return false;
// }

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
