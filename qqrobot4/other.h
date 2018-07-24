#ifndef OTHER_H_INCLUDED
#define OTHER_H_INCLUDED

#include <iostream>
#include <limits.h>
#include <initializer_list>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <functional>
#include <iterator>
#include <memory>
#include <map>
#include <fstream>

#include <ctime>//时间戳
#include <netdb.h>//gethostbyname
#include <sys/types.h>
#include <unistd.h>
#include <cstring>

#include<openssl/bio.h>
#include<openssl/ssl.h>
#include<openssl/err.h>
#include<openssl/rsa.h>
#include<openssl/pem.h>

using namespace std;
using namespace placeholders;
// 用来终止递归并处理包中最后一个元素
template <typename T>
void Print(const T &t)
{
    cout <<t;
}
// 包中除了最后一个元素之外的其他元素都会调用这个版本的print
template <typename T, typename...Args>
void Print(const T &t, const Args&...rest)
{
    cout <<t;     // 打印第一个实参
    Print(rest...);       // 递归调用，打印其他实参
}

inline int my_error(const char * err,int line)
{
    perror(err);
    cout<<line<<endl;
    exit(1);
}
inline string has33(string const &t)
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
inline string get_time()
{
    time_t result = time(nullptr);
cout<<asctime(localtime(&result))<<endl;//Sun Nov 22 11:48:58 2015
cout<<result<<endl;
    return  to_string(result) ;
}
#endif // OTHER_H_INCLUDED
