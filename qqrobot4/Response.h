#ifndef RESPONSE_H
#define RESPONSE_H

#include "other.h"

class Response
{
    string head;
    string data;
    map<string,string> cookies;
public:
    Response(pair<string,string>);
    ~Response();
    string const gethead();
    string const getdata();
    map<string,string> get_cookies();


};

#endif // RESPONSE_H
