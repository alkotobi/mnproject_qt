#include "dtm.h"
#include "mnlibrary/db_basic.h"
#include "mnlibrary/mnexception.h"
#include <QMessageBox>
#include <QFile>

Dtm::Dtm() {};

mnconnection *Dtm::connOptions() {
    if (!_connOptions) {

        QString path = makeDbPath("options");
        bool exists = QFile(path).exists();
        _connOptions = new mnconnection_sqlite(path, this);
        if (!_connOptions->connect()) {
            QMessageBox::critical(nullptr, "Error", "CANT CONNECT TO SQLIE DB:" + path);
            throw MNException("CANT CONNECT TO SQLIE DB:" + path);
        }

        for (const auto &tbl: options_def.tables) {
            if (!exists) {
                _connOptions->execCreateTableSql(tbl);
            } else {
                _connOptions->execUpdateTableSql(tbl);
            }
        }
    }
    return _connOptions;
}

MnTable *Dtm::tblGroup() {
    if (!_tblGroup) {
        _tblGroup = new MnTable(connOptions(), main_group_def, this);
        if (!_tblGroup->open()) {
            QMessageBox::critical(nullptr, "Error", "CANT OPEN TABLE :" + main_group_def.table_name);
            throw MNException("CANT OPEN TABLE :" + main_group_def.table_name);
        }
    }
    return _tblGroup;
}

Dtm::~Dtm() {

}

MnTable *Dtm::tblUser() {
    if (!_tblUser) {
        _tblUser = new MnTable(connOptions(), main_user_def, this);
        if (!_tblUser->open()) {
            QMessageBox::critical(nullptr, "Error", "CANT OPEN TABLE :" + main_user_def.table_name);
            throw MNException("CANT OPEN TABLE :" + main_user_def.table_name);
        }
    }
    return _tblUser;
}

MnTable *Dtm::tblDatabases() {
    if (!_tblDatabases) {
        _tblDatabases = new MnTable(connOptions(), options_databases_def, this);
        if (!_tblDatabases->open()) {
            QMessageBox::critical(nullptr, "Error", "CANT OPEN TABLE :" + options_databases_def.table_name);
            throw MNException("CANT OPEN TABLE :" + options_databases_def.table_name);
        }
    }
    return _tblDatabases;
}

mnconnection *Dtm::connMain() {
    if (!_connMain){

    }

    return _connMain;
}
