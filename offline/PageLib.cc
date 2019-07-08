//#include "../include/Configuration.h"
//#include "../include/DirScanner.h"
#include "../include/RssReader.h"
#include "../include/PageLib.h"

//using namespace tinyxml2;

namespace hk
{

void PageLib::create()
{
    //拿到所有xml文档
    //cout<<"xxxx"<<endl;
    cout<<"开始读取xml文档"<<endl;
    auto  vecFiles = _dirScanner->files();
    cout<<"读取完成"<<endl;
    // cout<<"ppp"<<endl;
    cout<<"开始解析xml文档"<<endl;
    RssReader reader(vecFiles);
    // cout<<"qqq"<<endl;
    cout<<"解析完成"<<endl;
    cout<<"开始创建原始网页库"<<endl;
    reader.loadFeedFiles();
    //cout<<"fff"<<endl;
    reader.makePages(_vecPages);
   // cout<<"ddd"<<endl;
   cout<<"创建原始网页库完成"<<endl;
}

void PageLib::store()
{
    auto configMap = _conf->getConfigMap();
    string pageLibPath = configMap["ripepagelib"];
    string offsetlibPath = configMap["offsetlib"];

    ofstream ofsPage(pageLibPath.c_str());
    ofstream ofsOffset(offsetlibPath.c_str());

    if(!(ofsPage.good() && ofsOffset.good()))
    {
        cout<<"ofstream open error !"<<endl;
        return ;
    }

    for(size_t idx =0 ;idx != _vecPages.size();++idx)
    {
        int id = idx + 1;
        int length = _vecPages[idx].size();
        auto offset = ofsPage.tellp();
        ofsPage << _vecPages[idx];
        ofsOffset << id << '\t'<< offset <<'\t'<<length <<'\n';
        
        //除了写入文件 也写入map 233
        _offsetLib.insert(make_pair(id,make_pair(offset,length)));  
    }
   
    ofsPage.close();
    ofsOffset.close();
}


}//end of namespace hk
