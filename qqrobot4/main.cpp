#include "other.h"
#include "Request.h"
#include "User.h"
#include "Socket.h"
#include "Wxuser.h"
int main()
{
    Request re;
    Response rep=re.get("http://zhanzhang.baidu.com/sitesubmit/index");
    cout<<rep.getdata()<<endl;
    return 0;

//    Request re;
//    re.get("https://web2.qq.com/");

//    string ss("nN5NOOvM-s50NyF669d*ZQWRmzNmXUYOOE5b2QIv42LZH41-Nz1ZfUnK5WcBO*hT");
//    cout<<has33(ss);
//    return 0;

/**之前的写了一半的qq请求*/

    User gjl;
    map<string,string> nextcook=gjl.login();

    cout<<"登录成功！！！！！！！！！！！！！！！！"<<endl;
    for(auto i:nextcook)
        Print(i.first," : ",i.second,"\n");

    gjl.somerequest(nextcook);


/**这是微信的登录,

    Wxuser gjl;
    gjl.tologin();
*/
    cout << "Hello world!" << endl;
    return 0;
}

