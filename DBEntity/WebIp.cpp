

#include <iostream>
#include <stdio.h>
#include <string>

#include "WebIp.h"

using namespace std;

WebIp::WebIp()
{
    id = -1;
    web_site_id = -1;
    ipaddress = "";
    status = -1;
    modified_date = "";
    created_date = "";

    this->LnkWebIpList = new ObjectNodelink();
}

WebIp::~WebIp() {
    if (this->LnkWebIpList)
    {
        DeleteObjList(this->LnkWebIpList);
        delete this->LnkWebIpList;
    }
}

void WebIp::DBDataSettings(char* _hostname, char* _user, char* _password, char* _database, int _port, char* _sockpath) {

    hostname = _hostname;
    user = _user;
    password = _password;
    database = _database;
    port = _port;
    sockpath = _sockpath;

}

void WebIp::DeleteObjList(ObjectNodelink *Obj) {

    ObjectNodelink *currPtr = Obj;
    currPtr = currPtr->GetNodeHead();
    WebIp *tmpWS;
    while(currPtr != 0) {
        (currPtr->ObjPtr);
        tmpWS = (WebIp*)(currPtr->ObjPtr);
        delete tmpWS;
        currPtr = currPtr->GetNodeNextPtr();
    }
}

void WebIp::Replace(string &Data, const string &find, string &replace_with) {
    string::size_type pos = 0;

    while((pos = Data.find(find, pos)) != string::npos) {
        Data.erase(pos, find.length());
        Data.insert(pos, replace_with);
        pos+=replace_with.length();
    }
}

void WebIp::DBConnect() {
    //Connect to DB
    MySql_conn::CreateMysqlInstance(hostname, user, password, database, port, sockpath);
}

void WebIp::DBClose()
{
    MySql_conn::CloseMysqlInstance();
}

void WebIp::print() {

    cout << "Web site data" << endl;
    cout << "[ID]: "<< this->id << endl;
     cout << "[FK ID]: "<< this->web_site_id << endl;
    cout << "[Ipname]: "<< this->ipaddress << endl;
    cout << "[Status]: "<< this->status << endl;
    cout << "[Modified Date]: "<< this->modified_date << endl;
    cout << "[Created Date]: "<< this->created_date << endl <<endl;
}

void WebIp::printPtr(ObjectNodelink &Obj) {

    ObjectNodelink *currPtr = &Obj;
    currPtr = currPtr->GetNodeHead();
    WebIp *tmpWS;
    while(currPtr != 0) {
        (currPtr->ObjPtr);
        tmpWS = (WebIp*)(currPtr->ObjPtr);
        tmpWS->print();
        currPtr = currPtr->GetNodeNextPtr();
    }
}

int WebIp::GetkeyId_apps(string id) {

    MYSQL_RES *results;
    MYSQL_ROW record;
    int query_state;

    string query;

    int key_value;

    this->DBConnect();

    //Call stored procedure
    query = "CALL inter_serverGetKeyVal('" + id +"', @key)";
    mysql_query(&conn_db, query.c_str());
    //return SP buffer values
    query = "SELECT @key";
    query_state = mysql_query(&conn_db, query.c_str());

    if (query_state != 0)
    {
        cout << mysql_error(&conn_db) << endl;
        this->DBClose();
        return 0;
    }

    results = mysql_store_result(&conn_db);

    while((record = mysql_fetch_row(results))) {
        key_value = (record[0]?atoi(record[0]):-1);
        //offset_max = atoi(record[1]);
        //printf("[GetkeyId_apps]: %s\n", record[0]);
    }

    mysql_free_result(results);
    this->DBClose();
    return key_value;
}

int  WebIp::Insert() {
    int query_state;
    string query;
    char buffer[80];

    int key_val = GetkeyId_apps("web_ip_id");
    sprintf(buffer, "%i", key_val);
    string id_val = buffer;

    sprintf(buffer, "%i", this->web_site_id);
    string web_site_id_val = buffer;

    sprintf(buffer, "%i", this->status);
    string status_val = buffer;

    query = "INSERT INTO web_ip (id, web_site_id, ipaddress, status, modified_date, created_date) "
            "VALUES('" + id_val + "','" + web_site_id_val + "','" + this->ipaddress + "','" + status_val +"', NOW(), NOW())";

    this->DBConnect();

    query_state = mysql_query(&conn_db, query.c_str());

    if (query_state != 0)
    {
        cout << mysql_error(&conn_db) << endl;
        this->DBClose();
        return 0;
    }
    //printf("Querya: %s\n", query.c_str());
    this->DBClose();
    return key_val;
}

void  WebIp::Update(int id) {
    int query_state;
    string query;
    char buffer[80];

    sprintf(buffer, "%i", id);
    string id_val = buffer;

    sprintf(buffer, "%i", this->web_site_id);
    string web_site_id_val = buffer;

    sprintf(buffer, "%i", this->status);
    string status_val = buffer;

    query = "UPDATE web_ip SET "
            "web_site_id = '" + web_site_id_val +"'"
            ",ipaddress = '" + this->ipaddress + "'"
            ",status = '" + status_val +"'"
            "modified_date = NOW()"
            "WHERE id = '" + id_val + "'";

    this->DBConnect();

    query_state = mysql_query(&conn_db, query.c_str());

    if (query_state != 0)
    {
        cout << mysql_error(&conn_db) << endl;
    }
    //printf("Querya: %s\n", query.c_str());
    this->DBClose();
}

void  WebIp::Delete(int id) {
    int query_state;
    string query;
    char buffer[80];

    sprintf(buffer, "%i", id);
    string id_val = buffer;

    query = "DELETE from web_ip WHERE id = '" + id_val + "'";

    this->DBConnect();

    query_state = mysql_query(&conn_db, query.c_str());

    if (query_state != 0)
    {
        cout << mysql_error(&conn_db) << endl;
    }
    //printf("Querya: %s\n", query.c_str());
    this->DBClose();
}

int WebIp::GetWebIp(int id) {
    MYSQL_RES *results;
    MYSQL_ROW record;
    int query_state;

    string query;

    char buffer[80];

    sprintf(buffer, "%i", id);
    string id_val = buffer;

    query = "SELECT "
            "id, "
            "web_site_id,"
            "ipaddress, "
            "status, "
            "modified_date, "
            "created_date "
            "FROM web_ip WHERE id = '" + id_val + "'";

    this->DBConnect();

    query_state = mysql_query(&conn_db, query.c_str());

    if (query_state != 0)
    {
        cout << mysql_error(&conn_db) << endl;
        this->DBClose();
        return 0;
    }

    results = mysql_store_result(&conn_db);

    while((record = mysql_fetch_row(results))) {

        this->id = (record[0]?atoi(record[0]):-1);
        this->web_site_id = (record[1]?atoi(record[1]):-1);
        this->ipaddress = (record[2]?record[2]:"");
        this->status = (record[3]?atoi(record[3]):-1);
        this->modified_date = (record[4]?record[4]:"");
        this->created_date = (record[5]?record[5]:"");

        //printf("Data %s \n", record[1]);
    }

    mysql_free_result(results);
    this->DBClose();
    return 1;
}

int WebIp::GetWebIpList(string ipname) {
    MYSQL_RES *results;
    MYSQL_ROW record;
    int query_state;

    if (this->LnkWebIpList)
    {
        DeleteObjList(this->LnkWebIpList);
        delete this->LnkWebIpList;
    }

    ObjectNodelink *LnkLstData = new ObjectNodelink();
    this->LnkWebIpList =  LnkLstData;

    string query;

    query = "SELECT "
            "id, "
            "web_site_id,"
            "ipaddress, "
            "status, "
            "modified_date, "
            "created_date "
            "FROM web_ip WHERE ipaddress like '%" + ipname + "%'";

    this->DBConnect();

    query_state = mysql_query(&conn_db, query.c_str());

    if (query_state != 0)
    {
        cout << mysql_error(&conn_db) << endl;
        this->DBClose();
        return 0;
    }

    results = mysql_store_result(&conn_db);

    while((record = mysql_fetch_row(results))) {

        WebIp *tmpObj = new WebIp();

        this->LnkWebIpList->AddBack(tmpObj);

        tmpObj->id = (record[0]?atoi(record[0]):-1);
        tmpObj->web_site_id = (record[1]?atoi(record[1]):-1);
        tmpObj->ipaddress = (record[2]?record[2]:"");
        tmpObj->status = (record[3]?atoi(record[3]):-1);
        tmpObj->modified_date = (record[4]?record[4]:"");
        tmpObj->created_date = (record[5]?record[5]:"");

        //printf("Data %s \n", record[1]);
    }

    mysql_free_result(results);
    this->DBClose();
    return 1;
}
