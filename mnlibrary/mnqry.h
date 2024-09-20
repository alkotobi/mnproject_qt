#ifndef MNQRY_H
#define MNQRY_H

#include "mnconnection.h"
#include <QObject>
#include <QVariant>
#include "mnsql.h"

typedef enum {stEdit,stInsert,stBrowse} MNQryState;
class mnqry : public QObject {
   Q_OBJECT

private:
    mnconnection *conn=nullptr;
    QList<QStringList> data={};
    int ind = -1;
    int fRecordCount=-1;
    MNSql _sql;
    bool fActive=false;
    MNQryState fState=stBrowse;

public:
    mnqry(mnconnection *conn,const QString& sql, QObject *parent = nullptr);
    [[nodiscard]] MNSql sql() const;
    bool exec(const QString &sql,const QList<QVariant>& params = {});
    bool open(QList<QVariant> params = {});
    void close();
    [[nodiscard]] int recordCount() const;
    void edit();
    bool append();
    bool post();
    bool goTo(int ind);
};

#endif // MNQRY_H
