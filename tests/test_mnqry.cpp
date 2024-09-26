#include <cstdio>
#include <iostream>
#include "mnconnection_postgres.h"
#include "mnqry.h"
int main(int argc, char *argv[])
{
    printf("hello world\n");
    mnconnection_postgres conn("test","127.0.01",5432,"postgres","nooo",nullptr);
    conn.connect();
    MnQry qry(&conn,"SELECT * FROM nour");
    qry.open();
    qry.printTableDef();
    std::cout<< "-------------------------\n";
    qry.printAll();
    std::cout<< "-------------------------\n";
    qry.first();
    do {
        std::cout << qry.fieldByName("id")->toStdString() << "\t";
        std::cout << qry.fieldByName("name")->toStdString() << "\t";
        std::cout << "\n";
    } while (qry.next());

    return 0;
}
