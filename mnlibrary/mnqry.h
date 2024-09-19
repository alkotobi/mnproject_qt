#ifndef MNQRY_H
#define MNQRY_H

#include "mnconnection.h"
#include <QObject>
#include <QVariant>

class mnqry : public QObject {
   Q_OBJECT

private:
    mnconnection *conn=nullptr;
    QString _sql="";
    QList<QStringList> data={};
    int ind = -1;
    int record_count=-1;

public:
    mnqry(mnconnection *conn, QObject *parent = nullptr);
    QString sql() const;
    void setSql(const QString &newSql);
    bool exec(QList<QVariant> params = {});
    bool open(QList<QVariant> params = {});
    void close();
    int recordCount();
};

#endif // MNQRY_H
