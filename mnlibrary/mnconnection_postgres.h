#pragma once

#include "mnconnection.h"
#include <QVariant>
#include <libpq-fe.h>

/**
 * @class mnconnection_postgres
 * @brief A class representing a connection to a PostgreSQL database.
 *
 * This class provides methods for connecting to a PostgreSQL database, executing SQL commands with and without parameters,
 * closing the connection, and retrieving error messages.
 */
class mnconnection_postgres : public mnconnection
{
private:
    PGconn *db;
public:
    /**
     * @brief Constructor for mnconnection_postgres.
     * @param db_name The name of the database.
     * @param server The server address.
     * @param port The port number.
     * @param user_name The username for the database connection.
     * @param password The password for the database connection.
     * @param parent The parent QObject (default is nullptr).
     */
    explicit mnconnection_postgres(QString db_name, QString server, int port, QString user_name, QString password, QObject *parent = nullptr);

    // mnconnection interface
public:
    /**
     * @brief Connects to the PostgreSQL database.
     * @return True if the connection is successful, false otherwise.
     */
    bool connect() override;

    /**
     * @brief Executes an SQL command without parameters.
     * @param sql The SQL command to execute.
     * @return True if the command is executed successfully, false otherwise.
     */
    bool exec(QString sql) override;

    /**
     * @brief Executes an SQL command with parameters.
     * @param sql The SQL command to execute.
     * @param params The list of parameters for the SQL command.
     * @return True if the command is executed successfully, false otherwise.
     */
    bool exec(QString sql, QList<QVariant> params) override;

    /**
     * @brief Closes the database connection.
     * @return True if the connection is closed successfully, false otherwise.
     */
    bool close() override;

    /**
     * @brief Returns the error message from the database connection.
     * @return The error message as a QString.
     */
    QString error_message() override;
};

