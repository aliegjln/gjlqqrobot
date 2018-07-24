#include "Request.h"
#include "Socket.h"
#include "Sockettp.h"
Request::Request()
{
    headers["User-Agent"]="Mozilla/5.0 (X11; Linux x86_64; rv:58.0) Gecko/20100101 Firefox/58.0";

    //headers["Accept-Language"]="zh-CN,zh;q=0.8,zh-TW;q=0.7,zh-HK;q=0.5,en-US;q=0.3,en;q=0.2";
    headers["Accept"]="*/*";
    //headers["Connection"]="keep-alive";

}
const string Request::getdomainname()//获得主机名
{
    auto n=url.find("://")+3;
    return string(url,n,url.find('/',n)-n);
}
const string Request::getpath()//获得路径
{
    return string (url,url.find('/',8));
}
const string Request::get_request_head()
{
    string Re="GET "+getpath()+" HTTP/1.1\r\n";

    headers["Host"]=getdomainname();

    for(auto i:headers)
        Re+=i.first+": "+i.second+"\r\n";
    if(!cookies.empty())
    {
        Re+="Cookie: ";
        for(auto i:cookies)
            Re+=i.first+"="+i.second+";";
        Re+="\r\n";
    }
    Re+="\r\n";
    return Re;
}
Response Request::get(const string &turl,const map<string,string> &thead,const map<string,string> &tcookie)
{
    if(turl.length()!=0)
        url=turl;
    for(auto i:thead)
        headers[i.first]=i.second;
    for(auto i:tcookie)
        cookies[i.first]=i.second;

    int flag=url.find("https");
cout<<"what is i find :"<<flag<<endl;
    if(flag>=0)
    {
cout<<"this is https >>>>>>>>>>>>>>>>>>>>>>>>>>"<<endl;
        Socket soc(getdomainname());
        soc.mysend(get_request_head());
        Response rep(soc.myrecv());
        return rep;
    }
    else
    {
cout<<"this is http >>>>>>>>>>>>>>>>>>>>>>>>>>"<<endl;
        Sockettp soc(getdomainname());
        soc.mysend(get_request_head());
        Response rep(soc.myrecv());
        return rep;
    }
}
const string Request::post_request_head()
{
    string Re="POST "+getpath()+" HTTP/1.1\r\n";

    headers["Host"]=getdomainname();

    for(auto i:headers)
        Re+=i.first+": "+i.second+"\r\n";
    if(!cookies.empty())
    {
        Re+="Cookie: ";
        for(auto i:cookies)
            Re+=i.first+"="+i.second+";";
        Re+="\r\n";
    }
    Re+="\r\n";
    Re+=data;
    return Re;
}
Response Request::post(const string &turl,const map<string,string> &thead,const map<string,string> &tcookie,string tdata)
{
    if(turl.length()!=0)
        url=turl;
    if(tdata.length()!=0)
        data=tdata;
    for(auto i:thead)
        headers[i.first]=i.second;
    for(auto i:tcookie)
        cookies[i.first]=i.second;
    Socket soc(getdomainname());
    soc.mysend(post_request_head());
    Response rep(soc.myrecv());
    return rep;
}
Request::~Request()
{
    //dtor
}
