#pragma once
#include <QString>
#include "mndb_types.h"





class mnconnection:public QObject
{
    Q_OBJECT

protected:
    bool fActive =false;
public:
    mnconnection();
    explicit mnconnection(QString &db_name,Provider db_type=Sqlite);
    mnconnection(QString &db_name, Provider db_type, QString &server, int port, QString &user_name, QString &password);
    QString db_name;
    QString server;
    int port{};
    QString user_name;
    QString password;
    Provider db_type;
    virtual bool connect()=0;
    virtual bool exec(QString sql)=0;
    virtual bool exec(QString sql,QList<QVariant> params)=0;
    virtual bool close()=0;
    virtual QString errorMessage()=0;
    virtual bool exec(QString sql, QList<QVariant> &params, QList<QStringList> *dataOut, QStringList *fieldNamesOut) =0;
    virtual int getLastInsertedId(QString idName,QString tableName)=0;
    virtual int execInsertSql(const QString &tableName, const QString &fields, const QList<QVariant> &params) =0;
    virtual bool execUpdateSql(const QString &tableName, const QString &fields, const QString &where,
                               const QList<QVariant> &params) =0;
    virtual MnTableDef tableDef(const QString &tableName, const QStringList &fields) =0;
    virtual bool isConnected() = 0;
    virtual bool execCreateTableSql(const MnTableDef &table)=0;
    virtual bool execUpdateTableSql(const MnTableDef &table) = 0;
    virtual bool tableBackup(const QString &originalTableName, const QString newTableName) = 0;

};


