#include "dtm.h"
#include "mnlibrary/db_basic.h"
#include "mnlibrary/mnexception.h"
#include <QMessageBox>
#include <QFile>
#include "mnlibrary/mndb_types.h"
#include "mnlibrary/mnconnection_postgres.h"

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
                creatLocalDb("default");
                regesterDb({.dbName="default",.isServer=false,.provider=Sqlite});
                activateDb("default");
            } else {
                _connOptions->execUpdateTableSql(tbl);
            }
        }
    }
    return _connOptions;
}

MnTable *Dtm::tblGroup() {
    if (!_tblGroup) {
        _tblGroup = new MnTable(connOptions(), main_group_def,"",{}, this);
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
        _tblUser = new MnTable(connOptions(), main_user_def,"",{}, this);
        if (!_tblUser->open()) {
            QMessageBox::critical(nullptr, "Error", "CANT OPEN TABLE :" + main_user_def.table_name);
            throw MNException("CANT OPEN TABLE :" + main_user_def.table_name);
        }
    }
    return _tblUser;
}

MnTable *Dtm::tblDatabases() {
    if (!_tblDatabases) {
        _tblDatabases = new MnTable(connOptions(), options_databases_def,"",{}, this);
        if (!_tblDatabases->open()) {
            QMessageBox::critical(nullptr, "Error", "CANT OPEN TABLE :" + options_databases_def.table_name);
            throw MNException("CANT OPEN TABLE :" + options_databases_def.table_name);
        }
    }
    return _tblDatabases;
}

mnconnection *Dtm::connMain() {
    if (!_connMain){
        DbInfo dbInfo = activeDbName();
        switch (dbInfo.provider) {

            case Sqlite:
                _connMain =new mnconnection_sqlite(makeDbPath(dbInfo.dbName), this);
                break;
            case Postgres:
                _connMain = new mnconnection_postgres(dbInfo.dbName,dbInfo.server,dbInfo.port,dbInfo.userName,dbInfo.password,this);

                break;
            default:
                throw MNException("unhandled provider");
                break;
        }
        if (!_connMain->connect()){
            QMessageBox::critical(nullptr, "Error", "CANT connect to db  :" +dbInfo.dbName+" " +_connMain->errorMessage());
            throw MNException("CANT connect to db  :" +dbInfo.dbName+" " +_connMain->errorMessage());
        }
    }

    return _connMain;
}

void Dtm::creatLocalDb(const char *string) {
    mnconnection_sqlite conn(makeDbPath("default"));
    for (const auto& tbl :main_def.tables) {
        conn.execCreateTableSql(tbl);
    }
    conn.execInsertSql(main_group_def.table_name,main_group_name,{"admin"});
    conn.execInsertSql(main_user_def.table_name,main_user_name+","+main_user_pass,{"admin","admin"});
}
QString boolToStrInt(bool val){
    if (val)
        return "1";
    else
        return "0";
}

void Dtm::regesterDb(const DbInfo &info) {
    MnTable tbl(_connOptions,options_databases_def);
    if (!tbl.open()){
        QMessageBox::critical(nullptr, "Error", _connOptions->errorMessage());
        throw MNException(_connOptions->errorMessage());
    }
    tbl.append();
    tbl.setFieldValue(options_databases_db_name, info.dbName);
    tbl.setFieldValue(options_databases_is_active,boolToStrInt(info.isActive));
    tbl.setFieldValue(options_databases_is_server, boolToStrInt(info.isServer));
    tbl.setFieldValue(options_databases_provider,QString::number(info.provider));
    tbl.setFieldValue(options_databases_server, info.server);
    tbl.setFieldValue(options_databases_port, QString::number(info.port));
    tbl.setFieldValue(options_databases_user_name, info.userName);
    tbl.setFieldValue(options_databases_password, info.password);
    tbl.post();
}

void Dtm::activateDb(const QString &name) {
    MnTable tbl(_connOptions,options_databases_def);
    if (!tbl.open()){
        QMessageBox::critical(nullptr, "Error", _connOptions->errorMessage());
        throw MNException(_connOptions->errorMessage());
    }
    if(!tbl.find(options_databases_db_name,name)){
        QMessageBox::critical(nullptr, "Error", "cant find database:"+name);
        throw MNException("cant find database:"+name);
    }
    tbl.edit();
    tbl.setFieldValue(options_databases_is_active,"1");
    tbl.post();
}

DbInfo fillDbInfo(const MnTable& tbl){
    DbInfo result;
    result.dbName = tbl.fieldByName(options_databases_db_name);
    result.isActive = true;
    result.isServer = tbl.fieldByName(options_databases_is_active) == "1";
    result.password = tbl.fieldByName(options_databases_password);
    result.port = tbl.fieldByName(options_databases_port).toInt();
    result.server = tbl.fieldByName(options_databases_server);
    result.userName = tbl.fieldByName(options_databases_user_name);
    result.provider = (Provider)tbl.fieldByName(options_databases_provider).toInt();
    return result;
}

DbInfo Dtm::dbInfo(QString name){
    MnTable tbl(_connOptions,options_databases_def,options_databases_db_name+"=?",{name});
    if (!tbl.open()){
        QMessageBox::critical(nullptr, "Error", _connOptions->errorMessage());
        throw MNException(_connOptions->errorMessage());
    }
    if (tbl.isEmpty()){
        QMessageBox::critical(nullptr, "Error", "no active db");
        throw MNException("no active db");
    }

    return fillDbInfo(tbl);
}

DbInfo Dtm::activeDbName() {
    MnTable tbl(_connOptions,options_databases_def,options_databases_is_active+"=?",{"1"});
    if (!tbl.open()){
        QMessageBox::critical(nullptr, "Error", _connOptions->errorMessage());
        throw MNException(_connOptions->errorMessage());
    }
    if (tbl.isEmpty()){
        QMessageBox::critical(nullptr, "Error", "no active db");
        throw MNException("no active db");
    }
    return fillDbInfo(tbl);
}
