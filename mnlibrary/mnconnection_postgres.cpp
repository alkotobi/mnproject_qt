#include <QRegularExpression>
#include "mnconnection_postgres.h"
#include "mnarrays.h"
#include "mnexception.h"

int id_names = 0;

#include <QRegularExpression>
#include "mnconnection_postgres.h"
#include "mnarrays.h"
#include "mnexception.h"

#include <QCoreApplication>
#include <QRegularExpression>
#include <QStringList>
#include <QVariant>
#include <libpq-fe.h>
#include <QDebug>
#include <QList>


QString convertSqlitePlaceHolder(const QString &sqliteSql){
    QString result=sqliteSql;
    int placeholderCount = 1;
    int index = (int) result.indexOf('?');
    while (index != -1) {
        QString plc = "$" + QString::number(placeholderCount++);
        result.replace(index, 1, plc);
        index = (int) result.indexOf('?');
    }

    return result;
}

mnconnection_postgres::mnconnection_postgres(QString db_name, QString server, int port, QString user_name,
                                             QString password, QObject *parent)
        : mnconnection(db_name, Postgres, server, port, user_name, password) {
    db = nullptr;
}

mnconnection_postgres::~mnconnection_postgres() {
    PQfinish(db);
}

bool mnconnection_postgres::connect() {
    if (fActive) throw QString("Cant perform open operation on an active connection");
    QString str;
    str = "hostaddr=" + server + " port=" + QString::number(port) + " dbname=" + db_name + " user=" + user_name +
          " password=" + password;
    std::string s = str.toStdString();
    db = PQconnectdb(s.c_str());
    if (PQstatus(db) == CONNECTION_OK) {
        fActive = true;
        return true;
    } else {
        qCritical() << "Can't open database: " << QString(PQerrorMessage(db));
        PQfinish(db);
        return false;
    }
}

bool mnconnection_postgres::exec(QString sql) {
    std::string s = sql.toStdString();
    PGresult *res = PQexec(db, s.c_str());
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        qCritical() << s.c_str() << " failed: " << QString(PQerrorMessage(db));
        PQclear(res);
        return false;
    }
    PQclear(res);
    return true;
}



QString convertSqliteToPostgres(const QString &sqliteSql) {
    // Convert INTEGER PRIMARY KEY AUTOINCREMENT
    QString result = sqliteSql.simplified().toLower().replace("integer primary key autoincrement",
                                                              "serial primary key");
    result = result.replace("datetime", "timestamp");

    // Replace placeholders from "?" to "$n"
    int placeholderCount = 1;
    int index = (int) result.indexOf('?');
    while (index != -1) {
        QString plc = "$" + QString::number(placeholderCount++);
        result.replace(index, 1, plc);
        index = (int) result.indexOf('?');
    }

    return result;
}

void fillParamsArrays(QList<QVariant> &params, QList<QByteArray> &param_values, QList<int> &param_lengths,
                      QList<int> &param_formats) {
    for (int i = 0; i < params.count(); ++i) {
        if (params[i].isNull()) {
            param_values.append(QByteArray());
            param_lengths.append(0);
            param_formats.append(1);
            continue;
        }
        switch (params[i].typeId()) {
            case QMetaType::QString: {
                std::string ss = params[i].toString().toStdString();
                param_values.append(QByteArray(ss.c_str(), ss.length() + 1));
                param_lengths.append(ss.length() + 1);
                param_formats.append(0);
            }
                break;
            case QMetaType::Int: {
                int val = params[i].toInt();
                param_values.append(QByteArray(reinterpret_cast<const char *>(&val), sizeof(int)));
                param_lengths.append(sizeof(int));
                param_formats.append(1);
            }
                break;
            case QMetaType::Float: {
                float val = params[i].toFloat();
                param_values.append(QByteArray(reinterpret_cast<const char *>(&val), sizeof(float)));
                param_lengths.append(sizeof(float));
                param_formats.append(1);
            }
                break;
            case QMetaType::Double: {
                double val = params[i].toDouble();
                param_values.append(QByteArray(reinterpret_cast<const char *>(&val), sizeof(double)));
                param_lengths.append(sizeof(double));
                param_formats.append(1);
            }
                break;
            case QMetaType::Nullptr: {
                param_values.append(QByteArray());
                param_lengths.append(0);
                param_formats.append(1);
            }
                break;
            case QMetaType::Bool: {
                char val = params[i].toBool() ? 1 : 0;
                param_values.append(QByteArray(&val, 1));
                param_lengths.append(1);
                param_formats.append(1);
            }
                break;
            case QMetaType::Long: {
                long int val = params[i].toLongLong();
                param_values.append(QByteArray(reinterpret_cast<const char *>(&val), sizeof(long)));
                param_lengths.append(sizeof(long));
                param_formats.append(1);
            }
                break;
            case QMetaType::LongLong: {
                long long int val = params[i].toLongLong();
                param_values.append(QByteArray(reinterpret_cast<const char *>(&val), sizeof(long long)));
                param_lengths.append(sizeof(long long));
                param_formats.append(1);
            }
                break;
            case QMetaType::QByteArray: {
                QByteArray byteArray = params[i].toByteArray();
                param_values.append(byteArray);
                param_lengths.append(byteArray.size());
                param_formats.append(1);
            }
                break;
            default:
                qCritical() << "Unsupported data type" << "\n";
                throw QString("fillParamsArrays Unsupported data type");
                break;
        }
    }
}

bool do_exec(PGconn *db, PGresult *res, int sqlType, const QString &sql, QList<QVariant> &params) {
    if (res) {
        PQclear(res);
    }
    std::string s = convertSqlitePlaceHolder(sql).toStdString();
    const char *command = s.c_str();
    QList<QByteArray> param_values;
    QList<int> param_lengths;
    QList<int> param_formats;
    fillParamsArrays(params, param_values, param_lengths, param_formats);
    int nParams = (int) params.count();
    int resultFormat = 0;
    QString stmt_nm = "stmt" + QString::number(id_names++);
    std::string s0 = stmt_nm.toStdString();
    const char *stmt_name = s0.c_str();
    if (nParams > 0) {
        res = PQprepare(db, stmt_name, command, nParams, nullptr);
        if (PQresultStatus(res) != sqlType) { //PGRES_COMMAND_OK
            qCritical() << "PQprepare failed: " << QString(PQresultErrorMessage(res));
            return false;
        }
        char **paramValuePointers = new char *[nParams];
        for (int i = 0; i < nParams; ++i) {
            if (param_values[i] == QByteArray()) {
                paramValuePointers[i] = nullptr;
            } else
                paramValuePointers[i] = param_values[i].data();
        }
        PQclear(res);
        res = PQexecPrepared(db, stmt_name, nParams, paramValuePointers, param_lengths.data(), param_formats.data(),
                             resultFormat);
        delete[] paramValuePointers;
    } else {
        res = PQexec(db, s.c_str());
    }
    if (PQresultStatus(res) != sqlType) {//PGRES_COMMAND_OK
        qCritical() << "PQexecPrepared failed: " << QString(PQresultErrorMessage(res)) << "\n";
        return false;
    }
    return true;
}

bool mnconnection_postgres::exec(QString sql, QList<QVariant> params) {
    PGresult *res = nullptr;
    bool ret = do_exec(db, res, PGRES_COMMAND_OK, sql, params);
    PQclear(res);
    return ret;
}

bool mnconnection_postgres::close() {
    PQfinish(db);
    fActive = false;
    return true;
}

QString mnconnection_postgres::errorMessage() {
    return {PQerrorMessage(db)};
}

void getColumnNamesFromSelect(PGresult *res, QStringList *columns) {
    if (!columns) return;
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        throw QString("Query is not select");
    }
    int numCols = PQnfields(res);
    for (int i = 0; i < numCols; ++i) {
        columns->append(PQfname(res, i));
    }
}

bool
mnconnection_postgres::exec(QString sql, QList<QVariant> &params, QList<QStringList> *dataOut,
                            QStringList *fieldNamesOut) {
    if (!dataOut->isEmpty()) dataOut->clear();
    PGresult *res = nullptr;
    std::string s = convertSqlitePlaceHolder(sql).toStdString();
    //---------------no params---------------------
    if (params.count() == 0) {
        res = PQexec(db, s.c_str());
        if (PQresultStatus(res) != PGRES_TUPLES_OK) {
            qCritical() << "Query execution failed: " << PQerrorMessage(db) << "\n";
            PQclear(res);
            return false;
        }
        int numRows = PQntuples(res);
        int numCols = PQnfields(res);

        for (int row = 0; row < numRows; ++row) {
            QStringList rowData;
            for (int col = 0; col < numCols; ++col) {
                rowData.append(QString(PQgetvalue(res, row, col)));
            }
            dataOut->append(rowData);
        }
        if (fieldNamesOut)
            getColumnNamesFromSelect(res, fieldNamesOut);
        PQclear(res);
        return true;
    }
    //---------------with params---------------------
    QList<QByteArray> param_values;
    QList<int> param_lengths;
    QList<int> param_formats;
    fillParamsArrays(params, param_values, param_lengths, param_formats);
    char **paramValuePointers = new char *[params.count()];
    for (int i = 0; i < params.count(); ++i) {
        paramValuePointers[i] = param_values[i].data();
    }
    PQclear(res);
    res = PQprepare(db, "name", s.c_str(), params.count(), nullptr);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        qCritical() << "Query execution failed: " << PQerrorMessage(db) << "\n";
        PQclear(res);
        return false;
    }
    PQclear(res);
    res = PQexecPrepared(db, "name", params.count(), paramValuePointers, param_lengths.data(), param_formats.data(), 0);
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        qCritical() << "Execution of prepared statement failed: " << PQerrorMessage(db) << "\n";
        PQclear(res);
        return false;
    }
    int numRows = PQntuples(res);
    int numCols = PQnfields(res);

    for (int row = 0; row < numRows; ++row) {
        QStringList rowData;
        for (int col = 0; col < numCols; ++col) {
            rowData.append(QString(PQgetvalue(res, row, col)));
        }
        dataOut->append(rowData);
    }
    if (fieldNamesOut)
        getColumnNamesFromSelect(res, fieldNamesOut);
    PQclear(res);
    delete[] paramValuePointers;
    return true;
}

int mnconnection_postgres::getLastInsertedId(QString idName, QString tableName) {
    QString str = "SELECT currval('" + QString(tableName) + "_" + QString(idName) + "_seq" + "');";
    std::string s = str.toStdString();
    PGresult *res = PQexec(db, s.c_str());
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        qCritical() << "get last inserted id failed: " << PQerrorMessage(db) << "\n";
        PQclear(res);
        return -1;
    }
    char *endptr;
    long int tempId = strtol(PQgetvalue(res, 0, 0), &endptr, 10);
    if (*endptr != '\0') {
        qCritical() << "Invalid input for ID conversion." << "\n";
        PQclear(res);
        return -1;
    }
    int lastInsertedId = static_cast<int>(tempId);
    return lastInsertedId;
}


QString mnconnection_postgres::insertSql(const QString &tableName, const QString &fields) {
    int count = fields.count(',') + 1;
    QString par = "$1";

    for (int i = 2; i <= count; ++i) {
        par = par + ",$" + QString::number(i);
    }

    return "INSERT INTO " + tableName + "(" + fields + ") VALUES(" + par + ");";
}

QString mnconnection_postgres::updateSql(const QString &tableName, const QString &fields, const QString &where) {

    QStringList l = fields.split(',');
    QString s = "UPDATE " + tableName + " SET " + l[0] + "=$1";
    for (int i = 1; i < l.count() ; ++i) {
        s = s + "," + l[i] + "=$" + QString::number(i + 1);
    }
    if (where != "") {
        int placeholderCount = 1;
        QString postgresSql = where;
        int index = (int) postgresSql.indexOf('?');
        while (index != -1) {
            QString plc = "$" + QString::number(placeholderCount++);
            postgresSql.replace(index, 1, plc);
            index = (int) postgresSql.indexOf('?');
        }
        s = s + " WHERE " + postgresSql;
    }
    return s;
}

MnTableDef mnconnection_postgres::tableDef(const QString &tableName, const QStringList &fields) {
    MnTableDef table;
    table.table_name = tableName;


    // Query to get column information
    QString sql =
            "SELECT column_name, data_type, character_maximum_length FROM information_schema.columns WHERE table_name = '" +
            tableName + "'";
    PGresult *res = PQexec(db, sql.toStdString().c_str());

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        qDebug() << "Query failed: " << PQerrorMessage(db) << "\n";
        PQclear(res);
        throw MNException("Query failed: " + QString(PQerrorMessage(db)));
    }
    for (int i = 0; i < fields.size(); ++i) {
        table.fields.append(MnFieldDef());
    }
    int numRows = PQntuples(res);
    for (int i = 0; i < numRows; ++i) {
//        if (!fields.isEmpty())
//            if(!fields.contains(QString(PQgetvalue(res, i, 0)))) continue;

        QString fieldName = QString(PQgetvalue(res, i, 0));
        int ind = (int) fields.indexOf(fieldName);
        MnFieldDef *field = &table.fields[ind];
        field->field_name = fieldName;
        QString typeName = QString(PQgetvalue(res, i, 1));
        if (typeName == "integer") {
            field->field_type = INTEGER;
        } else if (typeName == "text") {
            field->field_type = TEXT;
        } else if (typeName == "real" || typeName == "double precision") {
            field->field_type = REAL;
        } else if (typeName == "varchar" || typeName == "character varying") {
            field->field_type = VARCHAR;
        } else if (typeName == "bytea") {
            field->field_type = BLOB;
        } else if (typeName == "boolean") {
            field->field_type = BOOL;
        } else if (typeName == "timestamp" || typeName == "date" || typeName == "time" ||
                   typeName == "timestamp without time zone") {
            field->field_type = DATETIME;
        }
        field->field_length = QString(PQgetvalue(res, i, 2)).toInt();
        //table.fields.push_back(field);
    }

    PQclear(res);
    return table;

}

int
mnconnection_postgres::execInsertSql(const QString &tableName, const QString &fields, const QList<QVariant> &params) {
    exec("BEGIN TRANSACTION ISOLATION LEVEL SERIALIZABLE;");
    if (!exec(insertSql(tableName, fields), params)) {
        exec("ROLLBACK;");
        return -1;
    }
    int lastId = getLastInsertedId("id", tableName);
    if (!exec("COMMIT")) {
        exec("ROLLBACK;");
        return -1;
    }
    return lastId;
}

bool mnconnection_postgres::execUpdateSql(const QString &tableName, const QString &fields, const QString &where,
                                          const QList<QVariant> &params) {
    return exec(updateSql(tableName, fields, where), params);
}

bool mnconnection_postgres::isConnected() {
    return fActive;
}

bool mnconnection_postgres::execCreateTableSql(const MnTableDef &table) {
        QString sql = "CREATE TABLE IF NOT EXISTS " + table.table_name + " (id serial primary key,";
        for (int i = 0; i < table.fields.size(); ++i) {
            const MnFieldDef& field = table.fields[i];
            if (field.field_name =="id"){
                continue;
            }
            QString fieldDefinition;
            switch (field.field_type) {
                case INTEGER:
                    fieldDefinition = field.field_name + " INTEGER";
                    break;
                case TEXT:
                    fieldDefinition = field.field_name + " TEXT";
                    break;
                case REAL:
                    fieldDefinition = field.field_name + " REAL";
                    break;
                case VARCHAR:
                    fieldDefinition = field.field_name + " VARCHAR(" + QString::number(field.field_length) + ")";
                    break;
                case BLOB:
                    fieldDefinition = field.field_name + " BYTEA";
                    break;
                case BOOL:
                    fieldDefinition = field.field_name + " BOOLEAN";
                    break;
                case DATETIME:
                    fieldDefinition = field.field_name + " TIMESTAMP";
                    break;
            }
            if (field.is_not_null) {
                fieldDefinition += " NOT NULL";
            }
            if (field.is_unique) {
                fieldDefinition += " UNIQUE";
            }
            if (!field.default_value.isEmpty()) {
                fieldDefinition += " DEFAULT " + field.default_value;
            }
            if (i < table.fields.size() - 1) {
                fieldDefinition += ",";
            }
            sql += fieldDefinition;
        }
        sql += ");";

    return exec(sql);

}



QString addColumnSqlPostgres(const MnFieldDef& fld,const  QString& table_name) {
    QString sql = "ALTER TABLE ";
    sql += table_name;
    sql += " ADD COLUMN ";
    sql += fld.field_name + " ";

    switch (fld.field_type) {
        case INTEGER:
            sql += "INTEGER";
            break;
        case TEXT:
            sql += "TEXT";
            break;
        case REAL:
            sql += "REAL";
            break;
        case VARCHAR:
            sql += "VARCHAR(" + QString::number(fld.field_length) + ")";
            break;
        case BLOB:
            sql += "BYTEA"; // PostgreSQL equivalent of BLOB
            break;
        case BOOL:
            sql += "BOOLEAN";
            break;
        case DATETIME:
            sql += "TIMESTAMP";
            break;
    }

    if (fld.is_not_null) {
        sql += " NOT NULL";
    }
    if (fld.is_unique) {
        sql += " UNIQUE";
    }
    if (!fld.default_value.isEmpty()) {
        sql += " DEFAULT " + fld.default_value;
    }

    return sql;
}

bool mnconnection_postgres::tableBackup(const QString &originalTableName, const QString newTableName) {

    // Create the backup table with the same structure and data as the original
    QString createBackupTableSql = "CREATE TABLE " + newTableName + " AS SELECT * FROM " + originalTableName + ";";
    bool created = exec(createBackupTableSql);
    if (!created) {
        qDebug() << "Failed to create backup table: " << newTableName;
        return false;
    }
    return true;
}

bool mnconnection_postgres::execUpdateTableSql(const MnTableDef &table) {
    QString checkTableExistsSql = "SELECT table_name FROM information_schema.tables WHERE table_name = '" + table.table_name + "';";
    QList<QStringList> list;
    QStringList fields;
    QList<QVariant> v = {};
    bool ret = this->exec(checkTableExistsSql, v, &list, &fields);
    if (!ret) {
        return false;
    }
    if (list.isEmpty()) {
        return execCreateTableSql(table);
    }
    QString getExistingFieldsSql = "SELECT column_name FROM information_schema.columns WHERE table_name = '" + table.table_name + "';";
    QList<QVariant> params;
    QList<QStringList> dataOut;
    QStringList fieldNamesOut;

    bool fieldsRetrieved = exec(getExistingFieldsSql, params, &dataOut, &fieldNamesOut);

    if (fieldsRetrieved) {
        QList<QString> existingFields;
        for (const QStringList& row : dataOut) {
            existingFields.append(row[0]);
        }

        // Compare with the fields in the provided table definition
        ret = true;
        exec("BEGIN;");
        for (const MnFieldDef& fieldDef : table.fields) {
            if (!existingFields.contains(fieldDef.field_name)) {
                ret = ret && exec(addColumnSqlPostgres(fieldDef, table.table_name));
            }
        }
//        bool backuped = false;
//        for (int i = 0; i < existingFields.size(); ++i) {
//            bool found = false;
//            for (const auto& f : table.fields) {
//                if (existingFields[i] == f.field_name) {
//                    found = true;
//                    continue;
//                }
//            }
//            if (!found) {
//                if (!backuped) {
//                    QDateTime currentTime = QDateTime::currentDateTime();
//                    QString backupTableName = table.table_name + "_" + currentTime.toString("yyyyMMdd_hhmmss");
//                    tableBackup(table.table_name, backupTableName);
//                    backuped = true;
//                }
//                ret = ret && exec("ALTER TABLE " + table.table_name + " DROP COLUMN " + existingFields[i]);
//            }
//        }
        if (ret) {
            return exec("COMMIT;");
        } else {
            exec("ROLLBACK;");
            return false;
        }
    }
    return true;
}