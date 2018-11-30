#ifndef _SOCKETNET_H
#define	_SOCKETNET_H

//#define WIN32 1

#ifdef WIN32
#include <winsock.h>
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>

#ifdef WIN32
#else
#include <netdb.h>
#endif

using namespace std;

#define MAX_MSG 1500

#define ServerCmdPort 5700
#define ServerStatusPort 5701

/*
 * Class Usage
 * UDP RECV
 * 1)Call Create Socket with UDP
 * 2)Call SetupAddress with UDP,IP or NULL, PORT
 * 3)Call BindSocket with Socket,sockaddr
 * 4)Call RecvFrom with Socket,sockaddr sender,PacketSize
 *
 * UDP SEND
 * 1)Call Create Socket with UDP
 * 2)Call SetupAddress with UDP,IP, PORT
 * 3)  -- Do an Set Socket Options that you need
 * 4)Call SendTo with Socket, Dest sockaddr, Data to Send
 *
 * TCP RECV
 * //TODO
 *
 * TCP SEND
 * 1)Call Create Socket with TCP
 * 2)Call SetupAddress with TCP,Dest IP, PORT
 * 3)Call Connect with Socket, and Dest sockaddr
 * 4) use the standard send function
 */

enum NetworkType {UDP = 1, TCP};

class SocketNet{
public:
    SocketNet();
    ~SocketNet();
    
    //Common Functions used for Both types of Sockets
    int CreateSocket(NetworkType);
    void SetPort(int);
    int GetSocket();
    
    sockaddr_in GetReceiver();
    void CloseSocket(int);
    sockaddr_in SetupAddress(NetworkType, char*, int);
    
    int BindSocket(int, sockaddr_in);
    int Connect(int, sockaddr_in, int);
    
    int recvtimeout(int, int);
    
    //Functions used for TCP
    std::string ReceiveLine(int);
    int TCPSendLine(int, std::string);
    
    //Functions used for UDP
    char* RecvFrom(int, sockaddr_in, int &);
    int UDPSendTo(int, sockaddr_in, char*);
    
    char *GetIpname();
    char *GetIpaddress();
    void ParseIPName(char*);
    
private:
#ifdef WIN32
    SOCKET Sock_Desc;
#else
    int Sock_Desc;
#endif
    int Port_Num;
    sockaddr_in Receiver;
    sockaddr_in Sender;
    int addr;
    
    char *IPname;
    char *IPaddress;
};

#endif	/* _SOCKETNET_H */

