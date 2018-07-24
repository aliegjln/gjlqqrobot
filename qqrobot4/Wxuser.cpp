#include "Wxuser.h"

Wxuser::Wxuser()
{
    //ctor
}

Wxuser::~Wxuser()
{
    //dtor
}

int Wxuser::tologin()
{
    string uid=get_uid();
    get_picture(uid);
    map<string,string> n=chack_login(uid);
    return 0;
}
string Wxuser::get_uid()
{
    Request re;
    string url="https://login.wx.qq.com/jslogin?appid=wx782c26e4c19acffb&redirect_uri=https://wx.qq.com/cgi-bin/mmwebwx-bin/webwxnewloginpage&fun=new&lang=zh_CN&_=";
    url+=get_time();
    Response rep=re.get(url);
    url=rep.getdata();
    int n=url.find("uuid = \"")+8;
    url=string(url,n,url.find("\";",n)-n);
    Print("this is fristaaaaa :",url,"\n");
    return url;
}
int  Wxuser::get_picture(string uuid)
{
    Request re;
    string url="https://login.weixin.qq.com/qrcode/";
    url+=uuid;

    map<string,string> head;
    head["Referer"]="https://wx.qq.com/";

    Response rep=re.get(url);

    ofstream outdata;
    outdata.open("picW.jpg",ios::trunc|ios::binary);
    outdata<<rep.getdata();
    outdata.close();
    //将得到的data(图片)写到文件里，并通过邮件发送；
    return 1;
}
map<string,string> Wxuser::chack_login(string uuid)
{
    string url="https://login.wx.qq.com/cgi-bin/mmwebwx-bin/login?loginicon=true&uuid=";
    url+=uuid+"&tip=0&r=1304630042&_="+get_time();

    string retustr;
    while(1)
    {
        sleep(10);//最终版本这个还可以改小点；
        Request re;
        Response rep=re.get(url);
        string ch=rep.getdata();
        if((int)ch.find("200")>0)
        {
            int f=ch.find("http://");
            retustr=string(ch,f,(int)ch.find("','",f)-f);
Print("this is aaaaaaaaaaaaaaaaaaaaaaaaaaa next:",retustr);
//            qname=string(ch,ch.rfind("', '")+4,ch.size()-6-ch.rfind("', '"));
//Print("qname:",qname,"\n");
//            f=retustr.find("uin=")+4;
//            qid=string(retustr,f,retustr.find("&service=p",f)-f);
//Print("qid:",qid,'\n');
            return rep.get_cookies();
        }
        if((int )ch.find("201")>0)
            return {};
        if((int)ch.find("408")>0)
            exit(1);
        //二维码未失效，正在认证。
    }
    return {};
}
