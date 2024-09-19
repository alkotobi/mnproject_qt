#include "mnqry.h"

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
    data.clear();
}

int mnqry::recordCount() const {
    return fRecordCount;
}

bool mnqry::open(QList<QVariant> params) {
   bool ret= this->conn->exec(sql().text(),params,&data);
    fRecordCount = (int)data.count();
   return ret;
}
