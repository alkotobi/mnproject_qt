#ifndef MNCONNECTION_SQLITE_H
#define MNCONNECTION_SQLITE_H

#include "mnconnection.h"
#include <QVariant>
#include <QMetaType>

extern "C" {
#include "./sqlite3.h"
}

#include "mndb_types.h"

class mnconnection_sqlite : public mnconnection
{
public:


    // mnconnection interface
public:
    explicit mnconnection_sqlite(QString db_name,QObject *parent=nullptr) ;
    ~mnconnection_sqlite();
    bool connect() override;
    bool exec(QString sql) override;
    bool exec(QString sql, QList<QVariant> params) override;
    bool close() override;
    QString errorMessage() override;

    bool exec(QString sql, QList<QVariant> &params, QList<QStringList> *dataOut, QStringList *fieldNamesOut) override;

    int getLastInsertedId(QString idName, QString tableName) override;

private:
  sqlite3 *db=nullptr;
  public:
  QString insertSql(const QString &tableName, const QString &fields) override;
  QString updateSql(const QString &tableName, const QString &fields) override;
  mntable tableDef(const QString &tableName) override;
};

#endif // MNCONNECTION_SQLITE_H
