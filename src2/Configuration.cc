#include "Configuration.h"

namespace hk
{

//优化的时候可以将配置类写为单例 
//这里也可以将读取信息和返回信息分开
//就像工作线程和IO线程分开一样
Configuration::Configuration(const string & configPath)
:_configPath(configPath)
{
    loadConfigMap();//在构造的时候就加载configMap
    //加载配置信息完成
    loadStopWordList();
    //加载停用词集合完成
    cout << "Configuration(const string & configPath)"<<endl;
}


void Configuration::loadConfigMap()
{
    ifstream ifs(_configPath);//文件流
    string s1,s2 ;
    while(ifs>>s1>>s2)
    {
        _configMap.insert(make_pair(s1,s2));
    }
    ifs.close();    
}

ConfigMap & Configuration::getConfigMap()
{
    return _configMap;
}

void Configuration::loadStopWordList()
{
    string stopWordListPath = _configMap["stopword"];
    ifstream ifs(stopWordListPath);
    if(!ifs)
    {
        cout<<"StopWord ifstream open error"<<endl;
    }
    string word ;

    while(getline(ifs,word))
    {
        _stopWordList.insert(word);
    }
}

StopWordList & Configuration::getStopWordList()
{
    
    return  _stopWordList ; 
}


void Configuration::print()
{
    for(auto & pair:_configMap)
    {
        cout<<pair.first<<" "<<pair.second<<endl;
    }

}



}//end of namespace hk
