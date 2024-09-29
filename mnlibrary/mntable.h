#ifndef MNQRY_H
#define MNQRY_H

#include "./mnconnection.h"
#include <QObject>
#include <QVariant>
#include "mnsql.h"
#include "mncustomquery.h"
#include "mncustomdatasource.h"
#include "mndb_types.h"

typedef enum {stEdit,stInsert,stBrowse} MNQryState;
class MnTable : public MnCustomQry {

typedef bool (*MnNotify)(QObject *);
typedef  bool (*MnBeforeSetFieldVal)(MnTable *tbl, const QString &oldVal, QString &newVal);
private:
    mnconnection *conn=nullptr;
    QList<QStringList> data={};
    MNSql _sql;
    int row = -1;
    //int fRecordCount=-1;
    bool fActive=false;
    MNQryState fState=stBrowse;
    QStringList fOldVals;
    bool fNotEdited = true;
    QList<MnNotify> beforeScrollNtfs;
    QList<MnNotify> afterScrollNtfs;
    QList<MnNotify> beforeRemoveNtfs;
    QList<MnNotify> afterRemoveNtfs;
    QList<MnNotify> beforePostNtfs;
    QList<MnNotify> afterPostNtfs;
    QList<MnBeforeSetFieldVal> beforeSetFieldValProcs={};
    QList<MnCustomDataSource*> dataSources;
    MnTableDef fTableDef={};
    QString sqlText();
    QList<QVariant> toVariants(const QStringList& fields);
    bool doBeforeSetFieldVal(MnTable *tbl,const QString &oldVal,QString &newVal);
    bool goToNoBoundChenck(int ind);
public:

    MnTable(mnconnection *conn, MnTableDef table, QObject *parent = nullptr);
    MnTable(mnconnection *conn, QString sql, QObject *parent = nullptr);
    bool exec(const QString &sql,const QList<QVariant>& params = {});
    bool open(QList<QVariant> params = {});
    void close();
    [[nodiscard]] int recordCount() const;
    void edit();
    bool append();
    bool post();
    bool cancel();
    bool goTo(int ind);
    bool next();
    bool prior();
    bool last();
    bool first();
    bool priorFirst();
    bool remove();
    bool execBeforeScroll();
    void execAfterScroll();
    bool execBeforeRemove();
    void execAfterRemove();
    bool execBeforePost();
    void execAfterPost();
    void addDataSource(MnCustomDataSource *dts) override;
    void removeDataSource(MnCustomDataSource *dts)override;
    QString fieldByName(const QString& name)override;
    QString fieldByInd(const int index)override;
    void setFieldValue(const QString& fieldName, const QString &val)override;
    void setFieldValue(int col, const QString &val)override;
    int fieldIndex(const QString& fieldName);
    void printCurrent();
    void printAll();
    void printTableDef();
    QStringList rowAt(int row);
    int rowNo();
    bool isOpen() const;
    MnTableDef tableDef();


};

#endif // MNQRY_H
