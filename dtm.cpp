#include "dtm.h"
#include "mnlibrary/db_basic.h"
#include "mnlibrary/mnexception.h"
#include <QMessageBox>
#include <QFile>
#include "mnlibrary/mndb_types.h"
#include "mnlibrary/mnconnection_postgres.h"
#include "db_design.h"

Dtm::Dtm() {};

mnconnection *Dtm::connOptions() {
    if (!_connOptions) {

        QString path = makeDbPath("options");
        bool exists = QFile(path).exists();
        _connOptions = new mnconnection_sqlite(path, this);
        if (!_connOptions->connect()) {
            qDebug()<<  "CANT CONNECT TO SQLIE DB:" + path;
            throw MNException("CANT CONNECT TO SQLIE DB:" + path);
        }

        for (const auto &tbl: options_def.tables) {
            if (!exists) {
                if(!_connOptions->execCreateTableSql(tbl)){
                    qDebug()<<  "CANT CREATE TABLE:" + tbl.table_name+"\n"+_connOptions->errorMessage();
                    throw MNException("CANT CREATE TABLE:" + tbl.table_name+"\n"+_connOptions->errorMessage());
                }
            } else {
                if(!_connOptions->execUpdateTableSql(tbl)){
                    qDebug()<<  "CANT UPDATE TABLE:" + tbl.table_name+"\n"+_connOptions->errorMessage();
                    throw MNException("CANT UPDATE TABLE:" + tbl.table_name+"\n"+_connOptions->errorMessage());
                }
            }
        }
        if (!exists) {
            creatLocalDb("default");
            regesterDb({.dbName="default", .isServer=false, .provider=Sqlite});
            activateDb("default");
        }
    }
    return _connOptions;
}

MnTable *Dtm::tblGroup() {
    if (!_tblGroup) {
        _tblGroup = new MnTable(connOptions(), main_groups_def,"",{}, this);
        if (!_tblGroup->open()) {
            qDebug()<<  "CANT OPEN TABLE :" + main_groups_def.table_name;
            throw MNException("CANT OPEN TABLE :" + main_groups_def.table_name);
        }
    }
    return _tblGroup;
}

Dtm::~Dtm() {

}

MnTable *Dtm::tblUser() {
    if (_tblUser == nullptr) {
        _tblUser = new MnTable(connMain(), main_users_def,"",{}, this);
        if (!_tblUser->open()) {
            qDebug()<<  "CANT OPEN TABLE :" + main_users_def.table_name;
            throw MNException("CANT OPEN TABLE :" + main_users_def.table_name);
        }
    }
    return _tblUser;
}

MnTable *Dtm::tblDatabases() {
    if (!_tblDatabases) {
        _tblDatabases = new MnTable(connOptions(), options_databases_def,"",{}, this);
        if (!_tblDatabases->open()) {
            qDebug()<<  "CANT OPEN TABLE :" + options_databases_def.table_name;
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
            qDebug()<<  "CANT connect to db  :" +dbInfo.dbName+" " +_connMain->errorMessage();
            throw MNException("CANT connect to db  :" +dbInfo.dbName+" " +_connMain->errorMessage());
        }
    }

    return _connMain;
}

void Dtm::creatLocalDb(const char *string) {
    mnconnection_sqlite conn(makeDbPath("default"));
    if(!conn.connect()){
        qDebug()<<  "CANT connect to db  :default ;\n" +conn.errorMessage();
        throw MNException("CANT connect to db  :default ;\n"+conn.errorMessage());
    }
    for (const auto& tbl :main_def.tables) {
        if(!conn.execCreateTableSql(tbl)){
            qDebug()<<  "CANT CREATE TABLE:" + tbl.table_name+"\n"+conn.errorMessage();
            throw MNException("CANT CREATE TABLE:" + tbl.table_name+"\n"+conn.errorMessage());
        }
    }
    if(conn.execInsertSql(main_groups_def.table_name,main_groups_name,{"admin"})==-1){
        qDebug()<<  "CANT INSERT :admin in " + main_groups_def.table_name+"\n"+conn.errorMessage();
        throw MNException("CANT INSERT :admin in " + main_groups_def.table_name+"\n"+conn.errorMessage());
    }
    if(conn.execInsertSql(main_users_def.table_name,main_users_name+","+main_users_pass+","+main_users_id_group,{"admin","admin","1"})==-1){
        qDebug()<< "CANT INSERT :admin,admin in " + main_users_def.table_name+"\n"+conn.errorMessage();
        throw MNException("CANT INSERT :admin,admin in " + main_users_def.table_name+"\n"+conn.errorMessage());
    }
}
QString boolToStrInt(bool val){
    if (val)
        return "1";
    else
        return "0";
}

void Dtm::regesterDb(const DbInfo &info) {
    MnTable tbl(_connOptions,"select * from databases");
    if (!tbl.open()){
        qDebug()<< _connOptions->errorMessage();
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
    MnTable tbl(_connOptions,"select * from databases");
    if (!tbl.open()){
        qDebug()<<_connOptions->errorMessage();
        throw MNException(_connOptions->errorMessage());
    }
    if(!tbl.find(options_databases_db_name,name)){
        qDebug()<<"cant find database:" <<name;
        throw MNException("cant find database:"+name);
    }
    tbl.edit();
    tbl.setFieldValue(options_databases_is_active,"1");
    tbl.post();
}

DbInfo fillDbInfo(const MnTable& tbl){
    DbInfo result;
    result.dbName = tbl.fieldByName(options_databases_db_name);
    result.isActive = tbl.fieldByName(options_databases_is_active)=="1";
    result.isServer = tbl.fieldByName(options_databases_is_server) == "1";
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
    MnTable tbl(connOptions(),options_databases_def,options_databases_is_active+"=?",{"1"});
    if (!tbl.open()){
        QMessageBox::critical(nullptr, "Error", connOptions()->errorMessage());
        throw MNException(connOptions()->errorMessage());
    }
    if (tbl.isEmpty()){
        QMessageBox::critical(nullptr, "Error", "no active db");
        throw MNException("no active db");
    }
    return fillDbInfo(tbl);
}
