#include <cstdio>
#include "mnconnection_sqlite.h"
#include <QList>
#include <QStringList>
#include<QVariant>


int main(int argc, char *argv[])
{
    std::printf("hello world sqlite\n");
    mnconnection_sqlite lite("/Users/mac/tmp/qt_sqlite.db",nullptr);
    lite.connect();
    bool ret = lite.exec("CREATE TABLE IF NOT EXISTS nour("
              "\"id\" INTEGER PRIMARY KEY  AUTOINCREMENT,"
              "\"name\" VARCHAR(255) "
              ");",{});
    ret =ret && lite.exec("INSERT INTO nour(\"name\") VALUES(?)",{"NOUREDDINE"});
    ret =ret && lite.exec("INSERT INTO nour(\"name\") VALUES(?)",{"ALI"});
    ret =ret && lite.exec("INSERT INTO nour(\"name\") VALUES(?)",{"MOHAMED"});
    ret =ret && lite.exec("INSERT INTO nour(\"name\") VALUES(?)",{"MILOUD"});
    ret =ret && lite.exec("INSERT INTO nour(\"name\") VALUES(?)",{"KADA"});
    QList<QStringList> list;
    QStringList fields;
    QList<QVariant> v={};
    ret =ret && lite.exec(QString("SELECT * FROM nour"),v,&list,&fields);
    if(ret)
        return 0;
    else
        return 1;
}
