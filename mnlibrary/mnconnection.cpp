#include "mnconnection.h"

mnconnection::mnconnection(QString &db_name, Db_type db_type, QObject *parent)
:QObject(parent)
{
    this->db_name = db_name;
    this->db_type = db_type;
    port =-1;
}

mnconnection::mnconnection(QString &db_name, Db_type db_type, QString &server,
                           int port, QString &user_name, QString &password,
                           QObject *parent)
    : mnconnection(db_name, db_type, parent) {
    this->server = server;
    this->port = port;
    this->user_name = user_name;
    this->password = password;
}
