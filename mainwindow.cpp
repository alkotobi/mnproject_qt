#include "mainwindow.h"
#include "./ui_mainwindow.h"

extern "C" {
#include <libpq-fe.h>
}
#include "mnlibrary/mnconnection_sqlite.h"
#include "mnlibrary/mnconnection_postgres.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::edited()
{
        qDebug() <<" edited 1111 hi\n";
}

void MainWindow::edited2()
{
 qDebug() <<" edited 2222 hi\n";
}

void MainWindow::on_pushButton_clicked()
{
   /* printf("libpq tutorial\n");

    // Connect to the database
    // conninfo is a string of keywords and values separated by spaces.
     char *conninfo =(char*) "dbname=test user=postgres password=nooo host=localhost port=5432";

    // Create a connection
    PGconn *conn = PQconnectdb(conninfo);

    // Check if the connection is successful
    if (PQstatus(conn) != CONNECTION_OK) {
        // If not successful, print the error message and finish the connection
        printf("Error while connecting to the database server: %s\n", PQerrorMessage(conn));
        // Finish the connection
        PQfinish(conn);

        // Exit the program
        exit(1);
    }

    // We have successfully established a connection to the database server
    printf("Connection Established\n");
    printf("Port: %s\n", PQport(conn));
    printf("Host: %s\n", PQhost(conn));
    printf("DBName: %s\n", PQdb(conn));

    // Execute a query
    char *query = (char*)"SELECT * FROM products ";

    // Submit the query and retrieve the result
    PGresult *res = PQexec(conn, query);

    // Check the status of the query result
    ExecStatusType resStatus = PQresultStatus(res);

    // Convert the status to a string and print it
    printf("Query Status: %s\n", PQresStatus(resStatus));

    // Check if the query execution was successful
    if (resStatus != PGRES_TUPLES_OK) {
        // If not successful, print the error message and finish the connection
        printf("Error while executing the query: %s\n", PQerrorMessage(conn));

        // Clear the result
        PQclear(res);

        // Finish the connection
        PQfinish(conn);

        // Exit the program
        exit(1);
    }

    // We have successfully executed the query
    printf("Query Executed Successfully\n");

    // Get the number of rows and columns in the query result
    int rows = PQntuples(res);
    int cols = PQnfields(res);
    printf("Number of rows: %d\n", rows);
    printf("Number of columns: %d\n", cols);

    // Print the column names
    for (int i = 0; i < cols; i++) {
        printf("%s\t", PQfname(res, i));
    }
    printf("\n");

    // Print all the rows and columns
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            // Print the column value
            printf("%s\t", PQgetvalue(res, i, j));
        }
        printf("\n");
    }

    // Clear the result
    PQclear(res);

    // Finish the connection
    PQfinish(conn);
*/
    mnconnection_sqlite conn("/Users/mac/Documents/qt/db.db");
   conn.connect();
   conn.exec("CREATE TABLE IF NOT EXISTS \"databases\" ("
     "\"id\" INTEGER PRIMARY KEY  AUTOINCREMENT ,"
     "\"database_name\" VARCHAR(255) ,"
     "\"version\" REAL,"
     "\"dot_pas_file_path\" VARCHAR(255) ,"
     "\"db_path\" VARCHAR(255) ,"
             "\"description\" TEXT);");
   QList<QVariant> l;
   l.append(QVariant("lolo"));
   conn.exec("insert into databases(database_name) values($1)",l);


}


void MainWindow::on_pushButton_2_clicked()
{
    mnconnection_postgres conn("test","127.0.0.1",5432,"postgres","nooo");
    conn.connect();
    conn.exec("CREATE TABLE IF NOT EXISTS employees ("
              "employee_id SERIAL PRIMARY KEY,"
              "first_name VARCHAR(50) ,"
              "last_name VARCHAR(50) ,"
              "email VARCHAR(100) UNIQUE ,"
              "hire_date DATE ,"
              "salary NUMERIC(10,2)"
              ");");
    QList<QVariant> l;
    l.append(QVariant("lolo"));
    l.append(QVariant("NONO"));
    conn.exec("insert into employees(first_name,last_name) values($1,$2)",l);
}

void edited()
{
    qDebug()<<" edited hi\n";
}

void MainWindow::on_assign_function_to_signal_clicked()
{
    connect(this->ui->lineEdit, &QLineEdit::editingFinished, &MainWindow::edited);
    connect(this->ui->lineEdit, &QLineEdit::editingFinished, &MainWindow::edited2);
}


void MainWindow::on_pushButton_3_clicked()
{
//     db_options_design_def =MnDatabaseDef(db_options_design_json);
//    MnTableDef tbl = db_options_design_def.table_by_name(databases_table_name);
//    QString tblName = tbl.table_name;
}

