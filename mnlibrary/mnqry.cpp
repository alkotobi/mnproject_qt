#include "mnqry.h"
#include "mnexception.h"

MNSql MnQry::sql() const
{
    return _sql;
}



bool MnQry::exec(const QString& sql, const QList<QVariant>& params)
{
    if(conn != nullptr && sql!=""){
         return conn->exec(sql,params);
    }else
    {
        return false;
    }
}

MnQry::MnQry(mnconnection *conn, const QString& sql, QObject *parent )
    :MnCustomQry(parent),_sql(sql)
{
    this->conn=conn;
}

void MnQry::close() {
    if (!fActive) return;
    data.clear();
    fActive= false;
}

int MnQry::recordCount() const {
    return fRecordCount;
}

bool MnQry::open(QList<QVariant> params) {
    if(fActive){
        throw MNException("can not perform this operation on an open qry");
    }
    QStringList *fld = _sql.fields();
    if(!fld->contains("*")){
        fld= nullptr;
    }
   bool ret= this->conn->exec(sql().text(), params, &data, fld);
    fRecordCount = (int)data.count();
    if (ret) {
        fActive = true;
        fState = stBrowse;
    }
    else fActive = false;
   return ret;
}

void MnQry::edit() {
    fState = stEdit;
}

bool MnQry::append() {
    if (fState != stBrowse) post();
    fState=stInsert;
    QStringList l;
    for (int i =0;i<_sql.fields()->count();i++){
        l.append("");
    }
    data.append(l);
    if(!goTo((data.count()-1))){
        data.removeLast();
        return false;
    }
    return true;
}

bool MnQry::post() {

    fState=stBrowse;
    return false;
}

bool MnQry::goTo(int ind) {
    if (!execBeforeScroll()) return false;
    if(ind <0 && ind>= data.count()){
        qCritical()<< "index out of range\n";
        return false;
    }
    if(fState != stBrowse){
        if(!post())
            return false;
    }
    this->ind = ind;
    execAfterScroll();
    return true;
}

bool MnQry::execBeforeScroll() {
    bool ret= true;
    for (int i = 0; i < beforeScrollNtfs.count(); ++i) {
        ret=ret && beforeScrollNtfs[i](this);
    }
    return ret;
}

void MnQry::execAfterScroll() {
    for (int i = 0; i < beforeScrollNtfs.count(); ++i) {
        afterScrollNtfs[i](this);
    }
}

void MnQry::addDataSource(MnCustomDataSource* dts)
{
    if(dataSources.contains(dts)) return;
    dataSources.append(dts);
}

void MnQry::removeDataSource(MnCustomDataSource *dts)
{
    dataSources.removeOne(dts);
}

QString *MnQry::fieldByName(const QString &name)
{
    int index =this->sql().fields()->indexOf(name);
    if(index < 0 ) throw MNException(name + "is not a correct field name");
    return &(data[this->ind][index]);
}

QString *MnQry::fieldByInd(const int index)
{
    return &(data[this->ind][index]);
}


