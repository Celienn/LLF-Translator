#ifndef SOCKET_H
#define SOCKET_H

#include <sys/socket.h>
#include <sys/types.h>
#include <cstring>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <arpa/inet.h>

class Socket{
  public:
    Socket(int _type,int _protocol);
    ~Socket();
    int Bind();
    int Receive();
    int Send();
    char* getbuffer();
    
  private:
    char buffer[512];
    int descriptor;
    struct sockaddr_in localaddr;   
    struct sockaddr addr;
    struct sockaddr from;
    socklen_t fromlen = sizeof(from);
    
};

/*Socket test = Socket(0,0);

Socket test2(0,0);
test2.connect()
*/

#endif