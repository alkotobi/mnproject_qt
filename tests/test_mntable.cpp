#include <cstdio>
#include <iostream>
#include "mnconnection_postgres.h"
#include "mnconnection_sqlite.h"
#include "mntable.h"
#include "mnlog.h"
#include <iostream>
#include <QCoreApplication>
#include <QString>
#include <random>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <ctime>
#include "mnview.h"

QString generateRandomAlphanumericText(int length) {
    const std::string alphanumericChars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::string result;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, alphanumericChars.length() - 1);
    for (int i = 0; i < length; ++i) {
        result += alphanumericChars[dis(gen)];
    }
    return QString::fromStdString(result);
}

QString generateRandomDateTime() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> yearDis(2020, 2029);
    std::uniform_int_distribution<> monthDis(0, 11);
    std::uniform_int_distribution<> dayDis(1, 28);
    std::uniform_int_distribution<> hourDis(0, 23);
    std::uniform_int_distribution<> minuteDis(0, 59);
    std::uniform_int_distribution<> secondDis(0, 59);

    std::time_t now = std::time(nullptr);
    std::tm *timeInfo = std::localtime(&now);

    timeInfo->tm_year = yearDis(gen);
    timeInfo->tm_mon = monthDis(gen);
    timeInfo->tm_mday = dayDis(gen);
    timeInfo->tm_hour = hourDis(gen);
    timeInfo->tm_min = minuteDis(gen);
    timeInfo->tm_sec = secondDis(gen);

    std::ostringstream oss;
    oss << std::put_time(timeInfo, "%Y-%m-%d %H:%M:%S");
    return QString::fromStdString(oss.str());
}


const QString id_field = "id";
const QString integer_field = "integer_field";
const QString text_field = "text_field";
const QString real_field = "real_field";
const QString varchar_field = "varchar_field";
const QString bool_field = "bool_field";
const QString datetime_field = "datetime_field";


bool testPq() {
    const QString sqlCreateTableLite = R"(
CREATE TABLE nour (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    integer_field INTEGER,
    text_field TEXT,
    real_field REAL,
    varchar_field VARCHAR(255),
    bool_field BOOL,
    datetime_field DATETIME
);
)";

    const QString sqlCreateTablePq = R"(
CREATE TABLE nour (
    id serial primary key,
    integer_field INTEGER,
    text_field TEXT,
    real_field REAL,
    varchar_field VARCHAR(255),
    bool_field bool,
    datetime_field timestamp
);
)";
    QString sql = convertSqliteToPostgres(sqlCreateTableLite);
    bool ret = sql ==
               "create table nour ( id serial primary key, integer_field integer, text_field text, real_field real, varchar_field varchar(255), bool_field bool, timestamp_field timestamp );";
    test(ret, "convertSqliteToPostgres");
    mnconnection_postgres conn("test", "127.0.01", 5432, "postgres", "nooo", nullptr);
    ret = conn.connect();
    ret = ret && conn.isConnected();
    test(ret, "connect to db");
    MnTable qry(&conn, "SELECT * FROM nour", {});
    ret = ret && qry.exec("DROP TABLE IF EXISTS nour");
    test(ret, "drop table");
    ret = ret && qry.exec(sqlCreateTablePq);
    test(ret, "create table");
    ret = ret && qry.open();
    ret = ret && qry.isOpen();
    test(ret, "mntable open");
    MnTableDef tbldef = qry.tableDef();
    ret = ret && tbldef.table_name == "nour";
    ret = ret && tbldef.fields.size() == 7;
    ret = ret && tbldef.fields[0].field_name == id_field;
    ret = ret && tbldef.fields[0].field_type == INTEGER;
    ret = ret && tbldef.fields[1].field_name == integer_field;
    ret = ret && tbldef.fields[1].field_type == INTEGER;
    ret = ret && tbldef.fields[2].field_name == text_field;
    ret = ret && tbldef.fields[2].field_type == TEXT;
    ret = ret && tbldef.fields[3].field_name == real_field;
    ret = ret && tbldef.fields[3].field_type == REAL;
    ret = ret && tbldef.fields[4].field_name == varchar_field;
    ret = ret && tbldef.fields[4].field_type == VARCHAR;
    ret = ret && tbldef.fields[5].field_name == bool_field;
    ret = ret && tbldef.fields[5].field_type == BOOL;
    ret = ret && tbldef.fields[6].field_name == datetime_field;
    ret = ret && tbldef.fields[6].field_type == DATETIME;


    test(ret, " MnTableDef");
    qry.printTableDef();
    std::cout << "-------------------------\n";
    for (int i = 0; i < 10; ++i) {

        std::random_device rd;
        std::mt19937 gen(rd());
        qry.append();
        //qry.setFieldValue(id_field, QString::number(gen()));
        qry.setFieldValue(integer_field, QString::number(gen()));
        qry.setFieldValue(text_field, "TEXT" + QString::number(i));
        qry.setFieldValue(real_field, QString::number(static_cast<double>(gen()) / std::mt19937::max()));
        qry.setFieldValue(varchar_field, "VARCHAR" + QString::number(i));
        qry.setFieldValue(bool_field, "1");
        char buffer[80];
        auto now = std::chrono::system_clock::now();
        std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
        std::tm *timeInfo = std::localtime(&currentTime);
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeInfo);
        qry.setFieldValue(datetime_field, QString(buffer));
        qry.post();
    }
    ret = ret && qry.recordCount() == 10;
    test(ret, "add 10 records");
    std::cout << "-------------------------\n";

    if (qry.priorFirst())
        while (qry.next()) {
            std::cout << qry.fieldByName(id_field).toStdString() << "\t";
            std::cout << qry.fieldByName(integer_field).toStdString() << "\t";
            std::cout << qry.fieldByName(text_field).toStdString() << "\t";
            std::cout << qry.fieldByName(real_field).toStdString() << "\t";
            std::cout << qry.fieldByName(varchar_field).toStdString() << "\t";
            std::cout << qry.fieldByName(bool_field).toStdString() << "\t";
            std::cout << qry.fieldByName(datetime_field).toStdString() << "\t";
            std::cout << "\n";
        }

    std::cout << "-------------------------\n";
    qry.append();
    qry.setFieldValue(text_field, "NOUREDDINE");
    qry.post();
    qry.printAll();
    MnTable tbl = qry;
    std::cout << "-------------------------\n";

    if (tbl.priorFirst())
        while (tbl.next()) {
            std::cout << tbl.fieldByName(id_field).toStdString() << "\t";
            std::cout << tbl.fieldByName(integer_field).toStdString() << "\t";
            std::cout << tbl.fieldByName(text_field).toStdString() << "\t";
            std::cout << tbl.fieldByName(real_field).toStdString() << "\t";
            std::cout << tbl.fieldByName(varchar_field).toStdString() << "\t";
            std::cout << tbl.fieldByName(bool_field).toStdString() << "\t";
            std::cout << tbl.fieldByName(datetime_field).toStdString() << "\t";
            std::cout << "\n";
        }

    std::cout << "-------------------------\n";
    qry.edit();
    qry.setFieldValue(text_field, "ALI");
    qry.post();
    std::cout <<"tbl:"<< tbl.fieldByName(text_field).toStdString() << "\n";
    std::cout <<"qry:"<< qry.fieldByName(text_field).toStdString() << "\n";
    ret = tbl.fieldByName(text_field) == "NOUREDDINE" && qry.fieldByName(text_field).toStdString()=="ALI";
    test(ret,"clone MnTable");
    MnView view = tbl.filter([&tbl]() {
        if (tbl.fieldByName(text_field) == "NOUREDDINE")
            return true;
        else
            return false;
    });
    view.printAll();
    std::cout << "-------------------------\n";
    qry.close();
    qry = MnTable(&conn,"select * from nour where text_field=?",{"ALI"});
    ret = qry.open();
    test(ret,"open MnTable with params");
    qry.printAll();
    return ret;
}



bool testLite() {
    const QString sqlCreateTableLite = R"(
CREATE TABLE nour (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    integer_field INTEGER,
    text_field TEXT,
    real_field REAL,
    varchar_field VARCHAR(255),
    bool_field BOOL,
    datetime_field DATETIME
);
)";

    mnconnection_sqlite conn("/Users/mac/Downloads/test.db");
    bool ret = conn.connect();
    ret = ret && conn.isConnected();
    test(ret, "connect to db");
    MnTable qry(&conn, "SELECT * FROM nour", {});
    ret = ret && qry.exec("DROP TABLE IF EXISTS nour");
    test(ret, "drop table");
    ret = ret && qry.exec(sqlCreateTableLite);
    test(ret, "create table");
    ret = ret && qry.open();
    ret = ret && qry.isOpen();
    test(ret, "mntable open");
    MnTableDef tbldef = qry.tableDef();
    ret = ret && tbldef.table_name == "nour";
    ret = ret && tbldef.fields.size() == 7;
    ret = ret && tbldef.fields[0].field_name == id_field;
    ret = ret && tbldef.fields[0].field_type == INTEGER;
    ret = ret && tbldef.fields[1].field_name == integer_field;
    ret = ret && tbldef.fields[1].field_type == INTEGER;
    ret = ret && tbldef.fields[2].field_name == text_field;
    ret = ret && tbldef.fields[2].field_type == TEXT;
    ret = ret && tbldef.fields[3].field_name == real_field;
    ret = ret && tbldef.fields[3].field_type == REAL;
    ret = ret && tbldef.fields[4].field_name == varchar_field;
    ret = ret && tbldef.fields[4].field_type == VARCHAR;
    ret = ret && tbldef.fields[5].field_name == bool_field;
    ret = ret && tbldef.fields[5].field_type == BOOL;
    ret = ret && tbldef.fields[6].field_name == datetime_field;
    ret = ret && tbldef.fields[6].field_type == DATETIME;


    test(ret, " MnTableDef");
    qry.printTableDef();
    std::cout << "-------------------------\n";
    for (int i = 0; i < 10; ++i) {

        std::random_device rd;
        std::mt19937 gen(rd());
        qry.append();
        //qry.setFieldValue(id_field, QString::number(gen()));
        qry.setFieldValue(integer_field, QString::number(gen()));
        qry.setFieldValue(text_field, "TEXT" + QString::number(i));
        qry.setFieldValue(real_field, QString::number(static_cast<double>(gen()) / std::mt19937::max()));
        qry.setFieldValue(varchar_field, "VARCHAR" + QString::number(i));
        qry.setFieldValue(bool_field, "1");
        char buffer[80];
        auto now = std::chrono::system_clock::now();
        std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
        std::tm *timeInfo = std::localtime(&currentTime);
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeInfo);
        qry.setFieldValue(datetime_field, QString(buffer));
        qry.post();
    }
    ret = ret && qry.recordCount() == 10;
    test(ret, "add 10 records");
    std::cout << "-------------------------\n";

    if (qry.priorFirst())
        while (qry.next()) {
            std::cout << qry.fieldByName(id_field).toStdString() << "\t";
            std::cout << qry.fieldByName(integer_field).toStdString() << "\t";
            std::cout << qry.fieldByName(text_field).toStdString() << "\t";
            std::cout << qry.fieldByName(real_field).toStdString() << "\t";
            std::cout << qry.fieldByName(varchar_field).toStdString() << "\t";
            std::cout << qry.fieldByName(bool_field).toStdString() << "\t";
            std::cout << qry.fieldByName(datetime_field).toStdString() << "\t";
            std::cout << "\n";
        }

    std::cout << "-------------------------\n";
    qry.append();
    qry.setFieldValue(text_field, "NOUREDDINE");
    qry.post();
    qry.printAll();
    return ret;
}

int main(int argc, char *argv[]) {
    printf("hello world\n");
    testPq();

    std::cout << "-------------------------\n";
    std::cout << "---------testLite--------\n";
    std::cout << "-------------------------\n";

    testLite();


    return 0;
}
