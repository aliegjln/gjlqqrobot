#ifndef SOCKETTP_H
#define SOCKETTP_H

#include "other.h"

class Sockettp
{
    int isock;
    struct sockaddr_in pin;
public:
    Sockettp(string const domain);
    ~Sockettp();
    void mysend(string const head);
    pair<string,string> myrecv();
};

#endif // SOCKETTP_H
