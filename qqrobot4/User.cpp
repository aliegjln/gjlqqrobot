#include "User.h"

User::User()
{
    //ctor
}

User::~User()
{
    //dtor
}
map<string,string> User::get_cookie_for_login()
{
    Request re;
    Response rep=re.get("https://xui.ptlogin2.qq.com/cgi-bin/xlogin?daid=164&target=self&style=40&pt_disable_pwd=1&mibao_css=m_webqq&appid=501004106&enable_qlogin=0&no_verifyimg=1&s_url=http://web2.qq.com/proxy.html&f_url=loginerroralert&strong_login=1&login_state=10&t=20131024001");
    map<string,string> ss = rep.get_cookies();
    return ss;
}
map<string,string> User::get_login_pic(map<string,string> cookie)
{
    Request re;
    string url="https://ssl.ptlogin2.qq.com/ptqrshow?appid=501004106&e=2&l=M&s=3&d=72&v=4&t=0.05042586385144865&daid=164&pt_3rd_aid=0";
    map<string,string> head;
    head["Referer"]="https://xui.ptlogin2.qq.com/cgi-bin/xlogin?daid=164&target=self&style=40&pt_disable_pwd=1&mibao_css=m_webqq&appid=501004106&enable_qlogin=0&no_verifyimg=1&s_url=http%3A%2F%2Fweb2.qq.com%2Fproxy.html&f_url=loginerroralert&strong_login=1&login_state=10&t=20131024001";
    Response rep=re.get(url,head,cookie);

    ofstream outdata;
    outdata.open("pic.png",ios::trunc);
    outdata<<rep.getdata();
    outdata.close();
    //将得到的data(图片)写到文件里，并通过邮件发送；
    head.clear();
    head=rep.get_cookies();
    return head;
}
map<string,string> User::get_information_cookie(string const url,map<string,string> cookie)
{
    Request re;
    Response rep=re.get(url,{},cookie);
    return rep.get_cookies();
}
map<string,string> User::chack_login(map<string,string>  cookie,string & retustr)
{
    string url="https://ssl.ptlogin2.qq.com/ptqrlogin?u1=http://web2.qq.com/proxy.html&ptqrtoken=";
    url+=has33(cookie["qrsig"])+"&ptredirect=0&h=1&t=1&g=1&from_ui=1&ptlang=2052&action=0-0-1530105264932&js_ver=10275&js_type=1&login_sig=";
    url+=cookie["pt_login_sig"]+"&pt_uistyle=40&aid=501004106&daid=164&mibao_css=m_webqq&";

    map<string,string> head;
    head["Referer"]="https://xui.ptlogin2.qq.com/cgi-bin/xlogin?daid=164&target=self&style=40&pt_disable_pwd=1&mibao_css=m_webqq&appid=501004106&enable_qlogin=0&no_verifyimg=1&s_url=http%3A%2F%2Fweb2.qq.com%2Fproxy.html&f_url=loginerroralert&strong_login=1&login_state=10&t=20131024001";
    head["Accept-Language"]="zh-CN,zh;q=0.8,zh-TW;q=0.7,zh-HK;q=0.5,en-US;q=0.3,en;q=0.2";
    head["Accept-Encoding"]="gzip, deflate";
    head["Connection"]="keep-alive";

    map<string,string> nextciikie;
    while(1)
    {
        sleep(10);//最终版本这个还可以改小点；
        Request re;
        Response rep=re.get(url,head,cookie);
        string ch=rep.getdata();
        if((int)ch.find("成功")>0)
        {
            int f=ch.find("http://");
            retustr=string(ch,f,(int)ch.find("','",f)-f);
Print("this is aaaaaaaaaaaaaaaaaaaaaaaaaaa next:",retustr);
            qname=string(ch,ch.rfind("', '")+4,ch.size()-6-ch.rfind("', '"));
Print("qname:",qname,"\n");
            f=retustr.find("uin=")+4;
            qid=string(retustr,f,retustr.find("&service=p",f)-f);
Print("qid:",qid,'\n');

            return rep.get_cookies();
        }
        if((int )ch.find("二维码已失效")>0)
            return {};
        if((int)ch.find("失败")>0)
            exit(1);
        //二维码未失效，正在认证。
    }
    return {};
}
map<string,string> User::login()
{
    map<string,string> login_cookie=get_cookie_for_login();
    login_cookie.erase("pt_user_id");
    login_cookie.erase("ptui_identifier");
    login_cookie["pgv_pvi"]="5474558976";//这两个cookie随便搞一个就行
    login_cookie["pgv_si"]="s9296796672";

    int n=5;//循环获取chak的次数，最后可以文件配置
    while(n--)
    {
        string next;
        login_cookie["qrsig"]=get_login_pic(login_cookie)["qrsig"];
        map<string,string> nextcookie=chack_login(login_cookie,next);

        if((int)next.size()>10)//成功
        {
            map<string,string> funk;
            funk["pgv_pvi"]=login_cookie["pgv_pvi"];
            funk["pgv_si"]=login_cookie["pgv_si"];
            funk["p_uin"]="o"+qid;
            funk["pt2gguin"]=funk["p_uin"];
            funk["ptcz"]=nextcookie["ptcz"];
            funk["ptisp"]=nextcookie["ptisp"];
            funk["RK"]=nextcookie["RK"];
            funk["uin"]="o"+qid;
            login_cookie.clear();

            login_cookie=get_information_cookie(next,nextcookie);

            funk["p_skey"]=login_cookie["p_skey"];
            funk["pt4_token"]=login_cookie["pt4_token"];
            funk["skey"]=login_cookie["skey"];

            return funk;
        }
        //if()//图片过期，重新获取
    }
    exit(1);//获取规定次数后还没扫码就停止；
}
//////////////////以上是登录成功//////////////////////
string User::getvfwebqq(map<string,string> tcooke)
{
    string url="http://s.web2.qq.com/api/getvfwebqq?ptwebqq=&clientid=53999199&psessionid=&t=1530449475779";
    map<string ,string > thead;
    thead["Referer"]="http://s.web2.qq.com/proxy.html?v=20130916001&callback=1&id=1";
    Request re;
    Response rep=re.get(url,thead,tcooke);

    url=rep.getdata();
    int n=url.find("vfwebqq");
    n+=10;
    url=string(url,n,url.find("\"}}",n)-n);
cout<<url<<endl;
    return url;
}
string User::getpsessionid(map<string,string> tcooke)
{
    string url="http://d1.web2.qq.com/channel/login2";
    string tdata="r=%7B%22ptwebqq%22%3A%22%22%2C%22clientid%22%3A53999199%2C%22psessionid%22%3A%22%22%2C%22status%22%3A%22online%22%7D";
    map<string ,string > thead;
    thead["Referer"]="http://d.web2.qq.com/proxy.html?v=20151105001&callback=1&id=2";
    thead["Content-Length"]=to_string (tdata.size());
    thead["Content-Type"]="application/x-www-form-urlencoded";
    thead["Accept-Encoding"]="gzip,deflate";

    Request re;
    Response rep=re.post(url,thead,tcooke,tdata);
    url=rep.getdata();

cout<<url<<endl;

    int n=url.find("psessionid");
    n+=13;
    url=string(url,n,url.find("\",\"",n)-n);
cout<<url<<endl;
    return url;
}
map<string,string> User::somerequest(map<string,string> tcookie)
{
    string vfwebqq=getvfwebqq(tcookie);
    string psessionid=getpsessionid(tcookie);
    map<string,string> ret;
    ret["vfwebqq"]=vfwebqq;
    ret["psessionid"]=psessionid;
    return ret;
}
//////////////////一切准备就绪//////////////////////

int User::get_friend(map<string,string>tcookie,map<string,string>tparameter)
{
    string qhash=get_hash(atol(qid.c_str()),tparameter["psessionid"]);
    cout<<qhash<<endl;

    Request re;
    string url="http://s.web2.qq.com/api/get_user_friends2";
    string data="r=%7B%22vfwebqq%22%3A%22";
    data+=tparameter["vfwebqq"]+"%22%2C%22hash%22%3A%22";
    data+=qhash;
    data+="%22%7D";

    map<string,string> head;
    head["Referer"]="http://s.web2.qq.com/proxy.htm…v=20130916001&callback=1&id=1";
    Response rep=re.post(url,head,tcookie,data);

    friends=rep.getdata();

cout<<"this is get friend information>>>>>>>>>>>>>>"<<endl<<friends<<endl;

    return 0;
}
int User::get_group_name_list(map<string,string>tcookie,map<string,string>tparameter)
{
    string url="http://s.web2.qq.com/api/get_discus_list?clientid=53999199&psessionid=";
    url+=tparameter["psessionid"];
    url+="&vfwebqq="+tparameter["vfwebqq"];
    url+="&t="+get_time();

    map<string,string> head;
    head["Referer"]="http://s.web2.qq.com/proxy.htm…v=20130916001&callback=1&id=1";

    Request re;
    Response rep=re.get(url,head,tcookie);
    group=rep.getdata();
    return 0;
}

int User::get_discus_list(map<string,string>tcookie,map<string,string>tparameter)
{
    string url="http://s.web2.qq.com/api/get_discus_list?clientid=53999199&psessionid=";
    url+=tparameter["psessionid"];
    url+="&vfwebqq="+tparameter["vfwebqq"];
    url+="&t="+get_time();

    map<string,string> head;
    head["Referer"]="http://s.web2.qq.com/proxy.htm…v=20130916001&callback=1&id=1";

    Request re;
    Response rep=re.get(url,head,tcookie);
    discus=rep.getdata();

    return 0;
}
int get_self_info()
{
    string url="";
    return 0;
}

int User::get_allinfo(map<string,string> tcookie,map<string,string>tparameter)
{
    get_friend(tcookie,tparameter);
    get_group_name_list(tcookie,tparameter);
    get_discus_list(tcookie,tparameter);
    //get_self_info();


    return 0;
}


