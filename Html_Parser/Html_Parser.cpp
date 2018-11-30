#include <stdio.h>
#include <iostream>
#include <string>

#include "Html_Parser.h"

using namespace std;

const char* iTLDLevelDomain[2] = {

    "aero",
    "root"
};

const char* gTLDLevelDomain[20] = {

    "arpa","asia","biz","cat","com","coop","edu","gov","info","int",
    "jobs","mil","mobi","museum","name","net","org","pro","tel","travel"
};

const char* ccTLDLevelDomain[249] = {

    "ac","ad","ae","af","ag","ai","al","am","an","ao",
    "aq","ar","as","at","au","aw","ax","az","ba","bb",
    "bd","be","bf","bg","bh","bj","bm","bn","bo","br",
    "bs","bt","bv","bw","by","bz","ca","cc","cd","cf",
    "cg","ch","ci","ck","cl","cm","cn","co","cr","cu",
    "cv","cx","cy","cz","de","dj","dk","dm","do","dz",
    "ec","ee","eg","er","es","et","eu","fi","fj","fk",
    "fm","fo","fr","ga","gb","gd","ge","gf","gg","gh",
    "gi","gl","gm","gn","gp","gq","gr","gs","gt","gu",
    "gw","gy","hk","hm","hn","hr","ht","hu","id","ie",
    "il","im","in","io","iq","ir","is","it","je","jm",
    "jo","jp","ke","kg","kh","ki","km","kn","kp","kr",
    "kw","ky","kz","la","lb","lc","li","lk","lr","ls",
    "lt","lu","lv","ly","ma","mc","md","me","mg","mh",
    "mk","ml","mm","mn","mp","mo","mp","mq","mr","ms",
    "mt","mu","mv","mw","mx","my","mz","na","nc","ne",
    "nf","ng","ni","nl","no","np","nr","nu","nz","om",
    "pa","pe","pf","pg","ph","pk","pl","pm","pn","pr",
    "ps","pt","pw","py","qa","re","ro","rs","ru","rw",
    "sa","sb","sc","sd","se","sg","sh","si","sj","sk",
    "sl","sm","sn","so","sr","st","su","sv","sy","sz",
    "tc","td","tf","tg","th","tj","tk","tl","tm","tn",
    "to","tp","tr","tt","tv","tw","tz","ua","ug","uk",
    "um","us","uy","uz","va","vc","ve","vg","vi","vn",
    "vu","wf","ws","ye","yt","yu","za","zm","zw"
};
 
Html_Parser::Html_Parser() {
    //Header = new HeaderNodelink();
    LnkListData = 0;
    Header = 0;
    Data = 0;
}

Html_Parser::~Html_Parser() {
    delete Header;
    
    if(Data) {
        delete[] Data;
    }
    
    delete LnkListData;
}

void Html_Parser::RemoveSpecialChar(string &str, const char chardat) {
    string tmp_buffer;
    
    for (int i = 0; str[i] != '\0';i++) {
        if (!(str[i] == chardat )) {
            tmp_buffer = tmp_buffer + *(&str[i]);
        }
    }
    str = tmp_buffer;
}

//return the occurrence of a string
int Html_Parser::CountChar(string &Data, string find) {
    string::size_type pos = 0;
    int count = 0;
    while((pos = Data.find(find, pos)) != string::npos) {
        count++;
        pos+=find.length();
    }
    return count;
}

std::string Html_Parser::SetURL(std::string URL) {
    string ParseURL;
    
    char *Data;
    char *TokenPtr;
    int datasize = 0;
    
    if (URL.find("http:") < string::npos) {
        datasize = URL.length() + 1;
        Data = new char[datasize];
        memset(Data, 0x0, datasize);
        memcpy(Data, URL.c_str(), datasize);
        
        TokenPtr = strtok(Data, "//");
        TokenPtr = strtok(NULL, "/");
        
        if (TokenPtr == NULL) {
            this->Location = " ";
        }
        
        else {
            ParseURL = TokenPtr;
            
            this->hostname = TokenPtr;
            
            //cout << "Hostname:" << this->hostname << endl;
            
            if ((TokenPtr = strtok(NULL, " ")) == NULL)
                this->Location = "/";
            
            while( TokenPtr != NULL) {
                this->Location = TokenPtr;
                this->Location = "/" + this->Location;
                
                //cout << TokenPtr << endl;
                TokenPtr = strtok(NULL, " ");
            }
        }
        
        delete[] Data;
    }
    
    
    else if  (URL.find("/") != 0) {
        URL = "http://" + URL;
        datasize = URL.length() + 1;
        Data = new char[datasize];
        memset(Data, 0x0, datasize);
        memcpy(Data, URL.c_str(), datasize);
        
        TokenPtr = strtok(Data, "//");
        TokenPtr = strtok(NULL, "/");
        
        if (TokenPtr == NULL) {
            this->Location = " ";         
        }   
        else   
        {
            ParseURL = TokenPtr;
        
            this->hostname = TokenPtr;
        
            if ((TokenPtr = strtok(NULL, " ")) == NULL)
                this->Location = "/";
        
            while( TokenPtr != NULL) {
                this->Location = TokenPtr;
                this->Location = "/" + this->Location;
            
                cout << TokenPtr << endl;
                TokenPtr = strtok(NULL, " ");
            }
        }
        delete[] Data;
    }
    
    else {
        datasize = URL.length() + 1;
        Data = new char[datasize];
        memset(Data, 0x0, datasize);
        memcpy(Data, URL.c_str(), datasize);
        
        this->Location = URL;
        ParseURL = Data;
        
        delete[] Data;
    }
    
    return ParseURL;
}

int Html_Parser::GetHTTPWebSite(string Url, int port) {
    
    int retStat = 0;
    
    int URLsize =  Url.length() + 1;
    char *URL = new char[URLsize];
    memset(URL, 0x0, URLsize);
    memcpy(URL, Url.c_str(), URLsize);
    
    //Create socket connection
    SocketNet *Net = new SocketNet();
    
    int Sd;
    int status;
    sockaddr_in Server;
    
    /*
     * if ((Sd = Net->CreateSocket(TCP)) == -1) {
     * printf("[HTMLData_Parser::GetHTTPHeader]: Could not create socket.\n");
     * retStat = -1;
     * goto  CleanUp;
     * }
     */
    
    Server = Net->SetupAddress(TCP, URL , port);
    
    if (Server.sin_port == 0) {
        //printf("[HTMLData_Parser::GetHTTPHeader]: Could not setup address.\n");
        retStat = -1;
        goto  CleanUp;
        
        //return -1;
    }
    //Server = Net->SetupAddress(TCP,"www.microsoft.com" ,port);
    /*
     * if (Net->Connect(Sd, Server, 5) == -1) {
     * printf("[HTMLData_Parser::GetHTTPHeader]: Could not connect.\n");
     * retStat =  -1;
     * }
     */
    
    if (retStat == 0) {
        this->Ipname = Net->GetIpname();
        this->Ipaddress = Net->GetIpaddress();
    }
    
    CleanUp:
    delete[] URL;
    
    Net->CloseSocket(Sd);
    delete Net;
    
    return retStat;
}

int Html_Parser::GetHTTPHeader(string Url, int port) {
    
    int retStat = 0;
    
    //Ensure we delete any previous Header Link data node
    if (this->Header != 0) {
        delete this->Header;
    }
    
    HeaderNodelink *HeaderLinkData = new HeaderNodelink();
    
    this->Header =  HeaderLinkData;
    
    int URLsize =  this->hostname.length() + 1;
    char *URL = new char[URLsize];
    memset(URL, 0x0, URLsize);
    memcpy(URL, this->hostname.c_str(), URLsize);
    
    std::string SendHeadLine;
    std::string SendhostLine;
    
    SendHeadLine = "HEAD " + this->Location  + " HTTP/1.0";
    SendhostLine = "Host: " + this->hostname;
    
    //cout << SendHeadLine << endl;
    //cout << SendhostLine << endl;
    //cout << endl;
    
    //Create socket connection
    SocketNet *Net = new SocketNet();
    
    int Sd;
    int status;
    sockaddr_in Server;
    
    if ((Sd = Net->CreateSocket(TCP)) == -1) {
        printf("[HTMLData_Parser::GetHTTPHeader]: Could not create socket.\n");
        retStat =  -1;
        goto  CleanUp;
    }
    
    Server = Net->SetupAddress(TCP, URL , port);
    
    if (Server.sin_port == 0) {
        printf("[HTMLData_Parser::GetHTTPHeader]: Could not setup address.\n");
        retStat = -1;
        goto  CleanUp;
    }
    //Server = Net->SetupAddress(TCP,"www.microsoft.com" ,port);
    if (Net->Connect(Sd, Server, 2) == -1) {
        printf("[HTMLData_Parser::GetHTTPHeader]: Could not connect.\n");
        retStat = -1;
        goto  CleanUp;
    }
    
    //Net->TCPSendLine(Sd, "HEAD /en/us/default.aspx HTTP/1.0");
    //Net->TCPSendLine(Sd, "Host: www.microsoft.com");
    
    Net->TCPSendLine(Sd, SendHeadLine);
    Net->TCPSendLine(Sd, SendhostLine);
    Net->TCPSendLine(Sd, "\n");
    
    if (!Net->recvtimeout(Sd, 10) == 0) {
        printf("[HTMLData_Parser::GetHTTPHeader]: Recv Time out Error.\n");
        retStat = -1;
        goto  CleanUp;
    }
    else {
        while (1) {
            string l = Net->ReceiveLine(Sd);
            
            if (l.empty()) break;
            HeaderLinkData->AddBack(l);
            //cout << l;
            //cout.flush();
        }
    }
    
    CleanUp:
    delete[] URL;
    
    Net->CloseSocket(Sd);
    delete Net;
    
    return retStat;
}

int Html_Parser::GetHTTPData(string Url, int port) {
    
    int retStat = 0;
    int datasize = 0;
    
    string StrData = "";
    
    if (this->Data != NULL) {
        delete[] this->Data;
    }
    
    int URLsize =  this->hostname.length() + 1;
    char *URL = new char[URLsize];
    memset(URL, 0x0, URLsize);
    memcpy(URL, this->hostname.c_str(), URLsize);
    
    std::string SendHeadLine;
    std::string SendhostLine;
    
    SendHeadLine = "GET " + this->Location  + " HTTP/1.0";
    SendhostLine = "Host: " + this->hostname;
    
    //cout << SendHeadLine << endl;
    //cout << SendhostLine << endl;
    //cout << endl;
    
    //Create socket connection
    SocketNet *Net = new SocketNet();
    
    int Sd;
    int status;
    sockaddr_in Server;
    
    if ((Sd = Net->CreateSocket(TCP)) == -1) {
        printf("[HTMLData_Parser::GetHTTPData]: Could not create socket.\n");
        retStat = -1;
        goto  CleanUp;
    }
    
    Server = Net->SetupAddress(TCP, URL , port);
    if (Server.sin_port == 0) {
        printf("[HTMLData_Parser::GetHTTPData]: Could not setup address.\n");
        retStat = -1;
        goto  CleanUp;
    }
    
    //Server = Net->SetupAddress(TCP,"www.microsoft.com" ,port);
    if (Net->Connect(Sd, Server, 2) == -1) {
        printf("[HTMLData_Parser::GetHTTPData]: Could not connect.\n");
        retStat = -1;
        goto  CleanUp;
    }
    
    //Net->TCPSendLine(Sd, "HEAD /en/us/default.aspx HTTP/1.0");
    //Net->TCPSendLine(Sd, "Host: www.microsoft.com");
    
    Net->TCPSendLine(Sd, SendHeadLine);
    Net->TCPSendLine(Sd, SendhostLine);
    Net->TCPSendLine(Sd, "\n");
    
    if (!Net->recvtimeout(Sd, 10) == 0) {
        printf("[HTMLData_Parser::GetHTTPData]: Recv Time out Error.\n");
        retStat = -1;
        goto  CleanUp;
    }
    
    else {
        while (1) {
            string l = Net->ReceiveLine(Sd);
            if (l.empty()) break;
            StrData = StrData + l;
            //cout << l;
            //cout.flush();
        }
    }
    
    datasize = StrData.length();
    
    this->Data = new char[datasize];
    memset(this->Data, 0x0, datasize);
    memcpy(this->Data, StrData.c_str(), datasize);
    
    CleanUp:
    
    delete[] URL;
    Net->CloseSocket(Sd);
    delete Net;
    
    return retStat;
}

int Html_Parser::GetHttpCode() {
    return (Header->AnalyzeHTTPCode());
}

std::string Html_Parser::GetHttpLocation() {
    return Header->AnalyzeHTTPLocation();
}

void Html_Parser::PrintHTTPHeader() {
    Header->Print();
}

void Html_Parser::ParseData(char *Data, char* DelimStart, char* DelimEnd, int Instruction) {
    
    
    //Ensure we delete any previous Header Link data node
    //if (this->LnkListData != 0)
    if(this->LnkListData) {
        //printf("Deleting list.\n");
        delete this->LnkListData;
    }
    LinkedList *LnkLstData = new LinkedList();
    this->LnkListData =  LnkLstData;
    
    int DataCounter = 0;
    //Instruction: 0 -> Get Data + Get Gutdata, 1 -> Get Data Only
    //			   2 -> Get Gutdata only
    int size = strlen(Data);
    char *DataBuffer = new char[size];
    
    memcpy(DataBuffer, Data, size);
    
    ////************** TOKEN PARSER **************////
    enum TOKEN_STATUS {HEAD, TAIL, CHECKHEAD, CHECKTAIL, DATA, GUTDATA, SUCCESS, FAIL, MEMSTORE};
    TOKEN_STATUS Tkn_stat;
    
    int dataCnt = 0;
    int StartPos = 0;
    int EndPos = 0;
    
    int DataStartPos = 0;
    int DataEndPos = 0;
    
    int GutDataStartPos = 0;
    int GutDataEndPos = 0;
    
    //char TokenHead[TOKEN_SIZE] = "<a";
    //char TokenTail[TOKEN_SIZE] = "</a";
    
    char *TokenHead = DelimStart;
    char *TokenTail = DelimEnd;
    
    int i = 0;
    char c;
    
    Tkn_stat = HEAD;
    
    while(*(DataBuffer+i) != 0) {
        c = *(DataBuffer + i);
        c = tolower(c);
        
        //Find position of Header
        (c == '<' && Tkn_stat == HEAD)? (StartPos = i), (GutDataStartPos = i):false;
        (c == '>' && Tkn_stat == HEAD)? (EndPos = i), (GutDataEndPos = i), (Tkn_stat = CHECKHEAD):false;
        
        //Find position of Tail
        (c == '<' && Tkn_stat == TAIL)? (StartPos = i):false;
        (c == '>' && Tkn_stat == TAIL)? (EndPos = i), Tkn_stat = CHECKTAIL:false;
        
        //Check tokenizer header
        if (Tkn_stat == CHECKHEAD) {
            int datasize = (EndPos - StartPos)+2;
            char *tmp1Token = new char [datasize];
            memset(tmp1Token, 0x0, datasize);
            
            bool found = false;
            dataCnt = 0;
            
            //Check for Header
            for (int j = StartPos; j <= EndPos; j++) {
                c = *(DataBuffer + j);
                c = tolower(c);
                tmp1Token[dataCnt] = c;
                dataCnt++;
                
                //Check for head data position
                if (strcmp(tmp1Token, TokenHead) == 0) {
                    DataStartPos = EndPos + 1;
                    found = true;
                }
            }
            
            //Check for data within
            if (dataCnt > (strlen(TokenHead)+1)) {
                int datasize = (EndPos - StartPos) + 2;
                char *DataToken = new char [datasize];
                memset(DataToken, 0x0, datasize);
                
                dataCnt = 0;
                for (int k = StartPos; k <= EndPos; k++) {
                    c = *(DataBuffer + k);
                    DataToken[dataCnt] = c;
                    dataCnt++;
                }
                delete[] DataToken;
            }
            
            (found) ? (Tkn_stat = TAIL): (Tkn_stat = HEAD);
            delete[] tmp1Token;
        }
        /////////////////////End of tokenizer header//////////////////////////
        
        //////////////////////Check tokenizer Tail////////////////////////////
        if (Tkn_stat == CHECKTAIL) {
            int datasize = (EndPos - StartPos)+2;
            char *tmp2Token = new char[datasize];
            memset(tmp2Token, 0x0, datasize);
            
            bool found = false;
            dataCnt = 0;
            
            for (int j = StartPos; j <= EndPos; j++) {
                c = *(DataBuffer + j);
                c = tolower(c);
                tmp2Token[dataCnt] = c;
                dataCnt++;
                
                //If pass, search for tail
                if (strcmp(tmp2Token, TokenTail) == 0) {
                    //cout << "Data Tail token: " << tmp2Token << endl;
                    DataEndPos = StartPos;
                    found = true;
                }
            }
            
            (found) ? (Tkn_stat = DATA): (Tkn_stat = TAIL);
            delete[] tmp2Token;
        }
        ////////////////////End of tokenizer tail/////////////////////////
        
        ////////////////////Parse the data///////////////////////////////
        if (Tkn_stat == DATA) {
            int datasize = (DataEndPos-DataStartPos)+1;
            char *tmp3Token = new char [datasize];
            bool DataCheck = true;
            bool flagLimiter = true;
            
            dataCnt = 0;
            memset(tmp3Token, 0x0, datasize);
            
            for (int j = DataStartPos; j <= DataEndPos; j++) {
                c = *(DataBuffer + j);
                
                //printf("%c", c);
                
                (flagLimiter) ? (DataCheck = true): false;
                (c == '<')? ((DataCheck = false), (flagLimiter = false)):false;
                (c == '>')?(flagLimiter = true):false;
                (DataCheck)? (tmp3Token[dataCnt] = c), dataCnt++:false;
            }
            
            HTMLData = tmp3Token;
            
            HTMLData = (ParseTokenData(HTMLData, "\r"));
            HTMLData = (ParseTokenData(HTMLData, "\n"));
            HTMLData = (ParseTokenData(HTMLData, "\t"));
            HTMLData = (ParseTokenData(HTMLData, " "));
            
            //cout << "[Data]-> " << HTMLData << endl;
            //Done getting data find other
            Tkn_stat = GUTDATA;
            
            delete[] tmp3Token;
        }
        //////////////END Parse Data ////////////////////////////////////
        
        ////////////////////Parse the data///////////////////////////////
        if (Tkn_stat == GUTDATA) {
            int datasize = (GutDataEndPos-GutDataStartPos)+2;
            char *tmp3Token = new char [datasize];
            bool DataCheck = true;
            bool flagLimiter = true;
            
            dataCnt = 0;
            memset(tmp3Token, 0x0, datasize);
            
            for (int j = GutDataStartPos; j <= GutDataEndPos; j++) {
                c = *(DataBuffer + j);
                
                //printf("%c", c);
                
                (flagLimiter) ? (DataCheck = true): false;
                (DataCheck)? (tmp3Token[dataCnt] = c), dataCnt++:false;
            }
            
            HTMLGutData = tmp3Token;
            
            HTMLGutData = (ParseTokenData(HTMLGutData, "\r"));
            HTMLGutData = (ParseTokenData(HTMLGutData, "\n"));
            HTMLGutData = (ParseTokenData(HTMLGutData, "\t"));
            HTMLGutData = (ParseTokenData(HTMLGutData, " "));
            
            //cout << "[GutData]-> " << HTMLGutData << endl;
            //Done getting data find other
            
            Tkn_stat = MEMSTORE;
            
            DataCounter++;
            
            delete[] tmp3Token;
        }
        
        if (Tkn_stat == MEMSTORE) {
            //Add data in special link list
            LnkLstData->InsertAtBack(HTMLData, HTMLGutData);
            
            Tkn_stat = HEAD;
        }
        //////////////END Parse Data ////////////////////////////////////
        
        i++;
    }
    
    delete[] DataBuffer;
    
    //cout << "[Total Tag count]: " << DataCounter << endl;
    //cout << endl;
}

void Html_Parser::ReadFile(char* FilePath) {
    if (this->Data != NULL) {
        delete[] this->Data;
    }
    
    FILE *Fp;
    
    if ((Fp = fopen(FilePath, "r")) == NULL) {
        printf("[HTMLData_Parser::ReadFile]: Error opening file\n");
        exit(1);
    }
    
    int file_size;
    
    fseek(Fp, 0, SEEK_END);
    file_size = ftell(Fp);
    fseek(Fp, 0, SEEK_SET);
    
    this->Data = new char[file_size];
    
    memset(this->Data, 0x0, file_size);
    
    fread(this->Data, sizeof(char), file_size, Fp);
    
    //printf("File size: %i\n", file_size);
    
    /*
     * int ch;
     * int i = 0;
     *
     * while( (ch = getc(Fp)) != EOF)
     * {
     * //printf("%c", ch);
     *(Data + i) = ch;
     * i++;
     * }
     *
     * for (int j = 0; j < file_size;j++)
     * {
     * printf("%c", *(Data + j));
     * }
     */
    
    fclose(Fp);
}

std::string Html_Parser::ParseTokenData(std::string StrData, char* Delimiter) {
    
    std::string StrBuffer;
    std::string TmpBuffer;
    
    char *Data;
    char *TokenPtr;
    
    int datasize = StrData.length() + 1;
    Data = new char[datasize];
    memset(Data, 0x0, datasize);
    memcpy(Data, StrData.c_str(), datasize);
    
    StrBuffer = "";
    
    TokenPtr = strtok(Data, Delimiter);
    
    while(TokenPtr != NULL) {
        TmpBuffer = TokenPtr;
        StrBuffer = StrBuffer + " " + TmpBuffer;
        TokenPtr = strtok(NULL, Delimiter);
    }
    
    delete[] Data;
    
    return StrBuffer;
}

int Html_Parser::HTTPFetchWebData(std::string UrlPath) {
    
    int countCode = 0;
    
    int HttpCode = 0;
    string URL;
    URL = "";
    
    SetURL(UrlPath);
    
    if (GetHTTPHeader(URL, 80) == 0) {
        
        //PrintHTTPHeader();
        HttpCode = GetHttpCode();
    }
    
    //Check for any redirection
    while (((HttpCode >= 300) && (HttpCode <400))) {
        //Redefine hostname and location
        SetURL(GetHttpLocation());
        
        GetHTTPHeader(URL, 80);
        //PrintHTTPHeader();
        
        HttpCode = GetHttpCode();
        
        countCode++;
        //Break off redirection if it is in infinite loop
        if (countCode == 7)
            HttpCode = 200;
    }
    
    
    //cout << "[HTTP code]: " << HttpCode << endl;
    
    
    //URL found
    if (HttpCode >= 200 && HttpCode < 300) {
        //cout << "URL found.\n";
        if (GetHTTPData(GetHttpLocation(), 80) == 0)
            return 0;
        else
            return -1;
    }
    
    else if (HttpCode >= 300 && HttpCode < 400) {
        //cout << "URL redirection.\n";
        if (GetHTTPData(GetHttpLocation(), 80) == 0)
            return 0;
        else
            return -1;
    }
    
    else if (HttpCode >= 400 && HttpCode < 500) {
        //cout << "Client Error.\n";
        if (GetHTTPData(GetHttpLocation(), 80) == 0)
            return 0;
        else
            return -1;
    }
    
    else if (HttpCode >= 500 && HttpCode < 600) {
        //cout << "Server Error.\n";
        if (GetHTTPData(GetHttpLocation(), 80) == 0)
            return 0;
        else
            return -1;
    }
    else {
        //printf("Cannot Parse this web site.\n");
        return -1;
    }
}


/////////////////////////////// ANCHOR ANALYSIS /////////////////////////////////////////

int Html_Parser::GetAnchorHrefData(std::string &URL)
{
    int stat = 0;
    
    string TmpStr;
    
    int stringstart = 0;
    int stringend = 0;
    int Tmp1 = 0;
    int Tmp2 = 0;
    
    if (URL.find("href") < string::npos) {
         
        stringstart = URL.find_first_of("=") + 1;
        
        if (URL.find(" ", URL.find("href")) > 0)
            Tmp1 = URL.find(" ",URL.find("href"));
        
        if (URL.find(">", URL.find("href")) > 0)
            Tmp2 = URL.find(">", URL.find("href"));
        
        if (Tmp1 == 0 && Tmp2 == 0)
            stringend = URL.length();
                    
        if (Tmp1 <= Tmp2 && Tmp1 > 0)
            stringend = Tmp1;
        else
            stringend = Tmp2;
                    
        TmpStr = URL.substr(stringstart, (stringend - stringstart));
        RemoveSpecialChar(TmpStr, '"');
        //cout << "TMPSTR->" << TmpStr << endl;
        
        URL = TmpStr;
    }
    else
        URL = URL;
    return stat;   
}

//Function will analyze and parse a data from start to found start string
int Html_Parser::AnalyzeURL(std::string &URL, std::string FindStr, char* StartStr) {
    
    int stat = 0;
    
    string TmpStartStr;
    
    string TmpStr;
    
    int stringstart = 0;
    int stringend = 0;
    
    TmpStartStr = StartStr;
    
    if (URL.find(FindStr) < string::npos) {
        
        stringstart = URL.find_first_of(StartStr) + TmpStartStr.length();
        stringend = URL.length();
        TmpStr = URL.substr(stringstart, (stringend - stringstart));
        
        URL = TmpStr;
        stat = 1;
    }
    else
        URL = URL;
    return stat;
}

//Tokenize a URL by findind a valid found string
int Html_Parser::AnalyzeURLToken(std::string &URL, std::string FindStr, char* TokStr) {
    
    int stat = 0;
    
    string ParseURL;
    
    char *Data;
    char *TokenPtr;
    int datasize = 0;
    
    //printf("[Analyzing URL]-> %s\n", URL.c_str());
    
    //if (URL.find("http:") < string::npos) {
    if (URL.find(FindStr) < string::npos) {
        datasize = URL.length() + 1;
        Data = new char[datasize];
        memset(Data, 0x0, datasize);
        memcpy(Data, URL.c_str(), datasize);
        
        //TokenPtr = strtok(Data, "//");
        TokenPtr = strtok(Data, TokStr);
        TokenPtr = strtok(NULL, "/");
        
        if (TokenPtr == NULL) {
            ParseURL = "";
        }
        
        else {
            ParseURL = TokenPtr;
        }
        
        delete[] Data;
    }
    //Return its original string
    else {

        ParseURL = URL;
        stat = -1;
    }
    
    URL = ParseURL;
    
    return stat;
}

//Function will analyze and parse a data from start to found end string
int Html_Parser::AnalyzeURLSite(std::string &URL, const char* EndStr) {
    
    int stat = 0;
    
    string TmpEndStr;
    
    string TmpStr;
    
    int stringstart = 0;
    int stringend = 0;

    TmpEndStr = EndStr;
    
    if (URL.find(TmpEndStr) < string::npos) {
        
        stringstart = 0;
        stringend = URL.find_last_of(TmpEndStr)+1;//  + TmpEndStr.length();

        TmpStr = URL.substr(stringstart, (stringend - stringstart));
        
        URL = TmpStr;
    }
    else
        URL = URL;
    return stat;
}

//Check if the URL domain level is valid eg. .com, .org etc...
int Html_Parser::AnalyzeURLDomainLvl(std::string &URL, const char* EndStr)
{
    int stat = 0;
    
    string TmpEndStr;
    
    string TmpStr;
    
    int stringstart = 0;
    int stringend = 0;

    TmpEndStr = EndStr;
    
    if (URL.find(TmpEndStr) < string::npos) {
        
        stringstart = URL.find_last_of(".") + 1;
        stringend = URL.length();

        TmpStr = URL.substr(stringstart, (stringend - stringstart));

        if (TmpStr == TmpEndStr)
        {
            stat = 1;
        }   
    }
    
    return stat;
}

int Html_Parser::AnalyzeURLLink(std::string &URL,char* StartStr, char* EndStr) {

    int stat = 0;

    string TmpStr;

    int stringstart = 0;
    int stringend = 0;

    if (URL.find(StartStr) < string::npos) {

        stringstart = URL.find(StartStr);

        TmpStr = URL.substr(stringstart, URL.length());

        stringstart = 0;
        stringend = TmpStr.find(EndStr) ;

        if(stringend <= 0)
            stringend = TmpStr.length();
        
        TmpStr = TmpStr.substr(stringstart, stringend);

        URL = TmpStr;
        stat = 1;
    }
    else
        URL = URL;
    return stat;
}

//Search the init web site
int Html_Parser::GetWebSiteLevel1(std::string &URL)
{
    int stat = 0;
    
    //Tokenize all website directories
    AnalyzeURLToken(URL, "http:", "//");
    AnalyzeURLToken(URL, "www", ".");
    
    AnalyzeURL(URL, "http:", "//");
    AnalyzeURL(URL, "www", ".");
    
    for (int i = 0; i < 20; i++)
    {
        if (AnalyzeURLDomainLvl(URL, gTLDLevelDomain[i]) == 1)
            stat = 1;
    }
    
    for (int j = 0; j < 249; j++)
    {
        if (AnalyzeURLDomainLvl(URL, ccTLDLevelDomain[j]) == 1)
            stat = 1;
    }


    bool tmpFound = false;
    string tmpSrch = "";

    for (int k = 0; k < 249; k++)
    {
        tmpSrch = "";
        tmpSrch = ccTLDLevelDomain[k];
        tmpSrch = "." + tmpSrch + ".";

        if (CountChar(URL, tmpSrch)>0)
        {
            tmpFound = true;
            while (CountChar(URL, ".") >= 3)
            AnalyzeURL(URL, "", ".");
        }
    }
    
    if (tmpFound == false)
    {
        while (CountChar(URL, ".") >= 2)
        AnalyzeURL(URL, "", ".");
    }
    
    return stat;
}

//Search the init web site
int Html_Parser::GetWebLink(std::string &URL)
{
    int stat = 0;

    stat = AnalyzeURLLink(URL, "http", " ");
    RemoveSpecialChar(URL,'"');
    RemoveSpecialChar(URL,'>');
    RemoveSpecialChar(URL,'<');

    return stat;
}

//Search the sub site 
int Html_Parser::GetWebSiteLevelSub(std::string &URL)
{
    int stat = 0;

    AnalyzeURL(URL, "http:", "//");
    AnalyzeURL(URL, "www", ".");
    AnalyzeURLSite(URL, "/");
    
    if (CountChar(URL, "/") > 1)
        stat = 1;
    
    return stat;
}

int Html_Parser::GetWebSiteFile(string &URL)
{
    int stat = 0;
    
    if ((AnalyzeURL(URL, "http:", "//") == 0 || AnalyzeURL(URL, "www", ".") == 0) && (CountChar(URL, "/") < 1)) 
    {
        URL = URL;
        stat = 1;
    }
    return stat;
}
