#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <netdb.h>
#include <stdarg.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>


#include<openssl/bio.h>
#include<openssl/ssl.h>
#include<openssl/err.h>
#include<openssl/rsa.h>
#include<openssl/pem.h>

#include <stdio.h>
#include <sstream>
#include <string>
using namespace std;
string has33(string const &t)
{
    int e=0;
    int n = t.size();
    for (int i = 0; i < n; ++i)
        e += (e << 5) + t[i];
    e= 2147483647 & e;
    stringstream stream;
    stream<<e;
    return stream.str();
}
int my_error(char * err,int line)
{
    perror(err);
    cout<<line<<endl;
    exit(1);
}
void ShowCerts(SSL * ssl)
{
  X509 *cert;
  char *line;

  cert = SSL_get_peer_certificate(ssl);
  if (cert != NULL) {
    printf("Digital certificate information:\n");
    line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
    printf("Certificate: %s\n", line);
    free(line);
    line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
    printf("Issuer: %s\n", line);
    free(line);
    X509_free(cert);
  }
  else
    printf("No certificate information！\n");
}
class Murl
{
    string Host;        //主机名
    string Method;      //请求方法
    string Url;         //请求url

public:
    string Message;     //最后发送的报文
    Murl(const string method,const string &host,const string &url)//给主机,url 初始化报文
    {
        Url=url;
        Host=host;
        Method=method;
        Message=Method+" "+url+" HTTP/1.1\r\n";
        Message+="Accept: */*\r\n";
        Message+="Host: "+host+"\r\n";
        Message+="user-agent: Mozilla/curl/7.52.1\r\n";
    }
    void addend()
    {
        Message+="\r\n";
    }
};
class Network
{

    int isock;
    SSL *ssl;
    SSL_CTX *ctx;
    struct sockaddr_in pin;
public:
    Murl murl;
    Network(const string method,const string &host,const string &url):murl(method,host,url)
    {
        struct hostent *hosts;
        if(hosts=gethostbyname(host.c_str()),hosts==0)
            my_error((char *)"gethostbyname error",__LINE__);

        bzero(&pin,sizeof(pin));
        pin.sin_family=AF_INET;
        pin.sin_port=htons(443);
        pin.sin_addr.s_addr=((struct in_addr*)(hosts->h_addr))->s_addr;

        if((isock = socket(AF_INET,SOCK_STREAM, 0))==-1)
            my_error((char *)"open socket error",__LINE__);
        if(connect(isock, (const sockaddr*)&pin, sizeof(pin))==-1)
            my_error((char *)"oconnect error",__LINE__);

        SSL_library_init();
        OpenSSL_add_all_algorithms();
        SSL_load_error_strings();
        ctx = SSL_CTX_new(SSLv23_client_method());
        if (ctx == NULL)
        {
            ERR_print_errors_fp(stdout);
            exit(1);
        }
        ssl = SSL_new(ctx);
        SSL_set_fd(ssl, isock);

        if (SSL_connect(ssl) == -1)
        {
            ERR_print_errors_fp(stderr);
            exit(1);
        }
        ShowCerts(ssl);
printf("Connected with %s encryption\n", SSL_get_cipher(ssl));
    }
    void mysend(Murl & ssend)
    {
ssend.addend();
cout<<"this is my send->>>>"<<endl<<ssend.Message.c_str()<<endl;
        if (SSL_write(ssl,ssend.Message.c_str(),ssend.Message.size())< 0)
            my_error((char *)"SSL_write error",__LINE__);

    }
    void myrecv(string &headpass,string &pageContent)
    {
        int len;
        char c;
        int flag=0;
        int lenend;
        while((len = SSL_read(ssl, &c,1))>0)
        {
            if(flag&&c=='\r')
            {
                SSL_read(ssl, &c,1);
                headpass+=c;
                break;
            }
            else
                flag=0;

            if(c=='\n')
                flag=1;

            headpass+=c;
        }
        lenend=headpass.find("Content-Length:");
        int i=lenend+16;
        lenend=0;
        for(;headpass[i]!='\r';i++)
        {
            lenend=lenend*10+headpass[i]-'0';
        }
cout<<headpass<<endl;
cout<<lenend<<endl;
        lenend--;
        lenend--;

        char *buffer=new char[lenend];
        len=0;
        int lenrecved=0;
        while(lenend>lenrecved)
        {
            len = SSL_read(ssl, buffer, lenend-lenrecved);
            lenrecved+=len;
        }
        pageContent=buffer;
        FILE *fd=fopen("timp.png","w");
        fwrite(buffer,sizeof(char),lenend,fd);
        fclose(fd);
        delete []buffer;
cout<<pageContent<<endl;
        }
};
string findcooktopic(string const & head)
{
    int len=0;
    string timp;
    string cookies;
    while(len!=-1)
    {
        len=head.find("Set-Cookie:",len);
        if(len==-1)
            break;
        len+=12;
        timp=string(head,len,head.find(";",len)-len);
        cookies+="Cookies :"+timp+"\r\n";
    }
    return cookies;
    //cout<<cookies<<endl;
}
string findpictonext(string const &head,string const &answer)
{
    int len=0;
    len=head.find("Set-Cookie: qrsig=",len);
    len+=18;
    string cookies=string(head,len,head.find(";",len)-len);
    return cookies;
}
int main()
{
    Network network1("GET","xui.ptlogin2.qq.com","/cgi-bin/xlogin?daid=164&target=self&style=40&pt_disable_pwd=1&mibao_css=m_webqq&appid=501004106&enable_qlogin=0&no_verifyimg=1&s_url=http://web2.qq.com/proxy.html&f_url=loginerroralert&strong_login=1&login_state=10&t=20131024001");
    network1.mysend(network1.murl);
    string head ;
    string answer;
    network1.myrecv(head,answer);
cout<<"assdddddddddddddddddddddddd我就离开扩扩扩扩扩扩扩扩扩做寻错 "<<endl;

    string allcookies;

    head=findcooktopic( head);
    allcookies=head;
    Network npic("GET","ssl.ptlogin2.qq.com","/ptqrshow?appid=501004106&e=2&l=M&s=3&d=72&v=4&t=0.7339221978811015&daid=164&pt_3rd_aid=0");
    npic.murl.Message+=allcookies;
    npic.mysend(npic.murl);
    head="";
    answer="";
    npic.myrecv(head,answer);

sleep(12);
    head=findpictonext(head,answer);
    string url="/ptqrlogin?u1=http%3A%2F%2Fweb2.qq.com%2Fproxy.html&ptqrtoken=";
    url+=has33(head)+"&ptredirect=0&h=1&t=1&g=1&from_ui=1&ptlang=2052&action=0-0-1523282612995&js_ver=10270&js_type=1&login_sig="+head+"&pt_uistyle=40&aid=501004106&daid=164&mibao_css=m_webqq&";

    Network login("GET","ssl.ptlogin2.qq.com",url);
    head="Cookies :qrsig="+head+"\r\n";
    allcookies+=head;
    login.murl.Message+=allcookies;

    login.murl.Message+="Referer: https://xui.ptlogin2.qq.com/cgi-bin/xlogin?daid=164&target=self&style=40&pt_disable_pwd=1&mibao_css=m_webqq&appid=501004106&enable_qlogin=0&no_verifyimg=1&s_url=http%3A%2F%2Fweb2.qq.com%2Fproxy.html&f_url=loginerroralert&strong_login=1&login_state=10&t=20131024001\r\n";

    login.mysend(login.murl);
    head="";
    answer="";
    login.myrecv(head,answer);

    cout << "Hello world!" << endl;
    return 0;
}
