#include "socket.h"

Socket::Socket(int __type, int __protocol){
    descriptor = socket(AF_INET, __type, __protocol);
}

Socket::~Socket(){
    close();
}

// TODO close descriptor before opening a new one

int Socket::connect(const sockaddr *__addr, socklen_t __len){
    return ::connect(descriptor, __addr, __len);
}

int Socket::connect(const char *__host, int __port){
    hostent *toinfo = gethostbyname(__host);

    sockaddr_in to;
    to.sin_family = AF_INET;
    to.sin_port = htons(__port);
    memcpy(&to.sin_addr.s_addr, toinfo->h_addr_list[0], toinfo->h_length);

    return connect((sockaddr*)&to, sizeof(to));
}

ssize_t Socket::send(const void *__buf, size_t __n, int __flags){
    return ::send(descriptor, __buf, __n, __flags);
}

char* Socket::receive(){
    char* buffer = new char[1024];
    ssize_t size = recv(descriptor, buffer, 1024, 0);
    if (size == -1){
        delete[] buffer;
        return NULL;
    }
    buffer[size] = '\0';
    return buffer;
}

int Socket::close(){
    return ::close(descriptor);
}

