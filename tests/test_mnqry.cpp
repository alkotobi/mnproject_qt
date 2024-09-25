#include <cstdio>
#include "mnconnection_postgres.h"
#include "mnqry.h"
int main(int argc, char *argv[])
{
    printf("hello world\n");
    mnconnection_postgres conn("test","127.0.01",5432,"postgres","nooo",nullptr);
    conn.connect();
    MnQry qry(&conn,"SELECT * FROM nour");
    qry.open();
    qry.print();
    return 0;
}
