#ifndef REQUEST_H
#define REQUEST_H

#include "other.h"
#include "Response.h"
using namespace std;
class Request
{
private:
    string url;//https://www.xxxx.xx/xx/xx
    map<string,string> headers;
    map<string,string> cookies;
    string data;    //存放post请求内容


    const string getpath();//获得路径

public:
    Request();
    ~Request();
    Response get(const string &turl="",const map<string,string> &thead={},const map<string,string> &tcookie={});
    Response post(const string &turl="",const map<string,string> &thead={},const map<string,string> &tcookie={},string tdata="");
    const string get_request_head();
    const string post_request_head();
    const string getdomainname();//获得主机名
};

#endif // REQUEST_H
