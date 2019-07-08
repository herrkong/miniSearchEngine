#pragma once
#include "Configuration.h"
#include "dirent.h"
#include  <string.h>
namespace  hk
{

    using VecFiles = vector<string> ;

class Configuration ;

class DirScanner
{
public:
    //参数的赋值是在单例的创建里面进行的，
    //这里是因为违背了单一职责原则。
    //修改的方法也简单，这里只需要将
    //Configuration & config从DirScanner单例的创建中分离出来。
    static DirScanner * getDirScanner()
    {
        if(!_pDirScanner)
        {
            _pDirScanner = new DirScanner() ;
            atexit(destroy);
        }
        return _pDirScanner ;
    }

    static void destroy()
    {
        if(_pDirScanner)
        {
            delete  _pDirScanner ;
        }
    }

    VecFiles & files();//获取语料文件路径集合
    //多加一个参数
    void traverse(Configuration * pConfig,const string & dirpath);//获取某一个目录下所有文件 
    
    //不用这个了
    //void operator()();//重载函数调用符,调用traverse函数。
    
    void print();

private:

    //Configuration和DirScanner两个类都使用了单例模式
    //在构造DirScanner的时候又想使用Configuration.这就可能出问题了！
    //在Configuration开始之前 DirScanner就先构造了.
    //而这时还没有Configuration.
    DirScanner(){ cout<<"DirScanner()"<<endl; }
   // :_conf(conf)
   // {
    //    cout<<"DirScanner(Configuration & conf)"<<endl;
   // }

    ~DirScanner() 
    {
        cout<<"~DirScanner()"<<endl;
    }

private:
    static DirScanner * _pDirScanner ;//单例模式
    VecFiles _vecFiles ;//存放每个语料文件的绝对路径
   // Configuration & _conf ;
   //不用把它的对象作为成员，而将指向它的指针作为成员 
   //曲线救国
   // Configuration * _pConfiguration ;
   // 都不需要了。
};


}//end of namespace hk
