#ifndef MNQRY_H
#define MNQRY_H

#include "mnconnection.h"
#include <QObject>
#include <QVariant>
#include "mnsql.h"
#include "mncustomquery.h"
#include "mncustomdatasource.h"
#include "mndb_types.h"

typedef enum {stEdit,stInsert,stBrowse} MNQryState;
class MnQry : public MnCustomQry {

typedef bool (*MnNotify)(QObject *);
private:
    mnconnection *conn=nullptr;
    QList<QStringList> data={};
    MNSql _sql;
    int ind = -1;
    int fRecordCount=-1;
    bool fActive=false;
    MNQryState fState=stBrowse;
    QList<MnNotify> beforeScrollNtfs;
    QList<MnNotify> afterScrollNtfs;
    QList<MnCustomDataSource*> dataSources;
    MnTableDef tableDef={};
    QString sqlText();
    long long fieldIndex(QString fieldName);
public:

    MnQry(mnconnection *conn, MnTableDef table, QObject *parent = nullptr);
    MnQry(mnconnection *conn, QString sql, QObject *parent = nullptr);
    bool exec(const QString &sql,const QList<QVariant>& params = {});
    bool open(QList<QVariant> params = {});
    void close();
    [[nodiscard]] int recordCount() const;
    void edit();
    bool append();
    bool post();
    bool goTo(int ind);
    bool next();
    bool prior();
    bool last();
    bool first();
    bool eof();
    bool bof();
    bool execBeforeScroll();
    void execAfterScroll();

    // MnCustomQry interface
public:
    void addDataSource(MnCustomDataSource *dts) override;
    void removeDataSource(MnCustomDataSource *dts)override;
    QString* fieldByName(const QString& name)override;
    QString* fieldByInd(const int index)override;
    void print();
};

#endif // MNQRY_H
