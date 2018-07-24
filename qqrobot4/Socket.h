#ifndef SOCKET_H
#define SOCKET_H

#include "other.h"
class Socket
{
    int isock;
    SSL *ssl;
    SSL_CTX *ctx;
    struct sockaddr_in pin;
public:
    Socket(string const domain);
    ~Socket();
    void mysend(string const head);
    pair<string,string> myrecv();
};
#endif // SOCKET_H

