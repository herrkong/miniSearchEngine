#pragma once
#include "Configuration.h"
#include "WordSegmentation.h"
#include "WebPage.h"
#include <iostream>
#include <vector>
#include <unordered_map>
#include <json/json.h>

using namespace std;

namespace  hk
{

    using  PageLib = unordered_map<int,WebPage> ;
    using  OffsetLib = unordered_map<int,pair<int,int>>;
    using  InvertIndexTable = unordered_map<string,set<pair<int,double>>>;

class WordQuery
{
public:
    WordQuery(Configuration & config)
    :_config(config)
    {
        LoadLibrary();
        cout<<"WordQuery()"<<endl;
    }
    ~WordQuery()
    {
        cout<<"~WordQuery()"<<endl;
    }


    string  doQuery(const string & queryWord);//执行查询 返回结果
    
    //计算查询词的权重值
    vector<double> getQueryWordsWeightVector(vector<string> & queryWords);
    
    //执行查询
    bool executeQuery(const vector<string> & queryWords,
                      vector<pair<int,vector<double>>> & resultVec);

    string   createJson(vector<int> & docIdVec,const vector<string> & queryWords);

    string  returnNoAnswer();

    void LoadLibrary(); //加载库文件

private:
    Configuration & _config ;//配置信息
    WordSegmentation _jieba ;//jieba库分词对象
    PageLib _PageLib;  //网页库
    OffsetLib _OffsetLib ; //偏移库
    InvertIndexTable  _invertIndexTable ; //倒排索引

};

}//end of namespace hk
