/* 
 * File:   WebIp.h
 * Author: root
 *
 * Created on February 7, 2009, 4:12 PM
 */

#ifndef _WEBIP_H
#define	_WEBIP_H

#include <string>

#include "MySql_conn.h"
#include "LinkedListObject.h"

using namespace std;

class WebIp : public MySql_conn {
public:
    WebIp();
    ~WebIp();

    void DBDataSettings(char*, char*, char*, char*, int, char*);
    //Trim single quote for safe insert;
    void Replace(string&, const string&, string&);
    void DBConnect();
    void DBClose();

    ObjectNodelink *LnkWebIpList;
    void DeleteObjList(ObjectNodelink*);

    //System key generator//
    int GetkeyId_apps(string);

    int Insert();
    void Update(int id);
    void Delete(int id);
    int GetWebIp(int id);

    int GetWebIpList(string ipaddress);


    void print();
    void printPtr(ObjectNodelink&);

    int id;
    int web_site_id;
    string ipaddress;
    int status;
    string modified_date;
    string created_date;

    private:
        char* hostname;
	char* user;
	char* password;
	char* database;
	int port;
	char* sockpath;
};

#endif	/* _WEBIP_H */

