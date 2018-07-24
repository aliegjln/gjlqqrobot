#include "Response.h"

Response::Response(pair<string,string> re)
{
    head=re.first;
    data=re.second;
}
Response::~Response()
{
    //dtor
}
string const Response::gethead()
{
    return head;
}
string const Response::getdata()
{
    return data;
}
map<string,string> Response::get_cookies()
{
    int lenc=0;
    map<string ,string> ss;
    while(lenc=head.find("Set-Cookie:",lenc),lenc>0)
    {
        lenc+=12;
        string k(head,lenc,head.find("=",lenc)-lenc);
        lenc+=k.size()+1;
        string v(head,lenc,head.find(";",lenc)-lenc);
        if(ss.count(k)==0||(!k.empty()&&!v.empty()))
            ss[k]=v;
    }
    for(auto i:ss)
        Print(i.first,":",i.second,"\n");

    return ss;
}

