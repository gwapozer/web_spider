#ifndef HTML_PARSER_H
#define HTML_PARSER_H

#include <string>
#include <iostream>

#include "SocketNet.h"
#include "LinkedList.h"

#define TOKEN_SIZE 80

using namespace std;

//Class to store Header information
class HeaderNodelink {
public:
    /* the data of this link */
    std::string data;
    /* the next link; EMPTY_LIST if this is the last link */
    HeaderNodelink *nextPtr;
    HeaderNodelink *head;
    HeaderNodelink *tail;
    
    HeaderNodelink() {
        head = 0, tail = 0, nextPtr = 0;
    };
    
    ~HeaderNodelink() {
        HeaderNodelink *currPtr = head;
        HeaderNodelink *tmpPtr;
        while (currPtr != 0) {
            tmpPtr = currPtr;
            currPtr = currPtr->nextPtr;
            delete tmpPtr;
        }
    };
    
    void AddFront(std::string StrData) {
        HeaderNodelink *NewPtr = new HeaderNodelink();
        NewPtr->data = StrData;
        if (head == 0) {
            head = tail = NewPtr;
        }
        else {
            NewPtr->nextPtr = head;
            head = NewPtr;
        }
    };
    
    void AddBack(std::string StrData) {
        HeaderNodelink *NewPtr = new HeaderNodelink();
        NewPtr->data = StrData;
        if (head == 0) {
            head = tail = NewPtr;
        }
        else {
            tail->nextPtr = NewPtr;
            tail = NewPtr;
        }
    };
    
    void Print() {
        if (head == 0) {
            cout << "The list is empty\n";
            return;
        }
        
        printf("HTTP Header:\n");
        HeaderNodelink *currPtr = head;
        while(currPtr != 0) {
            cout << currPtr->data;// << endl;
            currPtr = currPtr->nextPtr;
        }
    };
    
    int AnalyzeHTTPCode() {
        if (head == 0) {
            cout << "The list is empty\n";
            return 0;
        }
        
        char *Data;
        char *TokenPtr;
        int Httpcode;
        
        HeaderNodelink *currPtr = head;
        
        int datasize = (currPtr->data).length() + 1;
        Data = new char[datasize];
        memset(Data, 0x0, datasize);
        memcpy(Data, (currPtr->data).c_str(), datasize);
        
        int DataheaderStart = (currPtr->data).find(" ");
        
        if (((currPtr->data).substr(0, DataheaderStart) == "HTTP/1.0") || ((currPtr->data).substr(0, DataheaderStart) == "HTTP/1.1"))
        {
            TokenPtr = strtok(Data, " ");
            TokenPtr = strtok(NULL, " ");
            Httpcode = atoi(TokenPtr);
        }
        else
        {
            Httpcode = 0;
        }

        
        delete[] Data;
        
        return Httpcode;
        
    };
    
    std::string AnalyzeHTTPLocation() {
        if (head == 0) {
            cout << "The list is empty\n";
            return "/";
        }
        
        //char *Data;
        string LocationData;
        
        HeaderNodelink *currPtr = head;
        
        while(currPtr != 0) {
            //cout << currPtr->data << endl;
            ////Find Location data/////
            int datasize = (currPtr->data).length()+1;

            int DataheaderStart = (currPtr->data).find(":");
            
            if (DataheaderStart >= 0) {
                if (((currPtr->data).substr(0, DataheaderStart) == "Location") || ((currPtr->data).substr(0, DataheaderStart) == "location")) {
                    LocationData = (currPtr->data).substr(DataheaderStart+2, (datasize-DataheaderStart)-5);
                }
            }
            ////End Find Location data/////
            
            currPtr = currPtr->nextPtr;
        }
        
        return LocationData;
    }
};
/////////////////////////////END HEADER NODE LINK/////////////////////////////////////////////////


class Html_Parser {
public:
    Html_Parser();
    ~Html_Parser();
    
    void ReadFile(char*);
    
    int CheckLevelDomain(string);
    
    void RemoveSpecialChar(string&, const char);
    int CountChar(string&, string);    //Return the occurence of a char or string
    
    std::string SetURL(std::string);
    
    //HTTP function to acquire web site
    int GetHTTPWebSite(std::string, int); 
    int GetHTTPHeader(std::string, int);
    int GetHTTPData(std::string, int);

    int GetHttpCode();
    std::string GetHttpLocation();
    
    std::string GetData(){ return HTMLData; }
    std::string GetGutData(){ return HTMLGutData; }
    char *GetPTRData() { return Data; }
    
    void PrintHTTPHeader();
    
    void ParseData(char*, char*, char*, int);
    
    int ParseWebsiteStatus(char*);
    
    void ParseWebURL(char*);

    std::string ParseTokenData(std::string, char*);    //Rid of empty spaces inside a string
    
    //function to analysze URL anchor
    int GetAnchorHrefData(std::string&);
    int AnalyzeURLToken(std::string&, std::string, char*);   //Analyze a URL by start of string
    int AnalyzeURL(std::string&, std::string, char*);   //Analyze a URL by start of string
    int AnalyzeURLSite(std::string&, const char*);   //Determine if web site or web directory
    int AnalyzeURLDomainLvl(std::string&, const char*);
    int AnalyzeURLLink(std::string &URL,char* StartStr, char* EndStr);

    ////// Process function//////
    int HTTPFetchWebData(std::string);
    int GetWebSiteLevel1(std::string&);
    int GetWebSiteLevelSub(std::string&);
    int GetWebSiteFile(std::string&);
    int GetWebLink(std::string&);
    ////// End //////////////////
    
    std::string Location;
    std::string hostname;

    char* Ipname;
    char* Ipaddress;
    
    //HTTP Header data//
    HeaderNodelink *Header;
    LinkedList *LnkListData;
    
private:

    int HttpStatusCode;
    ///////////////////
    char* Data;
    std::string HTMLData;
    std::string HTMLGutData;
    
};

#endif
