#include "dtm.h"
#include "mnlibrary/db_basic.h"
#include "mnlibrary/mnexception.h"
#include <QMessageBox>

Dtm::Dtm(){} ;

mnconnection *Dtm::connOptions()
{
    if(!_connOptions){
        QString path = makeDbPath("options");
        _connOptions = new mnconnection_sqlite(path, this);
        if(!_connOptions->connect()) {
            QMessageBox::critical(nullptr, "Error", "CANT CONNECT TO SQLIE DB:"+path);
            throw MNException("CANT CONNECT TO SQLIE DB:"+path);
        }
    }
    return _connOptions;
}

MnTable *Dtm::tblGroup() {
    if (!_tblGroup){
        _tblGroup = new MnTable(connOptions(), group_def, this);
       if(!_tblGroup->open()){
           QMessageBox::critical(nullptr, "Error", "CANT OPEN TBLE :"+group_def.table_name);
           throw MNException("CANT OPEN TABLE :"+group_def.table_name);
       }
    }
    return _tblGroup;
}

Dtm::~Dtm() {

}
