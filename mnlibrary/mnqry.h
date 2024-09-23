#ifndef MNQRY_H
#define MNQRY_H

#include "mnconnection.h"
#include <QObject>
#include <QVariant>
#include "mnsql.h"
#include "mncustomquery.h"
#include "mncustomdatasource.h"

typedef enum {stEdit,stInsert,stBrowse} MNQryState;
class MnQry : public MnCustomQry {

typedef bool (*MnNotify)(QObject *);
private:
    mnconnection *conn=nullptr;
    QList<QStringList> data={};
    int ind = -1;
    int fRecordCount=-1;
    MNSql _sql;
    bool fActive=false;
    MNQryState fState=stBrowse;
    QList<MnNotify> beforeScrollNtfs;
    QList<MnNotify> afterScrollNtfs;
    QList<MnCustomDataSource*> dataSources;
public:
    MnQry(mnconnection *conn, const QString& sql, QObject *parent = nullptr);
    [[nodiscard]] MNSql sql() const;
    bool exec(const QString &sql,const QList<QVariant>& params = {});
    bool open(QList<QVariant> params = {});
    void close();
    [[nodiscard]] int recordCount() const;
    void edit();
    bool append();
    bool post();
    bool goTo(int ind);
    bool execBeforeScroll();
    void execAfterScroll();

    // MnCustomQry interface
public:
    void addDataSource(MnCustomDataSource *dts) override;
    void removeDataSource(MnCustomDataSource *dts)override;
    QString* fieldByName(const QString& name)override;
    QString* fieldByInd(const int index)override;
};

#endif // MNQRY_H
