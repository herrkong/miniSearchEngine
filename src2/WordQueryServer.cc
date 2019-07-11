#include "WordQueryServer.h"

using namespace std;

namespace hk
{

//字符串转成整数 stoi atoi
WordQueryServer::WordQueryServer(const string & configPath)
:_conf(configPath)
//,_tcpserver(_conf.getConfigMap()["ip"],
//          atoi(_conf.getConfigMap()["port"].c_str()))  
,_tcpserver(_conf.getConfigMap()["ip"],
            stoi(_conf.getConfigMap()["port"]))
,_threadpool(4,10)
{
    _threadpool.start();
    cout<<"WordQueryServer(const string & configPath)"<<endl;
}

void WordQueryServer::doTaskThread(const TcpConnectionPtr & conn,const string & msg)
{
    //初始化 wordQuery对象
   WordQuery wordQuery(_conf,msg);
   string ret = wordQuery.doQuery();
   cout<<ret<<endl;
   conn->sendInLoop(ret);

}


void WordQueryServer::onConnection(const TcpConnectionPtr & conn)
{
    cout<<"连接中..."<<endl;
    cout<<conn->toString()<<"连接成功"<<endl;
    conn->send("服务器配置成功！");
}

void WordQueryServer::onMessage(const TcpConnectionPtr & conn)
{
    cout<<"数据发送中..."<<endl;
    string msg = conn->receive();
    cout<<"收到客户端数据:"<<msg<<endl;//收到查询词 

    //在这里写WordQuery()
    
    //把任务交给线程池处理
    _threadpool.addTask(std::bind(&WordQueryServer::doTaskThread,this,conn,msg));
}

void WordQueryServer::onClose(const TcpConnectionPtr & conn)
{
    cout<<"断开连接中"<<endl;
    cout<<conn->toString()<<"断开连接成功"<<endl;
}

using namespace placeholders ;

void WordQueryServer::start()
{
    _tcpserver.setConnectionCallback(std::bind(&WordQueryServer::onConnection,this,_1));//this 就是WordQueryServer对象 
    _tcpserver.setMessageCallback(std::bind(&WordQueryServer::onMessage,this,_1));
    _tcpserver.setCloseCallback(std::bind(&WordQueryServer::onClose,this,_1));
    _tcpserver.start();
}

};//end of namespace hk
