#include "mnconnection_postgres.h"
#include "mnarrays.h"

int id_names = 0;

mnconnection_postgres::mnconnection_postgres(QString db_name, QString server,
                                             int port, QString user_name,
                                             QString password, QObject *parent)
    : mnconnection(db_name, Postgres, server, port, user_name, password, parent) {}

bool mnconnection_postgres::connect() {
    QString str;
    str = "hostaddr=" + server + " port=" + QString::number(port) +
          " dbname=" + db_name + " user=" + user_name + " password=" + password;
    std::string s = str.toStdString();
    db = PQconnectdb(s.c_str());
    if (PQstatus(db) == CONNECTION_OK) {
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
    if (PQresultStatus(res)!= PGRES_COMMAND_OK) {
        qCritical() << s.c_str() << " failed: " << QString(PQerrorMessage(db));
        PQclear(res);
        return false;
    }
    PQclear(res);
    return true;
}

bool mnconnection_postgres::exec(QString sql, QList<QVariant> params) {
    PGresult *res = nullptr;
    std::string s = sql.toStdString();
    const char *command = s.c_str();
    mncstr_array param_values;
    mnarray param_lengths;
    mnarray param_formats;
    mncstr_array_init(&param_values);
    mnarray_init(&param_lengths);
    mnarray_init(&param_formats);
    int nParams = params.count();
    int resultFormat = 0;
    QString stmt_nm = "stmt" + QString::number(id_names++);
    std::string s0 = stmt_nm.toStdString();
    const char *stmt_name = s0.c_str();
    res = PQprepare(db, stmt_name, command, nParams, nullptr);
    if (PQresultStatus(res)!= PGRES_COMMAND_OK) {
        qCritical() << "PQprepare failed: " << QString(PQresultErrorMessage(res));
        PQclear(res);
        mncstr_array_finalize(&param_values);
        mnarray_finalize(&param_lengths);
        mnarray_finalize(&param_formats);
        return false;
    }
    const char *buffer = nullptr;
    for (int i = 0; i < params.count(); ++i) {
        switch (params[i].typeId()) {
        case QMetaType::QString: {
            std::string ss = params[i].toString().toStdString();
            buffer = ss.c_str();
            mncstr_array_add_with_size(&param_values, buffer, strlen(buffer) + 1);
            mnarray_add_int(&param_lengths, strlen(buffer) + 1);
            mnarray_add_int(&param_formats, 0);
        } break;
        case QMetaType::Int: {
            int val = params[i].toInt();
            mncstr_array_add_with_size(&param_values,(char*) &val, sizeof(int));
            mnarray_add_int(&param_lengths, sizeof(int));
            mnarray_add_int(&param_formats, 1);
        } break;
        case QMetaType::Double: {
            double val = params[i].toDouble();
            mncstr_array_add_with_size(&param_values,(char*) &val, sizeof(double));
            mnarray_add_int(&param_lengths, sizeof(double));
            mnarray_add_int(&param_formats, 1);
        } break;
        case QMetaType::Nullptr: {
            int val = 0;
            mncstr_array_add_with_size(&param_values,(char*) &val, sizeof(int));
            mnarray_add_int(&param_lengths, 0);
            mnarray_add_int(&param_formats, 1);
        } break;
        case QMetaType::Bool: {
            char val = params[i].toBool()? 1 : 0;
            mncstr_array_add_with_size(&param_values, (char*)&val, 1);
            mnarray_add_int(&param_lengths, 1);
            mnarray_add_int(&param_formats, 1);
        } break;
        case QMetaType::Long: {
            long int val = params[i].toLongLong();
            mncstr_array_add_with_size(&param_values,(char*) &val, sizeof(long));
            mnarray_add_int(&param_lengths, sizeof(long));
            mnarray_add_int(&param_formats, 1);
        } break;
        case QMetaType::LongLong: {
            long long int val = params[i].toLongLong();
            mncstr_array_add_with_size(&param_values, (char*)&val, sizeof(long long));
            mnarray_add_int(&param_lengths, sizeof(long long));
            mnarray_add_int(&param_formats, 1);
        } break;
        default:
            qCritical() << "Unsupported data type";
            assert(0);
            break;
        }
    }

    char **paramValues = (char **)param_values.data;
    int *paramLengths = (int *)param_lengths.data;
    int *paramFormats = (int *)param_formats.data;

    PQclear(res);
    res = PQexecPrepared(db, stmt_name, nParams, paramValues, paramLengths,
                         paramFormats, resultFormat);
    if (PQresultStatus(res)!= PGRES_COMMAND_OK) {
        qCritical() << "PQexecPrepared failed: " << QString(PQresultErrorMessage(res));
        PQclear(res);
        mncstr_array_finalize(&param_values);
        mnarray_finalize(&param_lengths);
        mnarray_finalize(&param_formats);
        return false;
    }
    PQclear(res);
    mncstr_array_finalize(&param_values);
    mnarray_finalize(&param_lengths);
    mnarray_finalize(&param_formats);
    return true;
}

bool mnconnection_postgres::close() {
    PQfinish(db);
    return true;
}

QString mnconnection_postgres::error_message() {
    return QString(PQerrorMessage(db));
}
