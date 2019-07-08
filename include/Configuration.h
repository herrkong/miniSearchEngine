#pragma once
#include <stdlib.h>
#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>
#include <set>
#include <fstream>
using namespace std ;

namespace hk
{

    using ConfigMap = map<string,string>;
    using StopWordList = set<string> ;

class Configuration
{
public:
    static Configuration * getConfiguration(const string & configFilePath)
    {
        if(!_pConfiguration)
        {
           _pConfiguration = new Configuration() ;
          _pConfiguration->_configFilePath = configFilePath ;   
           atexit(destroy);
        }    
         return _pConfiguration ;
    }

    static void destroy()
    {
        if(_pConfiguration)
        {
            delete _pConfiguration ;
        }
    }

    void readConfig();//读取配置信息到map中

    ConfigMap  & getConfigMap();//拿到map
    
    StopWordList & getStopWordList();//拿到停用词集合
    
    void print();//打印配置文件信息

private:
    Configuration()
    //:_configFilePath(configFilePath)
    {
        cout<<"Configuration(const string & configFilePath)"<<endl;
    }
    ~Configuration() 
    {
        cout<<"~Configuration()"<<endl;
    }
    
private:
    static Configuration * _pConfiguration ; //单例模式
    string _configFilePath ;  //配置文件路径
    ConfigMap _configMap ;//配置文件内容
    StopWordList _stopWordList; //停用词词集    
};


}//end of namespace hk
