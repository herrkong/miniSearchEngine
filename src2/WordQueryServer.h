#pragma once
#include "WordQuery.h"
#include "TcpServer.h"
#include "Threadpool.h"


namespace hk
{

class WordQueryServer
{
public:
    WordQueryServer(const string & configPath);
    ~WordQueryServer() { cout <<"~WordQueryServer()"<<endl; }

    void start();

private:
    //被注册回调函数 提供给TcpServer使用
    void onConnection(const TcpConnectionPtr & conn);
    void onMessage(const TcpConnectionPtr & conn);
    void onClose(const TcpConnectionPtr & conn);
    void doTaskThread(const TcpConnectionPtr & conn,const string & msg);


private:
    Configuration _conf;
    TcpServer _tcpserver;
    Threadpool _threadpool;
    
};


}; //end of namespace hk
