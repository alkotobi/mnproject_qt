#include "mnsql.h"
#include "mnstrings.h"
#include "MNException.h"

MNSql::MNSql(QString sql) {
    fText = sql;
    fChanged = false;
    QString lowerCaseSql = sql.toLower();
    sql = removeExtraSpacesForSqlText(sql);
    lowerCaseSql = lowerCaseSql.remove(0, 7).replace(";", "");
    QStringList strs;
    strs = lowerCaseSql.split("offset");
    if (strs.length() == 2)
        fOffset = strs[1].simplified().toInt();
    else
        fOffset = -1;
    lowerCaseSql = strs[0];
    strs = lowerCaseSql.split("limit");
    if (strs.length() == 2)
        fLimit = strs[1].simplified().toInt();
    else
        fLimit = 0;

    lowerCaseSql = strs[0];
    strs = lowerCaseSql.split("group by");
    if (strs.length() == 2)
        fGroupedBy = strs[1].simplified();
    else
        fGroupedBy = "";

    lowerCaseSql = strs[0];
    strs = lowerCaseSql.split("order by");
    if (strs.length() == 2)
        fOrderBy = strs[1].simplified();
    else
        fOrderBy = "";

    lowerCaseSql = strs[0];
    strs = lowerCaseSql.split("where");
    if (strs.length() == 2)
        fWhere = strs[1].simplified();
    else
        fWhere = "";

    lowerCaseSql = strs[0];
    strs = lowerCaseSql.split("from");
    if (strs.length() == 2)
        fTableName = strs[1].simplified();
    else
        throw MNException(QString("wrong sql: " + sql));

    fFields = strs[0].simplified();
}

bool MNSql::isChanged() {
    return fChanged;
}

QStringList MNSql::insertFields() {
    QStringList strs;
    QStringList strs2;
    QString table_name;
    QStringList flds;

    if (!fInsertFields.isEmpty()) {
        return fInsertFields;
    }
    strs = this->fTableName.split(',');
    setTableName(strs[0]);
    strs = this->fFields.split(',');
    for (const QString& str : strs) {
        strs2 = str.split('.');
        if (strs2.length() == 2) {
            if (strs2[0] == table_name) {
                flds.append(strs2[1]);
            }
        } else {
            flds.append(strs2[0]);
        }
    }
    fInsertFields = flds;
    return flds;
}

QString MNSql::text() {

        if (!isChanged())
        {
            return fText;
        }
        QString result = "SELECT " + fFields + " FROM " + fTableName;
        if (fWhere!= "")
            result += " WHERE " + fWhere;
        if (fOrderBy!= "")
        {
            result += "ORDER BY " + fOrderBy;
        }
        if (fGroupedBy!= "")
            result += " GROUP BY " + fGroupedBy;
        if (fLimit > 0)
        {
            result += " limit " + QString::number(fLimit);
            if (fOffset >= 0)
                result += " OFFSET " + QString::number(fOffset);
        }
        result += ";";
        fText = result;
        fChanged = false;
        return result;

}

void MNSql::setFields(QString AValue) {
    if (fFields == AValue) return;
    fFields = AValue;
    fChanged = true;
}

void MNSql::setGroupedBy(QString AValue) {
    if(fGroupedBy == AValue) return;
    fGroupedBy = AValue;
    fChanged = true;
}

void MNSql::setInsertFields(QStringList AValue) {
    fInsertFields = AValue;
}

void MNSql::setLimit(int AValue) {
    if (fLimit == AValue) return;
    fLimit = AValue;
    fChanged = true;
}

void MNSql::setOffset(int AValue) {
    if(fOffset == AValue) return;
    fOffset = AValue;
    fChanged = true;
}

void MNSql::setOrderBy(QString AValue) {
    if (fOrderBy == AValue) return;
    fOrderBy = AValue;
    fChanged = true;
}

void MNSql::setParams(QList<QVariant> AValue) {
    if(fParams == AValue) return;
    fParams = AValue;
    fChanged = true;
}

void MNSql::setTableName(QString AValue) {
    if (fTableName == AValue) return;
    fTableName = AValue;
    fChanged = true;
}

void MNSql::setWhere(QString AValue) {
    if(fWhere == AValue) return;
    fWhere = AValue;
    fChanged = true;
}

void MNSql::setChanged(bool AValue) {
    fChanged = AValue;
}

QString MNSql::tableName() {
    return fTableName;
}


