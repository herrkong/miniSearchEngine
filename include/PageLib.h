#pragma once
#include "../include/Configuration.h"
#include "../include/DirScanner.h"
#include <sstream>

namespace  hk
{

class PageLib
{
public:
    PageLib(Configuration * conf,DirScanner * dirScanner)
    :_conf(conf)
     ,_dirScanner(dirScanner)
    {
        cout<<"PageLib(Configuration * conf,DirScanner * dirScanner)"<<endl;
    }
    ~PageLib() {cout<<"~PageLib()"<<endl;}
    
    void create();  //创建网页库
    void store();  //存储网页库和位置偏移库
    //打印两篇格式化后的文章
    void print_top_2_pages()
    {
        for(size_t idx =0 ;idx<2;idx++)
        {
            cout<<_vecPages[idx]<<endl;
        }
    }

    //打印10条文档在网页库中的位置信息
    void print_10_offsetLib()
    {
        auto it = _offsetLib.begin();

        for(size_t idx =0 ;idx <10;idx++)
        {
            cout<<it->first<<" "<<it->second.first<<" "<<it->second.second<<endl;
            it++;
        }
    }

private:
    Configuration * _conf ; //拿配置文件 存储地址
    DirScanner * _dirScanner; //拿所有xml文档
    vector<string> _vecPages ; //存放格式化之后的网页的容器
    map<int,pair<int,int> >   _offsetLib; 
    //存放每篇文档在网页库的位置信息

};


}//end of namespace hk
