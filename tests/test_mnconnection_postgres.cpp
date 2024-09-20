#include "mnconnection_postgres.h"


int main(int argc, char *argv[])
{
    std::printf("hello world postgres\n");


    mnconnection_postgres lite("test","127.0.01",5432,"postgres","nooo",nullptr);
    lite.connect();
    bool ret = lite.exec(convertSqliteToPostgres("CREATE TABLE IF NOT EXISTS nour("
                                                       "\"id\" INTEGER PRIMARY KEY  AUTOINCREMENT,"
                                                       "\"name\" VARCHAR(255) "
                                                       ");"),{});
    ret =ret && lite.exec(convertSqliteToPostgres("INSERT INTO nour(\"name\") VALUES(?)"),{"NOUREDDINE"});
    ret =ret && lite.exec(convertSqliteToPostgres("INSERT INTO nour(\"name\") VALUES(?)"),{"ALI"});
    ret =ret && lite.exec(convertSqliteToPostgres("INSERT INTO nour(\"name\") VALUES(?)"),{"MOHAMED"});
    ret =ret && lite.exec(convertSqliteToPostgres("INSERT INTO nour(\"name\") VALUES(?)"),{"MILOUD"});
    ret =ret && lite.exec(convertSqliteToPostgres("INSERT INTO nour(\"name\") VALUES(?)"),{"KADA"});
    QList<QStringList> list;
    QStringList fields;
    QList<QVariant> v={};
    ret =ret && lite.exec(QString("SELECT * FROM nour"),v,&list,&fields);
    v={"NOUREDDINE","ALI"};
    ret =ret && lite.exec(convertSqliteToPostgresRegExp("SELECT * FROM nour where \"name\"=? OR \"name\"=?" ),v,&list,&fields);
    if(ret)
        return 0;
    else
        return 1;
    //    ("test","127.0.01",5432,"postgres","nooo",nullptr);
    //    bool ret=lite.connect();
    //    ret =ret && lite.exec("CREATE TABLE IF NOT EXISTS nour("
    //                         "\"id\" SERIAL PRIMARY KEY,"
    //                         "\"name\" VARCHAR(255) "
    //                         ");",{});
    //    ret =ret && lite.exec("INSERT INTO nour(\"name\") VALUES($1)",{"NOUREDDINE"});
    //    ret =ret && lite.exec("INSERT INTO nour(\"name\") VALUES($1)",{"ALI"});
    //    ret =ret && lite.exec("INSERT INTO nour(\"name\") VALUES($1)",{"MOHAMED"});
    //    ret =ret && lite.exec("INSERT INTO nour(\"name\") VALUES($1)",{"MILOUD"});
    //    ret =ret && lite.exec("INSERT INTO nour(\"name\") VALUES($1)",{"KADA"});
    //    QList<QStringList> list;
    //    QStringList fields;
    //    QList<QVariant> v={};
    //    ret =ret && lite.exec(QString("SELECT * FROM nour"),v,&list,&fields);
    //    v={"NOUREDDINE"};
    //    ret =ret && lite.exec(QString("SELECT * FROM nour where \"name\"=$1"),v,&list,&fields);
    //    if(ret)
    //        return 0;
    //    else
    //        return 1;
    return 0;
}
