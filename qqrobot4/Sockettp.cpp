#include "Sockettp.h"
#include "other.h"
Sockettp::Sockettp(string domain)
{
    struct hostent *hosts;
    if(hosts=gethostbyname(domain.c_str()),hosts==0)
        my_error("gethostbyname error",__LINE__);

    memset(&pin,0,sizeof(pin));
    pin.sin_family=AF_INET;
    pin.sin_port=htons(80);
    pin.sin_addr.s_addr=((struct in_addr*)(hosts->h_addr))->s_addr;

    if((isock = socket(AF_INET,SOCK_STREAM, 0))==-1)
        my_error("open socket error",__LINE__);
    if(connect(isock, (const sockaddr*)&pin, sizeof(pin))==-1)
        my_error("oconnect error",__LINE__);


}
void Sockettp::mysend(string head)
{
Print("this is my send->>>>\n",head,'\n');
    if(send(isock,head.c_str(),head.size(),0)< 0)
        my_error("SSL_write error",__LINE__);
//Print("my send end>\n");
}
pair<string,string> Sockettp::myrecv()
{
    char c;
    int flag=0;
    string headpass;
    while(recv(isock, &c,1,0)>0)
    {
        if(flag&&c=='\r')
        {
            recv(isock, &c,1,0);
            headpass+=c;
            break;
        }
        else
            flag=0;
        if(c=='\n')
            flag=1;
        headpass+=c;
    }
cout<<headpass<<endl;
string pageContent;
cout<<"find Content-Length resout :"<<(int )headpass.find("Content-Length")<<endl;
    if((int )headpass.find("Content-Length:")>=0)
    {
        int lenend=0;
        for(int i=headpass.find("Content-Length:")+16;headpass[i]!='\r'&&i>0;i++)
            lenend=lenend*10+headpass[i]-'0';

cout<<"recv for len:"<<lenend<<endl;

        if(lenend>0)
        {
            char *buffer=new char[lenend];
            int lenrecved=0;
            while(lenend>lenrecved)
            {
                int len = recv(isock, buffer, lenend-lenrecved,0);
                lenrecved+=len;
            }
            string page(buffer,lenend);
            pageContent=page;
            delete []buffer;
        }
cout<<"i recv from len>>>>>>>>>>>>"<<endl;
cout<<pageContent<<endl;
cout<<pageContent.size()<<endl;
    }
    else if(headpass.find("Transfer-Encoding")>=0&&headpass.find("chunked")>=0)
    {
int number=0;
        while(1)
        {
number++;
            int lenend=0;
            char c;
            while(recv(isock, &c,1,0)>0)
            {
                if('a'<=c&&c<='z')
                    lenend=lenend*16+c-'a'+10;
                if('A'<=c&&c<='Z')
                    lenend=lenend*16+c-'A'+10;
                if('0'<=c&&c<='9')
                    lenend=lenend*16+c-'0';
                if(c=='\n')
                    break;
            }

cout<<"recv for number :"<<number<<"'s"<<"len:"<<lenend<<endl;
            if(lenend>=0)
            {
                char *buffer=new char[lenend];
                int lenrecved=0;
                while(lenend>lenrecved)
                {
                    int len = recv(isock, buffer, lenend-lenrecved,0);
                    lenrecved+=len;
                }
                string page(buffer,lenend);
                pageContent+=page;

                recv(isock, buffer, 2,0);
                delete []buffer;
                if(lenend==0)
                    break;
            }
        }
    cout<<pageContent<<endl;
    cout<<pageContent.size()<<endl;
    }
    close(isock);
    return {headpass,pageContent};
}

Sockettp::~Sockettp()
{
    //dtor
}
