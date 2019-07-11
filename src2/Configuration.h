#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <map>
#include <fstream>
#include <set>

using namespace std;

namespace  hk
{
    using ConfigMap = map<string,string>;
    using StopWordList = set<string>;

class Configuration
{
public:
    Configuration(const string & configPath);
    ~Configuration() 
    {
        cout <<"~Configuration()"<<endl;
    }
    
    void loadConfigMap(); //加载configMap 

    ConfigMap & getConfigMap();//拿到configMap
    
    void loadStopWordList(); //加载停用词集合

    StopWordList & getStopWordList();//拿到停用词集合

    void print(); //打印configMap的内容

private:
    ConfigMap _configMap ; //配置文件存储的配置信息 
    StopWordList _stopWordList ; //停用词集合
    string _configPath ; //配置文件的地址
        
};


}//end of namespace hk
