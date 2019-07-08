#pragma once
#include "Configuration.h"
#include "WordSegmentation.h"
#include "WebPage.h"

namespace hk
{   

    using OffsetLib = unordered_map<int,pair<int,int>>;
    using InvertIndexTable = unordered_map< string,vector< pair<int ,double> > >;

class PageLibPreProcessor
{
public:
    PageLibPreProcessor(Configuration * conf)
    :_conf(conf)
   // ,_jieba(jieba)
    {
        cout<<"PageLibPreProcessor(Configuration * conf)"<<endl;
    }
    ~PageLibPreProcessor(){ cout<<"~PageLibPreProcessor()"<<endl; }
    
    void doProcess();  //执行预处理
    void readinfoFromFile(); 
    //根据配置信息读取网页库和位置偏移库的内容
    void cutRedundantPages();
    //对冗余的网页进行去重
    void buildInvertIndexTable();
    //创建倒排索引
    void storeOnDisk();
    //将经过预处理之后的网页库，位置偏移库和倒排索引写回到磁盘上


private:
    Configuration * _conf;  //配置文件对象的引用
    WordSegmentation _jieba ; //分词对象
    vector<WebPage> _pageLib ; //网页库的容器对象
    OffsetLib  _offsetLib ; //网页偏移库对象
    InvertIndexTable _invertIndexTable ; //倒排索引表对象    
};



}//end of namespace hk
