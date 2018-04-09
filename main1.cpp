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

#include <string>
using namespace std;
/*void ShowCerts(SSL * ssl)
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
}*/
string aa()
{
    string a;
    a+="GET /cgi-bin/xlogin?daid=164&target=self&style=40&pt_disable_pwd=1&mibao_css=m_webqq&appid=501004106&enable_qlogin=0&no_verifyimg=1&s_url=http://web2.qq.com/proxy.html&f_url=loginerroralert&strong_login=1&login_state=10&t=20131024001";
    a+=" HTTP/1.1\r\n";

    //a+="cookie:pgv_pvi=5630422016; TDC_token=3995359942; RK=ggGTmVEaG7; ptcz=443c83fba3042ba7541a1110e0bb227ccf225b27d0a381344b37f637a989cf2a; pt2gguin=o1303393472; pgv_pvid=8690754824; pt_recent_uins=a992c971885276a9c4e644376b318d49e71d0ad886aeb18baacb776bc73c292e69a668986456cb37b2ef174fa12852394235c8f9e2e2534a; pt_guid_sig=1c57e86a2b3ae5afa51aff912534aa209319519329a55edc974a319ccfbae34f; pgv_si=s5480275968; qrsig=JbYxFwwXm7w6mKQei*EZQJJs1dil9u7v6xDFeDJmK4Bd7t3XgLn7Vmpw4DCEZ9Ig\r\n";
    //a+="referer:https://web2.qq.com/\r\n";
    //a+="upgrade-insecure-requests:1\r\n";
    a+="Host: xui.ptlogin2.qq.com\r\n";
    a+="user-agent: curl/7.52.1\r\n";a+="Accept: */*\r\n";
    a+="\r\n";
    return a;
}
int main()
{
    struct hostent *host;
    string url="web2.qq.com";

    host=gethostbyname(url.c_str());
cout<<"this is host->"<<inet_ntoa(*(struct in_addr*)(host->h_addr))<<endl;
    if(host==0)
    {
        cout<<"gethostbyname error\n"<<endl;
        exit(1);
    }

    struct sockaddr_in pin;
    int port=80;
    bzero(&pin,sizeof(pin));
    pin.sin_family=AF_INET;
    pin.sin_port=htons(80);
    pin.sin_addr.s_addr=((struct in_addr*)(host->h_addr))->s_addr;
    int isock;
    if((isock = socket(AF_INET, SOCK_STREAM, 0))==-1)
    {
        cout<<"open socket error\n"<<endl;
        exit(1);
    }

    string requestHeader;
    requestHeader=requestHeader+"GET /" + " HTTP/1.1\r\n";
   // requestHeader=requestHeader+"Cookie: "+"pt_guid_sig=ebd02471ee80bd46e9c93661ebf202cc70f2eaebe29e599634b8628dc858b7d3; TDC_token=2979415633; pgv_pvi=95207424; qrsig=lO2tanWOCkOdh9aoBlWteWLKWEp-FGg0QpgK7zh6h57yDyomh8X4akALoOwA4-rZ; pgv_si=s7178227712; pt_login_sig=E8p2MaMgKsPhYWSlbjQBKSxgKcUVN5bZZiHqjK3SOXsN*DK4XhS54cGgXRxcBsIE; pt_clientip=09127520d81a85aa; pt_serverip=cc570a3272d1282c; pt_local_token=-901950810; uikey=45cdd6427f186ae71065f851140d624243b5d664398e4d065290e82a7dc55cd1"+"\r\n";
    requestHeader=requestHeader+"Host: "+"web2.qq.com"+"\r\n";
    requestHeader+="Accept: */*\r\n";
    requestHeader+="User-Agent: Mozilla/curl/7.52.1\r\n";
    //requestHeader+="connection:Keep-Alive\r\n";
   // requestHeader+="Upgrade-Insecure-Requests: 1\r\n";
    requestHeader+="\r\n";
   // requestHeader=aa();
cout<<"this is my send->"<<requestHeader<<endl;

    if(connect(isock, (const sockaddr*)&pin, sizeof(pin))==-1)\
    {
        cout<<"connect error\n"<<endl;
        exit(1);
    }
/*
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

if (SSL_connect(ssl) == -1)
ERR_print_errors_fp(stderr);
else
{
printf("Connected with %s encryption\n", SSL_get_cipher(ssl));
ShowCerts(ssl);
}*/

    if(send(isock, requestHeader.c_str(), requestHeader.size(), 0)==-1)
    {
        cout<<"send error\n"<<endl;
        exit(1);
    }

    char buffer[1024];
//    recv(isock,buffer,1023,0);

    int len;
    string pageContent;
/*while((len = SSL_read(ssl, buffer, 1024-1))>0)
{
    buffer[len]='\0';
    cout<<buffer<<endl;
    pageContent+=buffer;
}*/
    while((len = recv(isock, buffer, 1024-1, 0))>0)
    {
        buffer[len]='\0';
        cout<<buffer<<endl;
        pageContent+=buffer;
    }
    cout<<pageContent<<endl;
    cout << "Hello world!" << endl;
    return 0;
}

