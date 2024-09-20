#include "mnqry.h"
#include "MNException.h"

MNSql mnqry::sql() const
{
    return _sql;
}



bool mnqry::exec(const QString& sql,const QList<QVariant>& params)
{
    if(conn != nullptr && sql!=""){
         return conn->exec(sql,params);
    }else
    {
        return false;
    }
}

mnqry::mnqry(mnconnection *conn,const QString& sql, QObject *parent )
    :QObject(parent),_sql(sql)
{
    this->conn=conn;
}

void mnqry::close() {
    if (!fActive) return;
    data.clear();
    fActive= false;
}

int mnqry::recordCount() const {
    return fRecordCount;
}

bool mnqry::open(QList<QVariant> params) {
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

void mnqry::edit() {
    fState = stEdit;
}

bool mnqry::append() {
    fState=stInsert;
    QStringList l;
    for (int i =0;i<_sql.fields()->count();i++){
        l.append("");
    }
    data.append(l);
    if(!goTo(data.count()-1)){
        data.removeLast();
        return false;
    }
    return true;
}

bool mnqry::post() {

    fState=stBrowse;
    return false;
}

bool mnqry::goTo(int ind) {
    if(ind <0 && ind>= data.count()){
        qCritical()<< "index out of range\n";
        return false;
    }
    if(fState != stBrowse){
        if(!post())
            return false;
    }
    this->ind = ind;
    return true;
}
