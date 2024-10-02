#ifndef MNQRY_H
#define MNQRY_H

#include "./mnconnection.h"
#include <QObject>
#include <QVariant>
#include "mnsql.h"
#include "mncustomdatasource.h"
#include "mndb_types.h"

class MnView;
typedef enum {stEdit,stInsert,stBrowse} MNQryState;
struct MnDataSetCol {
private:
    bool _filtered = false;
    bool _edited = false;
    QString _value;
public:
    const QString &value() const;

    void setValue(const QString &value);

    bool isEdited() const;

    void setEdited(bool edited);

    bool isFiltered() const;

    void setFiltered(bool filtered);

public:
};
typedef QList<MnDataSetCol> MnDataSet;

class MnTable: public QObject{
Q_OBJECT
typedef bool (*MnNotify)(QObject *);
typedef  bool (*MnBeforeSetFieldVal)(MnTable *tbl, const QString &oldVal, QString &newVal);
private:

    MNSql _sql;
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

    QString sqlText();
    QList<QVariant> toVariants(const QStringList& fields);
    bool doBeforeSetFieldVal(MnTable *tbl,const QString &oldVal,QString &newVal);
    bool goToNoBoundChenck(int ind);

    bool _filtered = false;
protected:
    MnTableDef fTableDef={};
//int fRecordCount=-1;
bool fActive=false;
    int row = -1;
    mnconnection *conn=nullptr;
    QList<QStringList> *data() const;


    QList<QStringList> _data={};
public:

    MnTable(mnconnection *conn, MnTableDef table, QObject *parent = nullptr);
    MnTable(mnconnection *conn, QString sql, QObject *parent = nullptr);
    bool exec(const QString &sql,const QList<QVariant>& params = {});
    bool open(QList<QVariant> params = {});
    void close();
    [[nodiscard]] int recordCount();
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
    void addDataSource(MnCustomDataSource *dts) ;
    void removeDataSource(MnCustomDataSource *dts);
    QString fieldByName(const QString& name) const ;
    QString fieldByInd(const int index);
    void setFieldValue(const QString& fieldName, const QString &val);
    void setFieldValue(int col, const QString &val);
    int fieldIndex(const QString& fieldName) const;
    void printCurrent();
    void printAll();
    void printTableDef();
    QStringList rowAt(int row);
    int rowNo();
    bool isOpen() const;
    MnTableDef tableDef();
    void setFiltered(bool f);
    MnTable& operator=(const MnTable &other);
    MnTable(const MnTable &other);
    bool find(const QString &fieldName,const QString& value);
    bool findId(int id);
    bool findId(QString id);
    MnView filter(const std::function<bool()>& lambda_search);
};

#endif // MNQRY_H
