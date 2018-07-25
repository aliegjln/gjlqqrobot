#include "other.h"
#include "Request.h"
#include "User.h"
#include "Socket.h"
#include "Wxuser.h"
int main()
{
    cout<<(int) 'H'<<endl;
    string ps="8368046764001d636f6e6e7365727665725f77656271714031302e3133332e34312e383400001ad00000066b026e040015808a206d0000000a406172314338344a69526d0000002859185d94e66218548d1ecb1a12513c86126b3afb97a3c2955b1070324790733ddb059ab166de6857";
    string qhash=get_hash(1540853188,ps);
    cout<<qhash<<endl;
    return 0;
//    Request re;
//    Response rep=re.get("http://d1.web2.qq.com/channel/login2");
//    cout<<rep.getdata()<<endl;
//    return 0;



//    Request re;
//    re.get("https://web2.qq.com/");

//    string ss("nN5NOOvM-s50NyF669d*ZQWRmzNmXUYOOE5b2QIv42LZH41-Nz1ZfUnK5WcBO*hT");
//    cout<<has33(ss);
//    return 0;

/**之前的写了一半的qq请求*/

    User gjl;
    map<string,string> nextcook=gjl.login();
cout<<"登录成功！！！！！！！！！！！！！！！！"<<endl;

    map<string ,string> someparameter=gjl.somerequest(nextcook);

    gjl.get_allinfo(nextcook,someparameter);


/**这是微信的登录,

    Wxuser gjl;
    gjl.tologin();
*/
    cout << "Hello world!" << endl;
    return 0;
}

