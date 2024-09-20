#pragma once

#include <QObject>
#include <QVariant>
#include <QStringList>
#include <QList>


struct MNSql
{

private:
    QString fText;
    QString fTableName;
    QStringList fFields;
    QString fWhere;
    QString fOrderBy;
    QString fGroupedBy;
    int fLimit;
    int fOffset;
    QList<QVariant> fParams;
    bool fChanged;
    QStringList fInsertFields;

public:
    explicit MNSql(QString sql);
    bool isChanged();
    QStringList insertFields();
    QStringList *fields();
    QString tableName();
    QString text();
    void setFields(QStringList AValue);
    void setGroupedBy(QString AValue);
    void setInsertFields(QStringList AValue);
    void setLimit(int AValue);
    void setOffset(int AValue);
    void setOrderBy(QString AValue);
    void setParams(QList<QVariant> AValue);
    void setTableName(QString AValue);
    void setWhere(QString AValue);
    void setChanged(bool AValue);
};
