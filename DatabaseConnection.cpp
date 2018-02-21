#include "DatabaseConnection.h"

/*construct will initiate the Database Connection */
DatabaseConnection::DatabaseConnection()
{
	SubTest = Logger::getInstance(LOG4CPLUS_TEXT("Database Connection"));
	LOG4CPLUS_INFO(SubTest, "Connecting Sql Server");
	SqlConnHandle = NULL;
	SqlHandle = NULL;
	if (SQL_SUCCESS == SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &SqlEnvHandle))
	{
		if (SQL_SUCCESS == SQLSetEnvAttr(SqlEnvHandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0))
		{
			if (SQL_SUCCESS == SQLAllocHandle(SQL_HANDLE_DBC, SqlEnvHandle, &SqlConnHandle))
			{
				switch (SQLDriverConnect(SqlConnHandle,
					NULL,
					CONNECTION_DRIVER,
					SQL_NTS,
					RetConString,
					1024,
					NULL,
					SQL_DRIVER_NOPROMPT))
				{
				case SQL_SUCCESS:
					LOG4CPLUS_INFO(SubTest, "Successfully connected to SQL Server");
					break;

				case SQL_SUCCESS_WITH_INFO:
					LOG4CPLUS_INFO(SubTest, "Successfully connected to SQL Server");
					break;

				case SQL_INVALID_HANDLE:
					LOG4CPLUS_FATAL(SubTest, "Could not connect to SQL Server");
					break;

				case SQL_ERROR:
					LOG4CPLUS_FATAL(SubTest, "Could not connect to SQL Server");
					break;

				default:
					break;
				}
				SQLAllocHandle(SQL_HANDLE_STMT, SqlConnHandle, &SqlHandle);
			}
		}
	}
}

/*in destructor the initiated sql connection varibale are deleted*/
DatabaseConnection::~DatabaseConnection()
{
	LOG4CPLUS_INFO(SubTest, "Disconnecting Sql Server");
	SQLDisconnect(SqlConnHandle);
	SQLFreeHandle(SQL_HANDLE_STMT, SqlHandle);
	SQLFreeHandle(SQL_HANDLE_DBC, SqlConnHandle);
	SQLFreeHandle(SQL_HANDLE_ENV, SqlEnvHandle);
}

/*This method will return the connection handler for establishing a connection with the database with in the application*/
SQLHANDLE DatabaseConnection::get_connection_handler(){
	return SqlConnHandle;
}