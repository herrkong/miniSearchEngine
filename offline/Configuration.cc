#include "../include/Configuration.h"

namespace  hk
{

void Configuration::readConfig()
{
   // cout<<"111"<<endl;
    ifstream ifs(_configFilePath);
    string s1,s2 ;
    while(ifs>>s1>>s2)
    {
       // cout<<"222"<<endl;
        _configMap.insert(make_pair(s1,s2));
       // cout<<s1<<" "<<s2<<endl;
    }
   // cout<<"333"<<endl;
    ifs.close();
}


ConfigMap & Configuration::getConfigMap()
{
    return _configMap ;

}

//set<string>
StopWordList & Configuration::getStopWordList()
{
   // if(_stopWordList.size() > 0)
       // return _stopWordList ;
    string stopWordFilePath = _configMap["stopword"];
    ifstream ifs(stopWordFilePath.c_str());
    if(!ifs)
    {
        cout<<"StopWord ifstream open error!"<<endl;
    }
    
    string word ;

    while(getline(ifs,word))
    {
        _stopWordList.insert(word);
    }
    
    return _stopWordList ;
}

void Configuration::print()
{
    for(auto & pair:_configMap)
    {
        cout<<pair.first<<" "<<pair.second<<endl;
    }
}

//带有参数的构造函数使用懒汉模式
Configuration * Configuration::_pConfiguration = nullptr ;
//懒汉模式


//Configuration * Configuration::_pConfiguration = getConfiguration("../conf/myconf.conf") ;
//Configuration * Configuration::_pConfiguration = new Configuration("../conf/myconf.conf");
//饿汉模式

}//end of namespace hk
