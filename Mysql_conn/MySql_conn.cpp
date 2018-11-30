#include <stdio.h>
#include <iostream>
#include <string>
#include <mysql.h>

#include "MySql_conn.h"

using namespace std;

MySql_conn::MySql_conn()
{
	
}

MySql_conn::~MySql_conn()
{
	
}

void MySql_conn::CreateMysqlInstance(char* hostn, char* usr, char* pass, char* db, int portid, char* sckpath)
{
	mysql_init (&(this->conn_db));
	//if(!mysql_real_connect(&(this->conn_db), hostn, usr, pass, db, portid, sckpath, 0))
        if(!mysql_real_connect(&(this->conn_db), hostn, usr, pass, db, portid, NULL, 0))
	{
		printf(mysql_error(&(this->conn_db)));
		exit(1);
	}
	else
	{
		hostname = hostn;
		user = usr;
		password = pass;
		database = db;
		port = portid;
		sockpath = sckpath;
	}
}

void MySql_conn::CloseMysqlInstance()
{
	mysql_close(&(this->conn_db));
}

MYSQL MySql_conn::GetConnDBObj()
{
	return this->conn_db;
}

void MySql_conn::print()
{
	cout << hostname; 
	cout << user; 
	cout << password;
	cout << database;
	cout << port;
	cout << sockpath;
	cout << endl;
}
