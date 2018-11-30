#ifndef MYSQL_CONN_H
#define MYSQL_CONN_H

#include <mysql.h>

class MySql_conn
{
	public:
		MySql_conn();
		~MySql_conn();
		void CreateMysqlInstance(char*, char*, char*, char*, int, char*);
		void CloseMysqlInstance();
		MYSQL GetConnDBObj();
		MYSQL conn_db;	
		
		void print();
	private:
		char* hostname;
		char* user;
		char* password;
		char* database;
		int port;
		char* sockpath;
};
#endif
