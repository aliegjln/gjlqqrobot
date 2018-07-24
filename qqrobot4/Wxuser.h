#ifndef WXUSER_H
#define WXUSER_H

#include <other.h>
#include <Request.h>
#include <Response.h>


#include<string>
#include <locale>
#include<vector>

using namespace std;

class Wxuser
{
    string id;
    string name;

string get_uid();
int get_picture(string uuid);
map<string,string> chack_login(string uuid);
public:
    Wxuser();
    virtual ~Wxuser();
    int tologin();

};

#endif // WXUSER_H
