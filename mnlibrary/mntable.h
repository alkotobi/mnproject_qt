#ifndef MNQRY_H
#define MNQRY_H

#include "./mnconnection.h"
#include <QObject>
#include <QVariant>
#include "mnsql.h"
#include "mncustomdatasource.h"
#include "mndb_types.h"

class MnView;
typedef enum {stEdit,stInsert,stBrowse} MnTablrState;
typedef bool (*MnNotify)(QObject *,void*);//(sender,receiver)
typedef  bool (*MnBeforeSetFieldVal)(MnTable *tbl, const QString &oldVal, QString &newVal);
struct MnNoTifyInfo{
    MnNotify ntf;
    void *receiver;
};

class MnTable: public QObject{
Q_OBJECT

private:

    MNSql sql_;
    MnTablrState fState=stBrowse;
    QStringList fOldVals;
    bool fNotEdited = true;
    QList<QVariant> fParams;
    QList<MnNoTifyInfo> beforeScrollNtfs;
    QList<MnNoTifyInfo> afterScrollNtfs;
    QList<MnNoTifyInfo> beforeRemoveNtfs;
    QList<MnNoTifyInfo> afterRemoveNtfs;
    QList<MnNoTifyInfo> beforePostNtfs;
    QList<MnNoTifyInfo> afterPostNtfs;
    QList<MnNoTifyInfo> onStartEditNtfs;
    QList<MnBeforeSetFieldVal> beforeSetFieldValProcs={};
    QList<MnCustomDataSource*> dataSources;

    QString sqlText();
    QList<QVariant> toVariants(const QStringList& fields);
    bool doBeforeSetFieldVal(MnTable *tbl,const QString &oldVal,QString &newVal);
    bool goToNoBoundChenck(int ind);

    bool _filtered = false;
protected:
    MnTableDef fTableDef_={};
//int fRecordCount=-1;
bool fActive=false;
    int row = -1;
    mnconnection *conn=nullptr;
    QList<QStringList> *data() const;


    QList<QStringList> _data={};
    void mntableCopy(MnTable *des, const MnTable &other);
public:

    MnTable(mnconnection *conn, MnTableDef table,QString where = "", QList<QVariant> params={}, QObject *parent = nullptr);
    MnTable(mnconnection *conn, QString sql, QList<QVariant> params={}, QObject *parent = nullptr);
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
    void setFieldValue(int row,int col, const QString &val);
    int fieldIndex(const QString& fieldName) const;
    void printCurrent();
    void printAll();
    void printTableDef();
    QStringList rowAt(int row);
    QString valueAt(int row,int col);
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
    bool isEmpty();
    MnFieldDef field(int col);

    bool fieldsContains(const char *string);

    void fieldsClear();

    void fieldAppend(QStringList list);

    void insertFieldsClear();

    QString tableName();

    QStringList fields();
    QStringList insertFields();
    MnTablrState state();

    void refresh();
    void addOnStartEditNtf(MnNoTifyInfo ntf);
    void removeOnStartEditNtf(MnNoTifyInfo ntf);
};

#endif // MNQRY_H
