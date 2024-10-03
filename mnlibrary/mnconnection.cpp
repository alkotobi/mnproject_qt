#include "mnconnection.h"

mnconnection::mnconnection()
{
    fActive = false;
}

mnconnection::mnconnection(QString &db_name, Provider db_type)
{
    this->db_name = db_name;
    this->db_type = db_type;
    port =-1;
}

mnconnection::mnconnection(QString &db_name, Provider db_type, QString &server,
                           int port, QString &user_name, QString &password)
    : mnconnection(db_name, db_type) {
    this->server = server;
    this->port = port;
    this->user_name = user_name;
    this->password = password;
}
