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

#define  MAXBUFFERSIZE 200000
#define MY_BUFF_SIZE  1024
#include"zlib.h"
#include <assert.h>
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

inline int UnCompress_Dataa(const char * source,string &out ,unsigned long source_len)
{
cout<<"进入了"<<endl;
    if(source==nullptr||source_len==0)
    {
        cout<<"没有要压缩的数据"<<endl;
        return false;
    }
    Byte uncompr[MAXBUFFERSIZE];
    memset(uncompr, 0, MAXBUFFERSIZE);
    uLong uncomprLen = MAXBUFFERSIZE;
    int err;

    err = uncompress(uncompr, &uncomprLen, (const Bytef *)source, source_len);
    if (err != Z_OK)
    {
        cout << "uncompess error: " << err << endl;
        return false;
    }
    cout << "compress size: " << source_len << "  uncompressed size : " << uncomprLen << endl;
    char aa[8888];
    memcpy(aa, uncompr, uncomprLen);
    out=aa;
    return true;
}
inline int ungzip(const char* source,int len,char*des)//des为解压之后的内容要保存的地方
{
	int ret,have;
	int offset=0;
	void * buff = (void *)source;
	z_stream d_stream;
	char uncompr[MY_BUFF_SIZE]={0};//
	d_stream.zalloc = Z_NULL;
	d_stream.zfree = Z_NULL;
	d_stream.opaque = Z_NULL;
	d_stream.next_in = Z_NULL;//inflateInit和inflateInit2都必须初始化next_in和avail_in
	d_stream.avail_in = 0;//deflateInit和deflateInit2则不用

	ret = inflateInit2(&d_stream,47);//这里一定要用inflateInit2（）函数进行初始化，其他的不行，我试过貌似第二个参数为31也可以，可以按照自己情况选择
	d_stream.next_in= (Byte *)buff;//下一个要解压的字节
	d_stream.avail_in= len;//还有多少字节需要解压

	do//本处含义为：开始解压，每循环一次代表解压出了1024个字节（可能只解压了几百个字节）。并将解压出的内容缓存到uncompr中，然后赋值给des；
	{
	 bzero(uncompr, MY_BUFF_SIZE);
	 d_stream.next_out=(Bytef *)uncompr;
	 d_stream.avail_out=MY_BUFF_SIZE;

	 ret = inflate(&d_stream,Z_NO_FLUSH);//这里就是解压函数
	 assert(ret != Z_STREAM_ERROR);
	 if (ret != Z_OK && ret != Z_STREAM_END)//解压正常返回Z_OK,解压结束返回Z_STREAM_END,
     {
       printf("\ninflate ret = %d\n", ret);//可能出现ret=-3，即Z_DATA_ERROR,这可能是因为数据格式不对；还有其他错误，暂时没碰到
       break;
     }

	 have=MY_BUFF_SIZE-d_stream.avail_out;//这里是将uncompr赋值给des
	 memcpy(des+offset,uncompr,have);
	 offset+=have;
	}while(d_stream.avail_out==0);//结束循环条件，当最后一次循环时，解压出的字节一般不到1024字节，所以d_stream.avail_out！=0，即还有可用字节。
	inflateEnd(&d_stream);
	memcpy(des+offset,"\0",1);//\0表示结束
	return ret;
}

/*
private static String get_hash (long x, String K) {
    int [] N = new int [4];
    for (int T = 0; T > 24 & 255 ^ U [0].charAt (0);
    V [1] = x >> 16 & 255 ^ U [0].charAt (1);
    V [2] = x >> 8 & 255 ^ U [1].charAt (0);
    V [3] = x & 255 ^ U [1].charAt (1);

    long [] U1 = new long [8];

    for (int T = 0; T > 1] : V [T >> 1];
    }

    String [] N1 = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F"};
    String V1 = "";
    for (long aU1 : U1) {
        V1 += N1 [(int) ((aU1 >> 4) & 15)];
        V1 += N1 [(int) (aU1 & 15)];
    }
    return V1;
}*/

/*function getHash(b,i)
{
    for(var a=i+"password error",s="",j=[];;)
        if(s.length<=a.length)
        {
            if(s+=b,s.length==a.length)
            break
        }
        else
        {
            s=s.slice(0,a.length);
        break
        }
        for(var d=0;d<s.length;d++)
            j[d]=s.charCodeAt(d)^a.charCodeAt(d);
        a=["0","1","2","3","4","5","6","7","8","9","A","B","C","D","E","F"];
        s="";
        for(d=0;d<j.length;d++)
            s+=a[j[d]>>4&15],s+=a[j[d]&15];
        return s;
}*/
inline string get_hash(long b,string i)
{
    string a = i+"password error";
    string s;
    vector <int> j;
    for(;;){
        if(s.size()<=a.size())
        {
            s+=to_string(b);
            if(s.size()==a.size())
                break;
        }
        else
        {
            s=string(s,0,(int)a.size());
            break;
        }
    }

    for(int d=0; d<(int)s.size();d++)
            j.push_back(((int )s[d]) ^ ((int)a[d]));
    char arr[] ={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    s="";
    for(int d=0;d<(int )j.size();d++)
    {
            s+=arr[j[d]>>4&15];
            s+=arr[j[d]&15];
    }
    return s;
}
inline string  getHashCode(long b, string j)
{
    int a[]= {0,0,0,0};
    for(int i=0;i<j.size();i++)
        a[i%4] ^= (int)j[i];

    string w[] = {"EC","OK"};
    d = [0,0,0,0]

    d[0] = int(b) >> 24 & 255 ^ ord(w[0][0])
    d[1] = int(b) >> 16 & 255 ^ ord(w[0][1])
    d[2] = int(b) >> 8 & 255 ^ ord(w[1][0])
    d[3] = int(b) & 255 ^ ord(w[1][1])

    w = [0,0,0,0,0,0,0,0]

    for i in range(0,8):
        if i%2 == 0:
            w[i] = a[i>>1]
        else:
            w[i] = d[i>>1]
    a = ["0","1","2","3","4","5","6","7","8","9","A","B","C","D","E","F"]
    d = ""

    for i in range(0,len(w)):
        d += a[w[i]>>4&15]
        d += a[w[i]&15]

    return d
}
#endif // OTHER_H_INCLUDED
