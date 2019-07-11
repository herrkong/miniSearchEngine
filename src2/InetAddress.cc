#include "InetAddress.h"
#include <string.h>
namespace hk
{

InetAddress::InetAddress(unsigned short port)
{
    memset(&_addr,0,sizeof(struct sockaddr_in));
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(port);
    _addr.sin_addr.s_addr = INADDR_ANY;
   // cout<<"InetAddress(unsigned short port)"<<endl;
}

InetAddress::InetAddress(const string & ip,unsigned short port)
{
   // printf("port=%d\n",port);
    memset(&_addr,0,sizeof(struct sockaddr_in));
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(port);
    _addr.sin_addr.s_addr = inet_addr(ip.c_str());
    cout<<"InetAddress(const string & ip,unsigend short port)"<<endl;
}

InetAddress::InetAddress(const struct  sockaddr_in &  addr)
:_addr(addr)
{
   // cout<<"InetAddress(const struct sin_addr addr)"<<endl;
}

string  InetAddress::ip() const
{
    return string(inet_ntoa(_addr.sin_addr));
    //网络字节序转主机字节序
}

unsigned short InetAddress::port() const
{
    return ntohs(_addr.sin_port);
    
}


}//end of namespace hk
