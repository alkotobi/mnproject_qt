#pragma once

#include <QObject>
#include <QString>



typedef enum{Sqlite,Postgres,Mysql} Db_type;

class mnconnection : public QObject
{
        Q_OBJECT
public:
    mnconnection(QString &db_name,Db_type db_type=Sqlite,QObject *parent=nullptr);
    mnconnection(QString &db_name, Db_type db_type, QString &server, int port, QString &user_name, QString &password , QObject *parent=nullptr);
    QString db_name;
    QString server;
    int port{};
    QString user_name;
    QString password;
    Db_type db_type;
    virtual bool connect()=0;
    virtual bool exec(QString sql)=0;
    virtual bool exec(QString sql,QList<QVariant> params)=0;
    virtual bool close()=0;
    virtual QString errorMessage()=0;
    virtual bool exec(QString sql,QList<QVariant>& params,QList<QStringList> *out)=0;
    virtual int getLastInsertedId(QString idName,QString tableName)=0;
};


