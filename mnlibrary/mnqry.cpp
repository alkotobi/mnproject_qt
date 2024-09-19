#include "mnqry.h"

QString mnqry::sql() const
{
    return _sql;
}

void mnqry::setSql(const QString &newSql)
{
    _sql = newSql;
}

bool mnqry::exec(QList<QVariant> params)
{
    if(conn != nullptr && _sql!=""){
         return conn->exec(_sql,params);
    }else
    {
        return false;
    }
}

mnqry::mnqry(mnconnection *conn,QObject *parent)
    :QObject(parent)
{
    this->conn=conn;
}
