#ifndef SOCKET_H
#define SOCKET_H

#include <sys/socket.h>
#include <sys/types.h>
#include <cstring>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>

class Socket {
    public:
        Socket(int __type, int __protocol);
        ~Socket();
        int connect(const sockaddr *__addr, socklen_t __len);
        int connect(const char *__host, int __port);
        ssize_t send(const void *__buf, size_t __n, int __flags);
        char* receive();
        int close();
    private:
        int descriptor;
};


#endif