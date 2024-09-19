#pragma once

#include <QObject>
#include <QVariant>
#include <QStringList>
#include <QList>


class MNSql : public QObject
{
Q_OBJECT
private:
    QString fText;
    QString fTableName;
    QString fFields;
    QString fWhere;
    QString fOrderBy;
    QString fGroupedBy;
    int fLimit;
    int fOffset;
    QList<QVariant> fParams;
    bool fChanged;
    QStringList fInsertFields;

public:
    explicit MNSql(QString sql,QObject *parent = nullptr);
    bool isChanged();
    QStringList insertFields();
    QString tableName();
    QString text();
    void setFields(QString AValue);
    void setGroupedBy(QString AValue);
    void setInsertFields(QStringList AValue);
    void setLimit(int AValue);
    void setOffset(int AValue);
    void setOrderBy(QString AValue);
    void setParams(QList<QVariant> AValue);
    void setTableName(QString AValue);
    void setWhere(QString AValue);
    void setChanged(bool AValue);

signals:


};
