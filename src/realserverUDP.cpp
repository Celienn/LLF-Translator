#include "serverUDP.h"
Socket::Socket(int _type, int _protocol){
    descriptor = socket(AF_INET,_type,_protocol);
    localaddr.sin_family = AF_INET;

    localaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    localaddr.sin_port = htons(9999);
}


Socket::~Socket(){
    close(descriptor);
};

int Socket::Receive(){
    return recvfrom(descriptor,buffer,sizeof(buffer),0 , &from, &fromlen);
};

int Socket::Bind(){
    return bind(descriptor, (struct sockaddr*)&localaddr, sizeof(localaddr) );
};

int Socket::Send(){
    return sendto(descriptor, buffer, strlen(buffer), 0, (struct sockaddr*)&from, sizeof(from));
};

char* Socket::getbuffer(){
    return buffer;
};