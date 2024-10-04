#include <QBitArray>
#include "mnconnection_sqlite.h"
#include "mnexception.h"


mnconnection_sqlite::mnconnection_sqlite(QString db_name, QObject *parent)
        : mnconnection(db_name, Sqlite) {
    db = nullptr;
}

mnconnection_sqlite::~mnconnection_sqlite()
{
    sqlite3_close(db);
}

bool mnconnection_sqlite::connect() {
    if (fActive) throw MNException("Cant perform open operation on an active connection");
    QByteArray array = db_name.toLocal8Bit();
    char *buffer = array.data();
    int rc = sqlite3_open(buffer, &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return false;
    } else {
        fActive = true;
        return true;
    }
}

bool mnconnection_sqlite::exec(QString sql) {
    char *error_message = 0;
    QByteArray array = sql.toLocal8Bit();
    const char *buffer = array.constData();
    int rc = sqlite3_exec(db, buffer, 0, 0, &error_message);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", error_message);
        sqlite3_free(error_message);
        return false;
    } else {
        return true;
    }
}

bool mnconnection_sqlite::exec(QString sql, QList<QVariant> params) {
    sqlite3_stmt *stmt = nullptr;

    QByteArray array_sql = sql.toLocal8Bit();
    const char *_sql = array_sql.constData();
    int rc = sqlite3_prepare_v2(db, _sql, -1, &stmt, nullptr);
    if (SQLITE_OK != rc) {
        fprintf(stderr, "Can't prepare insert statment %s (%i): %s\n", _sql, rc,
                sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return false;
    }
    const char *buffer;
    for (int i = 0; i < params.count(); ++i) {
        if (params[i].isNull()) {
            sqlite3_bind_null(stmt, i + 1);
            continue;
        }
        switch (params[i].typeId()) {
            case QMetaType::QString: {
                std::string ss = params[i].toString().toStdString();
                buffer = ss.c_str();
                rc = sqlite3_bind_text(stmt, i + 1, buffer, -1, SQLITE_TRANSIENT);
            }
                break;
            case QMetaType::Int:
                rc = sqlite3_bind_int(stmt, i + 1, params[i].toInt());
                break;

            case QMetaType::Double:
                rc = sqlite3_bind_double(stmt, i + 1, params[i].toDouble());
                break;
            case QMetaType::Float:
                rc = sqlite3_bind_double(stmt, i + 1, params[i].toFloat());
                break;
            case QMetaType::Nullptr:
                rc = sqlite3_bind_null(stmt, i + 1);
                break;
            case QMetaType::Bool:
                rc = sqlite3_bind_int(stmt, i + 1, params[i].toBool());
                break;
            case QMetaType::Long:
            case QMetaType::LongLong:
                rc = sqlite3_bind_int64(stmt, i + 1, params[i].toLongLong());
                break;
            case QMetaType::QByteArray: {
                QByteArray byteArray = params[i].toByteArray();
                sqlite3_bind_blob(stmt, 1, byteArray.data(), byteArray.size(), SQLITE_TRANSIENT);
            }
                break;
            default:
                qDebug()<<QMetaType(params[i].typeId())<<"\n";
                throw MNException("TYPE INHANDELED");
                break;
        }
        if (SQLITE_OK != rc) {
            fprintf(stderr, "Error binding value in sql:%s index:(%i): %s\n", _sql,
                    rc, sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            return false;
        }
    }

    rc = sqlite3_step(stmt);
    if (SQLITE_DONE != rc) {
        fprintf(stderr, "sql statement didn't return DONE (%i): %s\n", rc,sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return false;
    } else {
        sqlite3_finalize(stmt);
        return true;
    }
}

bool mnconnection_sqlite::close() {
    int rc = sqlite3_close_v2(db);
    db = nullptr;
    if (rc == SQLITE_OK) {
        fActive = false;
        return true;
    } else
        return false;
}

QString mnconnection_sqlite::errorMessage() {
    return {sqlite3_errmsg(db)};
}


bool mnconnection_sqlite::exec(QString sql, QList<QVariant> &params, QList<QStringList> *dataOut,
                               QStringList *fieldNamesOut) {
    sqlite3_stmt *stmt = nullptr;

    QByteArray array_sql = sql.toLocal8Bit();
    const char *_sql = array_sql.constData();
    int rc = sqlite3_prepare_v2(db, _sql, -1, &stmt, nullptr);
    if (SQLITE_OK != rc) {
        fprintf(stderr, "Can't prepare insert statment %s (%i): %s\n", _sql, rc,
                sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return false;
    }
    //const char *buffer= nullptr;
    for (int i = 0; i < params.count(); ++i) {
        switch (params[i].typeId()) {
            case QMetaType::QString: {
                std::string  ss = params[i].toString().toStdString();
                const char * buffer = ss.c_str();
                rc = sqlite3_bind_text(stmt, i + 1, buffer, -1, SQLITE_TRANSIENT);
            }
                break;
            case QMetaType::Int:
                rc = sqlite3_bind_int(stmt, i + 1, params[i].toInt());
                break;

            case QMetaType::Double:
                rc = sqlite3_bind_double(stmt, i + 1, params[i].toDouble());
                break;
            case QMetaType::Nullptr:
                rc = sqlite3_bind_null(stmt, i + 1);
                break;
            case QMetaType::Bool:
                rc = sqlite3_bind_int(stmt, i + 1, params[i].toBool());
                break;
            case QMetaType::Long:
            case QMetaType::LongLong:
                rc = sqlite3_bind_int64(stmt, i + 1, params[i].toLongLong());
                break;
            case QMetaType::QByteArray: {
                QByteArray byteArray = params[i].toByteArray();
                sqlite3_bind_blob(stmt, 1, byteArray.data(), byteArray.size(), SQLITE_TRANSIENT);
            }
                break;

            default:
                throw MNException("TYPE UNHANDLED");
                break;
        }
        if (SQLITE_OK != rc) {
            fprintf(stderr, "Error binding value in sql:%s index:(%i): %s\n", _sql,
                    rc, sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            return false;
        }
    }

        if (fieldNamesOut) {
            int columnCount = sqlite3_column_count(stmt);
            for (int i = 0; i < columnCount; ++i) {
                fieldNamesOut->append(QString(sqlite3_column_name(stmt, i)));
            }
        }
        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            QStringList rowData;
            int columnCount = sqlite3_column_count(stmt);
            for (int i = 0; i < columnCount; ++i) {
                const char *value = reinterpret_cast<const char *>(sqlite3_column_text(stmt, i));
                rowData.append(QString(value ? value : ""));
            }
            dataOut->append(rowData);
        }
        if (rc != SQLITE_DONE) {
            qCritical() << "Error in executing SELECT query: " << sqlite3_errmsg(db) << "\n";
            sqlite3_finalize(stmt);
            throw MNException(QString("Error in executing SELECT query: ") + QString(sqlite3_errmsg(db)));
        }
    sqlite3_finalize(stmt);
    return true;
}

int mnconnection_sqlite::getLastInsertedId(QString idName, QString tableName) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT last_insert_rowid();";
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc == SQLITE_OK) {
        rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW) {
            int lastInsertedId = sqlite3_column_int(stmt, 0);
            sqlite3_finalize(stmt);
            return lastInsertedId;
        } else {
            qCritical() << "Failed to get last inserted ID." << "\n";
        }
        sqlite3_finalize(stmt);
    } else {
        qCritical() << "Error in preparing statement: " << sqlite3_errmsg(db) << "\n";
    }
    sqlite3_finalize(stmt);
    return -1;
}


QString mnconnection_sqlite::insertSql(const QString &tableName, const QString &fields)
{
    int count = fields.count(',')+1;

    QString par ="?";

    for (int i = 2; i <= count; ++i) {
        par = par + ",?";
    }

    return "INSERT INTO "+tableName+"("+fields+") VALUES("+par+");";
}

QString mnconnection_sqlite::updateSql(const QString &tableName, const QString &fields, const QString &where)
{
    QStringList l = fields.split(',');
    QString s= "UPDATE "+tableName+" SET " + l[0]+"=?";
    for (int i = 1; i < l.count(); ++i) {
        s = s + ","+l[i]+"=?";
    }
    if (where != ""){
        s = s + " WHERE "+where;
    }
    return s;
}

mnconnection_sqlite::mnconnection_sqlite()
{
    db=nullptr;
    db_name = "";
    db_type=Provider::Sqlite;
}


MnTableDef mnconnection_sqlite::tableDef(const QString &tableName, const QStringList &fields) {
    MnTableDef table;
    table.table_name = tableName;



    // Query to get column information
    QString sql = "PRAGMA table_info(" + tableName + ")";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.toStdString().c_str(), -1, &stmt, nullptr);
    if (rc!= SQLITE_OK) {
        qDebug()<<"Error preparing statement: "<< sqlite3_errmsg(db) <<"\n";
        sqlite3_finalize(stmt);
        throw  MNException("Error preparing statement: " +QString(sqlite3_errmsg(db)));
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
//        if (!fields.isEmpty())
//            if (!fields.contains(QString(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)))))
//                continue;
        MnFieldDef field;
        field.field_name = QString(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        QString typeName = QString(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        if (typeName.contains("VARCHAR")){
            typeName = "VARCHAR";
        }
        if (typeName == "INTEGER") {
            field.field_type = INTEGER;
        } else if (typeName == "TEXT") {
            field.field_type = TEXT;
        } else if (typeName == "REAL") {
            field.field_type = REAL;
        } else if (typeName == "VARCHAR") {
            field.field_type = VARCHAR;
        } else if (typeName == "BLOB") {
            field.field_type = BLOB;
        } else if (typeName == "BOOL") {
            field.field_type = BOOL;
        } else if (typeName == "DATETIME") {
            field.field_type = DATETIME;
        }
        field.field_length = sqlite3_column_int(stmt, 3);
        table.fields.append(field);
    }

    if (rc!= SQLITE_DONE) {
        qDebug()<< "Error stepping through statement: " +QString(sqlite3_errmsg(db)) << "\n";
        sqlite3_finalize(stmt);
        throw MNException("Error stepping through statement: " +QString(sqlite3_errmsg(db)));
    }

    sqlite3_finalize(stmt);
    return table;
}

int mnconnection_sqlite::execInsertSql(const QString &tableName, const QString &fields, const QList<QVariant> &params) {
    if (!exec(insertSql(tableName,fields),params)){
        return -1;
    }
    return getLastInsertedId("id",tableName);
}

bool mnconnection_sqlite::execUpdateSql(const QString &tableName, const QString &fields, const QString &where,
                                        const QList<QVariant> &params) {
    return exec(updateSql(tableName,fields,where),params);
}

bool mnconnection_sqlite::isConnected() {
    return fActive;
}

bool mnconnection_sqlite::execCreateTableSql(const MnTableDef &table) {
    QString sql = "CREATE TABLE IF NOT EXISTS " + table.table_name + " (";
    sql += "id INTEGER PRIMARY KEY AUTOINCREMENT,";
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
                fieldDefinition = field.field_name + " BLOB";
                break;
            case BOOL:
                fieldDefinition = field.field_name + " BOOLEAN";
                break;
            case DATETIME:
                fieldDefinition = field.field_name + " DATETIME";
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


QString addColumnSql(const MnFieldDef& fld,const QString &table_name) {
    QString sql = "ALTER TABLE ";
    // Assuming you have a way to determine the table name. Replace 'table_name' with the actual table name.
    sql += table_name ;
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
            sql += "BLOB";
            break;
        case BOOL:
            sql += "BOOLEAN";
            break;
        case DATETIME:
            sql += "DATETIME";
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

bool mnconnection_sqlite::tableBackup(const QString &originalTableName, const QString newTableName) {

    // Create the backup table with the same structure and data as the original
    QString createBackupTableSql = "CREATE TABLE " + newTableName + " AS SELECT * FROM " + originalTableName + ";";
    bool created = exec(createBackupTableSql);
    if (!created) {
        qDebug() << "Failed to create backup table: " << newTableName ;
        return false;
    }
    return true;
}

bool mnconnection_sqlite::execUpdateTableSql(const MnTableDef &table) {
    QString checkTableExistsSql =
            "SELECT name FROM sqlite_master WHERE type='table' AND name='" + table.table_name + "';";
    QList<QStringList> list;
    QStringList fields;
    QList<QVariant> v = {};
    bool ret = this->exec(checkTableExistsSql, v, &list, &fields);
    if (!ret) {
        return false;
    }
    if (list.isEmpty()){
        return execCreateTableSql(table);
    }
    QString getExistingFieldsSql = "PRAGMA table_info(" + table.table_name + ");";
    QList<QVariant> params;
    QList<QStringList> dataOut;
    QStringList fieldNamesOut;

    bool fieldsRetrieved = exec(getExistingFieldsSql, params, &dataOut, &fieldNamesOut);

    if (fieldsRetrieved) {
        QList<QString> existingFields;
        for (const QStringList& row : dataOut) {
            existingFields.append(row[1]);
        }

        // Compare with the fields in the provided table definition
        ret = true;
        exec("BEGIN;");
        for (const MnFieldDef& fieldDef : table.fields) {
            if (!existingFields.contains(fieldDef.field_name)) {
                ret =ret && exec(addColumnSql(fieldDef,table.table_name));
            }
        }
//        bool backuped = false;
//        for (int i = 0; i < existingFields.size()-1; ++i) {
//            bool found = false;
//            for (const auto& f:table.fields) {
//                if (existingFields[i] == f.field_name){
//                    found = true;
//                    continue;
//                }
//            }
//            if (!found){
//                if (!backuped ) {
//                    QDateTime currentTime = QDateTime::currentDateTime();
//                    QString backupTableName = table.table_name + "_" + currentTime.toString("yyyyMMdd_hhmmss");
//                    tableBackup(table.table_name, backupTableName);
//                    backuped = true;
//                }
//              ret = ret && exec("ALTER TABLE "+table.table_name+" DROP COLUMN "+existingFields[i]) ;
//            }
//        }
        if (ret) {
            return exec("COMMIT;");
        }
        else {
            exec("ROLLBACK;");
            return false;
        }
    }
    return true;

}

