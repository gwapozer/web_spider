#include "SocketNet.h"

SocketNet::SocketNet() {
}


SocketNet::~SocketNet() {
}

int SocketNet::CreateSocket(NetworkType SocketType) {
#ifdef WIN32
    WSADATA ws;
    WSAStartup(0x202, &ws);
#endif
    if(SocketType == UDP) {
        if ((Sock_Desc = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
            
            printf("[SocketNet::CreateSocket]: Error: UDP Socket Creation.\n");
            return -1;
            //exit(1);
        }
    }
    else if(SocketType == TCP) {
        if ((Sock_Desc = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            
            cerr <<"[SocketNet::CreateSocket]: Error: TCP Socket Creation.\n";
            return -1;
            //exit(1);
        }
    }
    
    return Sock_Desc;
}

void SocketNet::CloseSocket(int soc) {
#ifdef WIN32
    closesocket(soc);
#else
    close(soc);
#endif
}

int SocketNet::Connect(int Sd, sockaddr_in Server, int AttemptCnt) {
    int Connectstat = 0;
    
    bool connected = false;
    
    if (AttemptCnt == 0) {
        while(!connected) {
            if (connect(Sd, (struct sockaddr *)&Server, sizeof(Server)) < 0) {
                printf("[SocketNet::Connect]: Infinite Error connect: Still Trying To Connect\n");
                Connectstat = -1;
                
#ifdef WIN32
                Sleep(1000);
#else
                sleep(1);
#endif
                
            }
            else {
                //printf("[SocketNet::Connect]: Successfull Connection\n\n");
                Connectstat = 0;
                connected = true;
            }
        }
    }
    
    else {
        while(!connected) {
            if (connect(Sd, (struct sockaddr *)&Server, sizeof(Server)) < 0) {
                printf("[SocketNet::Connect]: Non-Infinite Error connect: Still Trying To Connect\n");
                AttemptCnt--;
                printf("Attempt Count: %i\n", AttemptCnt);
                if(AttemptCnt == 0)
                {
                    Connectstat = -1;
                    goto CleanUp;
                }
                
#ifdef WIN32
                Sleep(1000);
#else
                sleep(1);
#endif
                
            }
            else {
                //printf("[SocketNet::Connect]: Successfull Connection\n\n");
                Connectstat = 0;
                AttemptCnt = 0;
                connected = true;
            }
        }
    }
    
    CleanUp:
    return Connectstat;
}

sockaddr_in SocketNet::SetupAddress(NetworkType SocketType, char* Addr, int Port) {
    
    if(SocketType == UDP) {
        if(Addr == NULL) {
            Receiver.sin_family = AF_INET;
            Receiver.sin_port = htons(Port);
            Receiver.sin_addr.s_addr = INADDR_ANY;
        }
        else {
            struct hostent *hostInfo;
            hostInfo = gethostbyname(Addr);
            
            memset(&Receiver, 0, sizeof(Receiver));
            
            if (hostInfo == NULL) {
                printf("[SocketNet::SetupAddress]: Error Gethostbyname.\n");
                return Receiver;
            }
            
            Receiver.sin_family = AF_INET;
            Receiver.sin_port = htons(Port);
            memcpy(&Receiver.sin_addr, hostInfo->h_addr_list[0], hostInfo->h_length);
        }
    }
    
    else if(SocketType == TCP) {
        
        addr = inet_addr(Addr);
        
        struct hostent *hostInfo;
        
        if (addr == -1) {
            
            hostInfo = gethostbyname(Addr);
            memset(&Receiver, 0, sizeof(Receiver));
            
            if (hostInfo == NULL) {
                //printf("[SocketNet::SetupAddress]: Error Gethostbyname.\n");
                return Receiver;
            }
            else
            {
                IPname = hostInfo->h_name;
                IPaddress = inet_ntoa( *(struct in_addr *) hostInfo->h_addr_list[0]);
            }
        }
        
        else {
            
            hostInfo = gethostbyaddr((char *)&addr, sizeof(addr), AF_INET);
            memset(&Receiver, 0, sizeof(Receiver));
            
            if (hostInfo == NULL) {
                //printf("[SocketNet::SetupAddress]: Error gethostbyaddr.\n");
                return Receiver;
            }
            else
            {
                IPname = hostInfo->h_name;
                IPaddress = inet_ntoa( *(struct in_addr *) hostInfo->h_addr_list[0]);
                ParseIPName(IPname);
            }
        }
        
        Receiver.sin_family = AF_INET;
        memcpy(&Receiver.sin_addr, hostInfo->h_addr_list[0], hostInfo->h_length);
        Receiver.sin_port = htons(Port);
    }
    
    return Receiver;
}


int SocketNet::BindSocket(int Sd, sockaddr_in MyAddr) {
    int Status;
    Status = bind(Sd, (struct sockaddr *)&MyAddr, sizeof(struct sockaddr));
    return Status;
}

/////////////////////////////////TCP /////////////////////////////////////////////
//Time out fix for hanging recv()
int SocketNet::recvtimeout(int s, int timeout) {
    fd_set fds;
    int n;
    struct timeval tv;
    
    // set up the file descriptor set
    FD_ZERO(&fds);
    FD_SET(s, &fds);
    
    // set up the struct timeval for the timeout
    tv.tv_sec = timeout;
    tv.tv_usec = 0;
    
    // wait until timeout or data received
    n = select(s+1, &fds, NULL, NULL, &tv);
    if (n == 0) return -2; // timeout!
    if (n == -1) return -1; // error
    
    // data must be here, so do a normal recv()
    return 0;
}

std::string SocketNet::ReceiveLine(int Sd) {
    std::string ret;
    while (1) {
        char r;
        
        //this_recv = recv(m_socket,reinterpret_cast<char*>(buf + bytes_recv),remaining,0)
        switch(recv(Sd, &r, 1, 0)) {
            case 0: // not connected anymore;
                // ... but last line sent
                // might not end in \n,
                // so return ret anyway.
                return ret;
            case -1:
                return "";
        }
        
        ret += r;
        if (r == '\n')  return ret;
    }
}

int SocketNet::TCPSendLine(int Sd, std::string Buffer) {
    int PacketSentSize = -1;
    Buffer += '\n';
    PacketSentSize = send(Sd, Buffer.c_str(), Buffer.length(), 0);
    return PacketSentSize;
}

/////////////////////////////////UDP /////////////////////////////////////////////
char* SocketNet::RecvFrom(int Sd, sockaddr_in MyAddr, int &PacketSize) {
    char* Buffer = new char[MAX_MSG];
    int Addr_Len;
    Addr_Len = sizeof(struct sockaddr);
#ifdef WIN32
    PacketSize = recvfrom(Sd, Buffer, MAX_MSG, 0,(struct sockaddr *)&MyAddr,&Addr_Len);
#else
    PacketSize = recvfrom(Sd, Buffer, MAX_MSG, 0,(struct sockaddr *)&MyAddr,(socklen_t *)&Addr_Len);
#endif
    return Buffer;
}

int SocketNet::UDPSendTo(int Sd, sockaddr_in Dest, char* Buffer) {
    int PacketSentSize = -1;
    int Addr_Len;
    Addr_Len = sizeof(struct sockaddr);
    
    PacketSentSize =  sendto(Sd, Buffer, MAX_MSG, 0, (struct sockaddr *)&Dest, Addr_Len);
    
    //cout << "[Packet size]: " << PacketSentSize << endl;
    
    return PacketSentSize;
}

void SocketNet::SetPort(int p) {
    Port_Num = p;
}

int SocketNet::GetSocket() {
    return Sock_Desc;
}

sockaddr_in SocketNet::GetReceiver() {
    return Receiver;
}

void SocketNet::ParseIPName(char *ipstr) {
    int strcnt = 0;
    int dotcnt = 0;
    int dotstr = 0;
    
    char tempstr[80][500];
    
    //Count string length
    while (ipstr[strcnt] != '\0') {
        strcnt++;
    }
    
    //Iterate thru Ipname
    for (int x = 0; x < strcnt; x++) {
        //cout << ipstr[x] << endl;
        //Store name by sequence of dot found && Count number of dot in ipname
        if (ipstr[x] == '.') {
            tempstr[dotcnt][dotstr] = 0x0;
            dotcnt++;
            dotstr = 0;
            x++;
        }
        
        tempstr[dotcnt][dotstr] = ipstr[x];
        //cout << tempstr[dotcnt][dotstr] << endl;
        dotstr++;
    }
    
    tempstr[dotcnt][dotstr] = 0x0;
    
    
    string co;
    co = *(tempstr+(dotcnt-1));
    
    string catstr;
    string tmpstr;
    
    if (dotcnt >= 2 && (co == "co" || co == "ne")) {
        for (int i = (dotcnt-2); i <= dotcnt; i++) {
            //cout << "Dot" << "[" << i << "] -> " << *(tempstr+i) <<endl;
            tmpstr = *(tempstr+i);
            if (i == dotcnt-2) {
                catstr = tmpstr;
            }
            else {
                catstr = catstr + "." + tmpstr;
            }
        }
    }
    
    else {
        for (int i = (dotcnt-1); i <= dotcnt; i++) {
            //cout << "Dot" << "[" << i << "] -> " << *(tempstr+i) <<endl;
            tmpstr = *(tempstr+i);
            if (i == dotcnt-1) {
                catstr = tmpstr;
            }
            else {
                catstr = catstr + "." + tmpstr;
            }
        }
    }
    
    strcpy(ipstr, catstr.c_str());
    
    //cout << "Ipname-> " << catstr << endl;
}

char* SocketNet::GetIpname() {
    return this->IPname;
}

char* SocketNet::GetIpaddress() {
    return this->IPaddress;
}
