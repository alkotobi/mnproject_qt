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
    //TODO MAKE TH MNTABLE FROM SQL
    this->conn=conn;
    this->table = conn->tableDef(_sql.tableName());
}

MnQry::MnQry(mnconnection *conn, mntable table, QObject *parent)
:MnQry(conn,table.select_sql(),parent)
{
    //TODO:CAN BE FASTER BY CREATING _sql better way
    this->table = table;

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
    if(data[ind][0] == ""){
       //do insert
        this->conn->insertSql(_sql.tableName(),_sql.fields()->join(","));
    }else{
       //do update
    }
    fState=stBrowse;
    return false;
}

bool MnQry::goTo(int ind) {
    if (data.empty()) return false;
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
    //update controls
    for (int i = 0; i < dataSources.count(); ++i) {
        dataSources[i]->updateControls();
    }
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

bool MnQry::next() {
    if (ind != data.count()-1){
        goTo(ind+1);
        return true;
    }
    return false;
}

bool MnQry::prior() {
    if (ind != 0){
        goTo(ind-1);
        return true;
    }
    return false;
}

bool MnQry::last() {
    int i =data.count()-1;
    if (ind != i){
        goTo(i);
        return true;
    }
    return false;
}

bool MnQry::first() {
    if (ind != 0){
        goTo(0);
        return true;
    }
    return false;
}

bool MnQry::eof() {
    return ind == data.count()-1;
}

bool MnQry::bof() {
    return ind == 0;
}


