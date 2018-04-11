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

#include <string>
using namespace std;

class M_url
{
public :
    string url;
    string host;
    string ssend;

    M_url(const string &shost,const string &surl)
    {
        url=surl;
        host=shost;
        ssend="GET "+url+" HTTP/1.1\r\n";
        ssend+="Accept: */*\r\n";
        ssend+="Host: "+host+"\r\n";
        ssend+="user-agent: Mozilla/curl/7.52.1\r\n";
    }
    void addhead(const string &key,const string &value)
    {
        ssend+=key+": "+value+"\r\n";
    }
    void endadd()
    {
        ssend+="\r\n";
    }
};
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
void https(const int &isock, M_url & seurl)
{
    SSL_CTX *ctx;
    SSL *ssl;
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
    /* 建立 SSL 连接 */
    if (SSL_connect(ssl) == -1)
    ERR_print_errors_fp(stderr);
    else
    {
        printf("Connected with %s encryption\n", SSL_get_cipher(ssl));
        ShowCerts(ssl);
    }

    seurl.endadd();
cout<<"this is my head->>>>"<<endl<<seurl.ssend<<endl;
    if (SSL_write(ssl,  seurl.ssend.c_str(),seurl.ssend.size())< 0)
    {
        cout<<"SSL_write error\n"<<endl;
        exit(1);
    }
    int len;
    string headpass;
    string pageContent;
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
        {
            flag=0;
        }
        if(c=='\n')
        {

            flag=1;
        }
        headpass+=c;
    }

    lenend=headpass.find("Content-Length:");
    int i=lenend+16;
    lenend=0;
    for(i;headpass[i]!='\r';i++)
    {
        lenend=lenend*10+headpass[i]-'0';
    }
cout<<headpass<<endl;
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
    cout << "Hello world!" << endl;
}
void http(const int &isock, M_url & seurl)
{
    seurl.endadd();
cout<<"this is my head->>>>"<<endl<<seurl.ssend<<endl;
    if(send(isock, seurl.ssend.c_str(),  seurl.ssend.size(), 0)==-1)
    {
        cout<<"send error\n"<<endl;
        exit(1);
    }
    char buffer[1024];
    int len;
    string pageContent;
    while((len = recv(isock, buffer, 1024-1, 0))>0)
    {
cout << "Hello worldasdsaaaaf!" << endl;
        buffer[len]='\0';
        cout<<buffer<<endl;
        pageContent+=buffer;
    }
    cout<<pageContent<<endl;
    cout << "Hello world!" << endl;
}
void addgenglu(M_url url)
{
    url.addhead("Referer","http://web2.qq.com/");
    url.addhead("Connection",	"keep-alive");
}
void picture(M_url &url)
{
cout<<"?????????????"<<endl;

    string aa="pgv_pvi	1165471744;";
    aa+="pgv_si	s9260925952;";
    aa+="pt_clientip	43ac7520d81afe55;";
    aa+="pt_guid_sig	368cddd0ba7d570f67c0b3340c9fa4793c39a267befcb320add6da40f5fd6887;";
    aa+="pt_local_token	-945903934;";
    aa+="pt_login_sig	jEj7l1zxy7PMifcJB-vbXPZv4*XSnzQnZv6srAJg-35pBm6QPZkU6osYUFsOjtlK;";
    aa+="pt_serverip	5c340a3272cb70e6;";
    aa+="qrsig	wMeUAJk9YC5KSK06eyy3jqmfGT61B7Q6vvlokfBo-8tk9Q7D4Nb6KdtHIgfyNhBy;";
    aa+="uikey	ad05910b8b845dd2fadbc11b7849a110632d1e98382b4c47adea607c821b14be;";
    url.addhead("Cookie",aa);
    url.addhead("Referer","https://xui.ptlogin2.qq.com//cgi-bin/xlogin?daid=164&target=self&style=40&pt_disable_pwd=1&mibao_css=m_webqq&appid=501004106&enable_qlogin=0&no_verifyimg=1&s_url=http://web2.qq.com/proxy.html&f_url=loginerroralert&strong_login=1&login_state=10&t=20131024001");
}
int main()
{
    M_url url1("web2.qq.com","/");

    M_url url2("xui.ptlogin2.qq.com","/cgi-bin/xlogin?daid=164&target=self&style=40&pt_disable_pwd=1&mibao_css=m_webqq&appid=501004106&enable_qlogin=0&no_verifyimg=1&s_url=http://web2.qq.com/proxy.html&f_url=loginerroralert&strong_login=1&login_state=10&t=20131024001");
    addgenglu(url2);

    M_url url3("ui.ptlogin2.qq.com","/js/ptloginout.js");
    M_url urlpic("ssl.ptlogin2.qq.com","/ptqrshow?appid=501004106&e=2&l=M&s=3&d=72&v=4&t=0.689966450051287&daid=164&pt_3rd_aid=0");
    picture(urlpic);

    struct hostent *host;

    host=gethostbyname(url2.host.c_str());

    if(host==0)
    {
        cout<<"gethostbyname error\n"<<endl;
        exit(1);
    }

    struct sockaddr_in pin;
    bzero(&pin,sizeof(pin));
    pin.sin_family=AF_INET;

    pin.sin_port=htons(443);

    pin.sin_addr.s_addr=((struct in_addr*)(host->h_addr))->s_addr;
    int isock;
    if((isock = socket(AF_INET,SOCK_STREAM, 0))==-1)
    {
        cout<<"open socket error\n"<<endl;
        exit(1);
    }
    if(connect(isock, (const sockaddr*)&pin, sizeof(pin))==-1)\
    {
        cout<<"connect error\n"<<endl;
        exit(1);
    }
    https(isock,url2);
    //https(isock,url2);
    //http(isock,url1);
    //http(isock,url2);
    return 0;
}
