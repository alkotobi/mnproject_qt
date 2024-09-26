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
    QStringList fInsertFields;
    int fLimit;
    int fOffset;
    QList<QVariant> fParams;
    bool fChanged;
    void setFields(const QStringList& AValue);

public:
    explicit MNSql(QString sql);
    bool isChanged();
    QStringList insertFields();
    QStringList fields();
    void fieldsClear();
    void fieldAppend(const QString& fld);
    void fieldAppend(const QStringList& flds);
    bool fieldsContains(const QString &fld);
    QString tableName();
    QString text();
    void setGroupedBy(const QString& AValue);
    void setLimit(int AValue);
    void setOffset(int AValue);
    void setOrderBy(QString AValue);
    void setParams(QList<QVariant> AValue);
    void setTableName(QString AValue);
    void setWhere(QString AValue);
    void setChanged(bool AValue);

    void insertFieldsClear();
};
