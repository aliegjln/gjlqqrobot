#include "Socket.h"
#include "other.h"
Socket::Socket(string domain)
{
    struct hostent *hosts;
    if(hosts=gethostbyname(domain.c_str()),hosts==0)
        my_error("gethostbyname error",__LINE__);

    memset(&pin,0,sizeof(pin));
    pin.sin_family=AF_INET;
    pin.sin_port=htons(443);
    pin.sin_addr.s_addr=((struct in_addr*)(hosts->h_addr))->s_addr;

    if((isock = socket(AF_INET,SOCK_STREAM, 0))==-1)
        my_error("open socket error",__LINE__);
    if(connect(isock, (const sockaddr*)&pin, sizeof(pin))==-1)
        my_error("oconnect error",__LINE__);


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

}
void Socket::mysend(string head)
{
Print("this is my send->>>>\n",head,'\n');
    if(SSL_write(ssl,head.c_str(),head.size())< 0)
        my_error("SSL_write error",__LINE__);
//Print("my send end>\n");
}
pair<string,string> Socket::myrecv()
{
    char c;
    int flag=0;
    string headpass;
    while(SSL_read(ssl, &c,1)>0)
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

    int lenend=0;
    for(int i=headpass.find("Content-Length:")+16;headpass[i]!='\r'&&i>0;i++)
        lenend=lenend*10+headpass[i]-'0';

cout<<"recv len:"<<lenend<<endl;
//    lenend--;
//    lenend--;
    string pageContent;
    if(lenend>0)
    {
        char *buffer=new char[lenend];
        int lenrecved=0;
        while(lenend>lenrecved)
        {
            int len = SSL_read(ssl, buffer, lenend-lenrecved);
            lenrecved+=len;
        }
        string page(buffer,lenend);
        pageContent=page;
        delete []buffer;
    }

cout<<headpass<<endl;
cout<<pageContent<<endl;
cout<<pageContent.size()<<endl;
    close(isock);
    SSL_shutdown(ssl);
    SSL_free(ssl);
    SSL_CTX_free(ctx);
    return {headpass,pageContent};
}

Socket::~Socket()
{
    //dtor
}
