#ifndef MNCONNECTION_SQLITE_H
#define MNCONNECTION_SQLITE_H

#include "mnconnection.h"
#include <QVariant>
#include <QMetaType>

extern "C" {
#include "./sqlite3.h"
}

class mnconnection_sqlite : public mnconnection
{
public:


    // mnconnection interface
public:
    mnconnection_sqlite(QString db_name,QObject *parent=nullptr);
    bool connect();
    bool exec(QString sql);
    bool exec(QString sql, QList<QVariant> params);
    bool close();
    QString error_message();
private:
  sqlite3 *db;



};

#endif // MNCONNECTION_SQLITE_H
