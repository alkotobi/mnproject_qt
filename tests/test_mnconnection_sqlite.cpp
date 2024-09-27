#include "../mnlibrary/mnconnection_sqlite.h"
#include "../mnlibrary/mnlog.h"

int main(int argc, char *argv[]) {
    std::printf("hello world SQLite\n");

    bool ret;
    mnconnection_sqlite conn("/Users/mac/Downloads/test.db");
    ret =!conn.isConnected();
    test(ret, "mnconnection_sqlite created");
    conn.connect();
    conn.exec("PRAGMA locking_mode = NORMAL");
    ret = ret && conn.isConnected();
    test(ret, "mnconnection_sqlite connected");

    ret = ret && conn.exec("CREATE TABLE IF NOT EXISTS nour("
                           "\"id\" INTEGER PRIMARY KEY AUTOINCREMENT,"
                           "\"name\" VARCHAR(255)"
                           ");", {});
    test(ret, "conn.exec CREATE TABLE");

    ret = ret && conn.exec("INSERT INTO nour(\"name\") VALUES('NOUREDDINE');", {});
    ret = ret && conn.exec("INSERT INTO nour(\"name\") VALUES('ALI');", {});
    ret = ret && conn.exec("INSERT INTO nour(\"name\") VALUES('MOHAMED');", {});
    ret = ret && conn.exec("INSERT INTO nour(\"name\") VALUES('MILOUD');", {});
    ret = ret && conn.exec("INSERT INTO nour(\"name\") VALUES('KADA');", {});
    test(ret, "conn.exec insert data");

    QList<QStringList> list;
    QStringList fields;
    QList<QVariant> v = {};
    ret = ret && conn.exec("SELECT * FROM nour", v, &list, &fields);
    test(ret, "conn.exec SELECT * FROM");
    ret = ret && fields.size() == 2;
    ret = ret && fields[0] == "id";
    ret = ret && fields[1] == "name";
    test(ret, "columns retreived");
    ret = ret && conn.exec("SELECT * FROM nour", v, &list, &fields);
    fields.clear();
    list.clear();
    v = {"NOUREDDINE", "ALI"};
    ret = ret && conn.exec("SELECT DISTINCT name FROM nour WHERE name=? OR name=?", v, &list, &fields);
    ret = ret && list.size() == 2;
    QString str =list[0][0];
    str = list[1][0];
    ret = ret && list[0][0] == "NOUREDDINE";
    ret = ret && list[1][0] == "ALI";
    test(ret, "data retreived");

    if (ret) {
        return 0;
    } else {
        return 1;
    }
}