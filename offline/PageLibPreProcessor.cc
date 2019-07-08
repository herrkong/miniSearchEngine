#include "../include/PageLibPreProcessor.h"


namespace hk

{

void PageLibPreProcessor::doProcess()
{
    readinfoFromFile();//从文件读取信息
    time_t t1 = time(NULL);
    cutRedundantPages(); //去除重复网页
    buildInvertIndexTable();//建立倒排索引
    time_t t2 = time(NULL);
    printf("preprocess time:%ld s\n",(t2 - t1));

    //cout<<"vvvv"<<endl;
    storeOnDisk();//存回磁盘
    //cout<<"xxxx"<<endl;
    time_t t3 = time(NULL);
    printf("store time: %ld s\n",(t3 -t2));

}

//从文件中读信息
void PageLibPreProcessor::readinfoFromFile()
{
    //先拿ConfidMap
    auto configMap = _conf->getConfigMap();
    string pageLibPath = configMap["ripepagelib"];
    string offsetLibPath = configMap["offsetlib"];

    //网页库文件流和偏移库文件流
    ifstream pageIfs(pageLibPath.c_str());
    ifstream offsetIfs(offsetLibPath.c_str());

    if(!(pageIfs.good() && offsetIfs.good()))
    {
        cout<<" page or offset lib open error"<<endl;
    }
    
    string line ;
    int docId,docOffset,docLen;

    while(getline(offsetIfs,line))
    {
        //字符串流->docId docOffset docLen
        stringstream ss(line);
        ss>>docId>>docOffset>>docLen;

        //string resize  fstream  seekg
        string doc ;
        doc.resize(docLen,' ');
        pageIfs.seekg(docOffset,pageIfs.beg);
        pageIfs.read(&*doc.begin(),docLen);

        WebPage webPage(doc,_conf,_jieba);
        _pageLib.push_back(webPage);
        _offsetLib.insert(make_pair(docId,make_pair(docOffset,docLen)));
    }
}
//网页去重
void PageLibPreProcessor::cutRedundantPages()
{
    for(size_t i =0 ;i != _pageLib.size() -1 ; ++i)
    {
        for(size_t j = i+1 ; j!=_pageLib.size();++j)
        {
            if(_pageLib[i] == _pageLib[j])
            {
                //把重复的和最后一篇网页交换再pop_back()
                _pageLib[j] = _pageLib[_pageLib.size() -1];
                _pageLib.pop_back();
                --j;
            }
        }//for
    }//for
}

//建立倒排索引
void PageLibPreProcessor::buildInvertIndexTable()
{
    cout<<"开始建立倒排索引"<<endl;
    for(auto page : _pageLib)
    {
        //WebPage 词频
        map<string,int> & wordsMap = page.getWordsMap();
        for(auto wordFreq : wordsMap)
        {
            _invertIndexTable[wordFreq.first].push_back(
                make_pair(page.getDocId(),wordFreq.second));
                //文档id和词频
        }
    }

    //计算每篇文档中的词的比重，并归一化
    map<int,double> weightSum ;
    //保存每一篇文档中所有词的权重平方和 int为docId 
    
    int totalPageNum = _pageLib.size();
    for(auto & item : _invertIndexTable)
    {
        //DF 某个词在所有文章中出现的次数
        int df = item.second.size();
        //求关键词item.first的逆文档频率IDF
        double idf = log(static_cast<double>(totalPageNum) /df + 1)/log(2);
       

        for(auto & sitem : item.second)
        {
            
            double weight = sitem.second * idf ;
            weightSum[sitem.first] += pow(weight,2);
            sitem.second = weight ;
        }

    }//for

    //归一化处理
    for(auto & item : _invertIndexTable)
    {
        for(auto & sitem : item.second)
        {
            sitem.second = sitem.second / sqrt(weightSum[sitem.first]);
        }
    }

    cout<<"倒排索引建立完毕"<<endl;

}


void PageLibPreProcessor::storeOnDisk()
{
    cout<<"开始将新的网页库和偏移库写入磁盘"<<endl;
    sort(_pageLib.begin(),_pageLib.end());

    ofstream ofsPageLib(_conf->getConfigMap()["newpagelib"].c_str());
    ofstream ofsOffsetLib(_conf->getConfigMap()["newoffsetlib"].c_str());

    if(!ofsPageLib.good() || ! ofsOffsetLib.good())
    {
        cout<<"new page or offset lib ofstream open error"<<endl;
    }
    
    for(auto & page : _pageLib)
    {
        int id = page.getDocId();
        int length = page.getDoc().size();
        ofstream::pos_type offset = ofsPageLib.tellp();
        ofsPageLib << page.getDoc();

        ofsOffsetLib << id <<'\t' <<offset <<'\t'
            <<length << '\n';
    }

    ofsPageLib.close();
    ofsOffsetLib.close();
    cout<<"新网页库和偏移库写入完毕"<<endl;
    //InvertIndexTable
    cout<<"开始写入倒排索引"<<endl;
    ofstream ofsInvertIndexTable(_conf->getConfigMap()["invertindexlib"]);
    if(!ofsInvertIndexTable.good())
    {
        cout<<"invert index table ofstream open error!"<<endl;
    }
    
    for(auto & item : _invertIndexTable)
    {
        ofsInvertIndexTable << item.first << '\t';
        for(auto sitem : item.second)
        {
            ofsInvertIndexTable << sitem.first << '\t'<< sitem.second<<'\t';
            
        }
        ofsInvertIndexTable<<endl;
    }
    ofsInvertIndexTable.close();
    cout<<"倒排索引写入完毕"<<endl;

}

}//end of namespace hk
