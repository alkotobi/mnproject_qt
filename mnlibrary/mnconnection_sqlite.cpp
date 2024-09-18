#include "mnconnection_sqlite.h"

mnconnection_sqlite::mnconnection_sqlite(QString db_name, QObject *parent)
    : mnconnection(db_name, Sqlite, parent) {}

bool mnconnection_sqlite::connect() {
    QByteArray array = db_name.toLocal8Bit();
    char *buffer = array.data();
    int rc = sqlite3_open(buffer, &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return false;
    } else {
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
    sqlite3_stmt *stmt = NULL;
    QByteArray array_sql = sql.toLocal8Bit();
    const char *_sql = array_sql.constData();
    int rc = sqlite3_prepare_v2(db, _sql, -1, &stmt, NULL);
    if (SQLITE_OK != rc) {
        fprintf(stderr, "Can't prepare insert statment %s (%i): %s\n", _sql, rc,
                sqlite3_errmsg(db));
        return false;
    }
    const char *buffer;
    for (int i = 0; i < params.count(); ++i) {
        switch (params[i].typeId()) {
        case QMetaType::QString: {
            std::string ss = params[i].toString().toStdString();
             buffer = ss.c_str();
            rc = sqlite3_bind_text(stmt, i+1, buffer, -1, SQLITE_STATIC);
        } break;
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
            rc = sqlite3_bind_int(stmt, i + 1, params[i].toBool() == true);
            break;
        case QMetaType::Long:
        case QMetaType::LongLong:
            rc = sqlite3_bind_int64(stmt, i + 1, params[i].toLongLong());
            break;

        default:
            sqlite3_finalize(stmt);
            assert(0);
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
        fprintf(stderr, "insert statement didn't return DONE (%i): %s\n", rc,
                sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return false;
    } else {
        printf("SQL EXEC completed\n\n");
        sqlite3_finalize(stmt);
        return true;
    }
}

bool mnconnection_sqlite::close() {
    int rc = sqlite3_close_v2(db);
    if (rc == SQLITE_OK) {
        return true;
    } else
        return false;
}

QString mnconnection_sqlite::error_message() {
    return QString(sqlite3_errmsg(db));
}
