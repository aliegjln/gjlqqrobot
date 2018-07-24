#ifndef USER_H
#define USER_H

#include "other.h"
#include "Request.h"
#include "Response.h"
#include "Socket.h"
using namespace std;
class User
{
    string qname;
    string qid;
    string qianm;

    map<string,string> get_cookie_for_login();
    map<string,string>  get_login_pic(map<string,string> cookie);
    map<string,string> chack_login(map<string,string> const cookie,string & retustr);
    map<string,string> get_information_cookie(string const url,map<string,string> cookie);

    string getvfwebqq(map<string,string> tcooke);
    string getpsessionid(map<string,string> tcooke);
public:
    User();
    ~User();
    map<string,string> login();
    int somerequest(map<string,string>);
};

#endif // USER_H
