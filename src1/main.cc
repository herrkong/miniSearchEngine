#include "WordQuery.h"
using namespace hk;

int main()
{

//测试Configuration类

    string configPath = "../conf/myconf.conf";
    Configuration config(configPath);
    config.loadConfigMap();
   // config.print(); 


//测试业务类WordQuery
    
    WordQuery wq(config);
    string queryWord = "海洋";
    Json::Reader reader ;
    Json::Value root ; 
    root  = wq.doQuery(queryWord);
    cout << "查询结果为:"<<endl;
    cout << root.asString()<<endl;

    return 0;
}

