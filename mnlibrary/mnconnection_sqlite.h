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
private:
    QString insertSql(const QString &tableName, const QString &fields) ;
    QString updateSql(const QString &tableName, const QString &fields, const QString &where) ;

    // mnconnection interface
public:
    mnconnection_sqlite();
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
  MnTableDef tableDef(const QString &tableName, const QStringList &fields) override;

    int execInsertSql(const QString &tableName, const QString &fields, const QList<QVariant> &params) override;

    bool execUpdateSql(const QString &tableName, const QString &fields, const QString &where,
                       const QList<QVariant> &params) override;

    bool isConnected() override;

    bool execCreateTableSql(const MnTableDef &table) override;

    bool execUpdateTableSql(const MnTableDef &table) override;

    bool tableBackup(const QString &originalTableName, const QString newTableName) override;

};

#endif // MNCONNECTION_SQLITE_H
