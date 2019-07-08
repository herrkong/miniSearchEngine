#pragma once
#include "Configuration.h"
#include "WordSegmentation.h"
namespace  hk
{

class WebPage
{
    const static int TOPK_NUMBER = 10 ;
public:
    WebPage(string & doc,Configuration * conf,WordSegmentation & jieba);
    
    ~WebPage() 
    {
       // cout<<"~WebPage()"<<endl;
    }


    //获取文档的docid
    int getDocId()
    {
        return _docId;
    }  

    //获取文档
    string getDoc()
    {
        return _doc;
    } 
    
    //获取文档的词频统计map
    map<string,int> & getWordsMap()
    {
        return _wordsMap;
    }  
    
    //对格式化文档进行处理
    void processDoc(const string & doc,Configuration * conf, WordSegmentation & wordSegmentation);
    //求取文档的topk词集
    void calcTopK(vector<string> & wordsVec,int k,
                  StopWordList & stopWordList);

    friend bool operator==(const WebPage & lhs,const WebPage & rhs);

    friend bool operator<(const WebPage & lhs,const WebPage & rhs);


private:
    string _doc ; //整篇文档,包括xml在内
    int _docId;  //文档id
    string _docTitle; //文档标题
    string _docUrl ;  //文档URL 
    string _docContent ; //文档内容
    string _docSummary ; //文档摘要 需要自动生成,不是固定的
    vector<string> _topWords ; //词频最高的前10个词语
    map<string,int>  _wordsMap ; 
    //保存每篇文档的所有词语和词频，不包括停用词
    Configuration * _conf;
};



}//end of namespace hk
