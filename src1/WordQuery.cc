#include "WordQuery.h"

namespace  hk
{

//相似性比较
struct SimilarityCompare
{
    SimilarityCompare(vector<double> & base)
    :_base(base)
    {}
    
    bool operator()(const pair<int,vector<double>> & lhs,
                    const pair<int,vector<double> > & rhs)
    {
        //都与基准向量进行计算
        double lhsCrossProduct = 0 ;
        double rhsCrossProduct = 0 ;
        double lhsVectorLength = 0 ;
        double rhsVectorLength = 0 ;
        
        for(size_t index = 0 ;index != _base.size();++index)
        {
            lhsCrossProduct +=(lhs.second)[index] * _base[index];
            rhsCrossProduct +=(rhs.second)[index] * _base[index];
            lhsVectorLength += pow((lhs.second)[index],2);
            rhsVectorLength += pow((rhs.second)[index],2);
        }
        
        if(lhsCrossProduct /sqrt(lhsVectorLength)
           < rhsCrossProduct/sqrt(rhsVectorLength))
        {
            return false ;
        }
        else
        {
            return true ;
        }
    }

    vector<double> _base;

};


string  WordQuery::doQuery(const string & queryWord )
{
    //对输入字符串进行分词
    vector<string>  queryWords;
    if(queryWord.size()>0)
    {
        queryWords = _jieba(queryWord.c_str());
    }

    for(auto & item:queryWords)
    {
        cout<<item<<endl;
        auto uit = _invertIndexTable.find(item);
        if(uit == _invertIndexTable.end())
        {
            cout<<"can not found"<<item <<endl;
            return returnNoAnswer();
        }
    }
    cout<<endl;

    //计算每个词的权重
    vector<double> weightList = getQueryWordsWeightVector(queryWords);
    
    SimilarityCompare similarityCmp(weightList);
    
    //执行查询
    vector<pair<int,vector<double>>>  resultVec ;
    if(executeQuery(queryWords,resultVec))
    {
        stable_sort(resultVec.begin(),resultVec.end(),similarityCmp);
        vector<int> docIdVec ;

        for(auto item : resultVec)
        {
            docIdVec.push_back(item.first);
        }
        return createJson(docIdVec,queryWords);
        //将查询结果封装成Json数据
    }
    else
    {
        return returnNoAnswer();
    }
}


string  WordQuery::returnNoAnswer()
{
    Json::Value root ;
    Json::Value arr ;

    Json::Value elem ;

    elem["title"] = "404,not found";
    elem["summary"] = "不好意思,没找到,请重新输入。";
    elem["url"] = "";

    arr.append(elem);
    root["files"] = arr ;
    Json::FastWriter fw;
    string str = fw.write(root);
    return str;

}

//计算查询词分词集合的权重
vector<double> WordQuery::getQueryWordsWeightVector(vector<string> & 
                                                    queryWords)
{
    //统计词频 
    map<string,int> wordFreqMap ;
    for(auto item : queryWords)
    {
        ++wordFreqMap[item];
    }

    //计算权重
    vector<double> weightList ;
    double weightSum = 0 ;
    int totalPageNum = _OffsetLib.size();
    
    for(auto & item:queryWords)
    {
        int df = _invertIndexTable[item].size();
        double idf = log(static_cast<double>(totalPageNum) / df+ 1)/log(2);
        int tf = wordFreqMap[item];
        double w = idf * tf ;
        weightSum +=pow(w,2);
        weightList.push_back(w);
    }

    //归一化
    
    for(auto & item :weightList)
    {
        item /= sqrt(weightSum);
    }

    cout << "weightList's elem: ";

    for(auto & item:weightList)
    {
        cout<<item<<"\t";
    }
    cout<<endl;
    return weightList ;
}


//执行查询
bool WordQuery::executeQuery(const vector<string> & queryWords,
                      vector<pair<int,vector<double>>> & resultVec)
{

    cout<<"开始查询"<<endl;
    if(queryWords.size() == 0)
    {
        return false ;
    }

    typedef set<pair<int,double> >::iterator setIter ;
    
    vector<pair<setIter,int>> iterVec ; //保存需要求取交集的迭代器

    int minIterNum = 0x7FFFFFFF ;//这是在干叼
    for(auto item:queryWords)
    {
        int sz = _invertIndexTable[item].size();
        if(sz == 0)
        {
            return false ;
        }
        if(minIterNum >sz)
        {
            minIterNum = sz ;
        }
        iterVec.push_back(make_pair(_invertIndexTable[item].begin(),0));
    }
    cout<<"minIterNum = "<<minIterNum <<endl;
    
    bool isExiting = false ;

    while(!isExiting)
    {
        size_t idx = 0 ;
        for(;idx!=iterVec.size() -1 ;++idx)
        {
            if((iterVec[idx].first)->first !=
               iterVec[idx+1].first->first)
                break ;
        }
        
        if(idx == iterVec.size() -1)
        {
            //找到相同的docid
            vector<double> weightVec ;
            int docid = iterVec[0].first->first ;
            for(idx = 0 ;idx !=iterVec.size();++idx)
            {
                weightVec.push_back(iterVec[idx].first->second);
                ++(iterVec[idx].first);//相同时,每个迭代器++
                ++(iterVec[idx].second);//同时记录迭代器++ 的次数
                if(iterVec[idx].second == minIterNum)
                {
                    isExiting = true ;
                }

            }//for
            resultVec.push_back(make_pair(docid,weightVec));
        }//if
        else
        {
            //找到最小的docid,并将其所在的iter++
            int minDocId = 0x7FFFFFFF ;
            int iterIdx ; //保存minDocId的迭代器位置
            for(idx =0 ;idx !=iterVec.size();++idx)
            {
                if(iterVec[idx].first->first < minDocId)
                {
                    minDocId = iterVec[idx].first->first ;
                    iterIdx = idx ;
                }
            }//for

            ++(iterVec[iterIdx].first);
            ++(iterVec[iterIdx].second);
            if(iterVec[iterIdx].second == minIterNum)
            {
                isExiting = true ;
            }
        }
    }
    
    return true ;
}

//不可以返回局部变量的引用或者指针
string  WordQuery::createJson(vector<int> & docIdVec,
                                const vector<string> & queryWords)
{
    Json::Value root ;
    Json::Value arr ;

    int cnt = 0 ;
    for(auto id : docIdVec)
    {
        string summary = _PageLib[id].summary(queryWords);
        string title = _PageLib[id].getTitle();
        string url = _PageLib[id].getUrl();

        Json::Value elem ;
        elem["title"] = title ;
        elem["summary"] = summary ;
        elem["url"] = url ;
        arr.append(elem);
        if(++cnt == 100)//最多记录100条
            break;
    }
    root["files"] = arr ;
    Json::FastWriter fw ;
    string str = fw.write(root);
    return str ;

}




void WordQuery::LoadLibrary()
{
    //先拿configMap
    auto ConfigMap = _config.getConfigMap();
    string OffsetLibPath = ConfigMap["newoffsetlib"];
    string PageLibPath = ConfigMap["newpagelib"]; 
    string InvertIndexTablePath = ConfigMap["invertindexlib"];

    //文件流
    ifstream IfsOffset(OffsetLibPath.c_str());
    ifstream IfsPage(PageLibPath.c_str());
    
    if(!(IfsPage.good() && IfsOffset.good()))
    {
        cout<<"offset or  page  open error"<<endl;
    }

    cout<<"开始加载网页库和偏移库"<<endl;
    string line ;
    int docid,offset,doclen;

    while(getline(IfsOffset,line))
    {
        stringstream ss(line);
        ss>>docid >>offset >>doclen ;
        string doc;
        doc.resize(doclen,' ');
        IfsPage.seekg(offset,IfsPage.beg);
        IfsPage.read(&*doc.begin(),doclen);

        WebPage webPage(doc,_config,_jieba);
        _PageLib.insert(make_pair(docid,webPage));
        
        _OffsetLib.insert(make_pair(docid,make_pair(offset,doclen)));
    }
    IfsPage.close();
    IfsOffset.close();
    cout<<"加载网页库和偏移库完成"<<endl;

    cout<<"开始加载倒排索引"<<endl;
    ifstream IfsInvertIndexTable(InvertIndexTablePath.c_str());
    if(!IfsInvertIndexTable.good())
    {
        cout<<"open InvertIndexTable error"<<endl;
    }
    string word ;
    double weight ;
    while(getline(IfsInvertIndexTable,line))
    {
        stringstream ss(line);
        ss>>word ;
        set<pair<int,double>>  setID ;
        while(ss>>docid>>weight)
        {
           setID.insert(make_pair(docid,weight));
        }
        _invertIndexTable.insert(make_pair(word,setID));

    }
    IfsInvertIndexTable.close();

    cout<<"加载倒排索引完成"<<endl;


}




}//end of namespace hk


