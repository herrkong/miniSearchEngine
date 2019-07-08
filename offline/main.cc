//#include "../include/Configuration.h"
//#include "../include/DirScanner.h"
#include "../include/PageLib.h"
#include "../include/PageLibPreProcessor.h"
using namespace hk;


int main()
{
   // string configpath =
       // "home/schalke04/miniSearchEngine/conf/myconf.conf";
    string configpath = "../conf/myconf.conf";
    Configuration * p = Configuration::getConfiguration(configpath);    
    p->readConfig();    
   // p->print();
    //指向Configuration的指针解引用就是一个Configuration对象了。
    //不需要Configuration对象作为DirScanner的成员了。
    //直接给traverse多传一个参数得了。233
    DirScanner * d = DirScanner::getDirScanner();
    //()优先级更高
   //(*d)();//可以
   // d->();//不行
   //去掉这个函数调用符重载了 
    d->traverse(p,"yuliao");
   // d->print();

    //生成网页库和偏移库    
    PageLib pagelib(p,d) ;
    pagelib.create();
    pagelib.store();
   // pagelib.print_top_2_pages();
   // pagelib.print_10_offsetLib();
    

   // WordSegmentation jieba;

   // cout<<"debug1"<<endl;
    PageLibPreProcessor server(p);
   // cout<<"debug2"<<endl;
    server.doProcess();  
   // cout<<"debug3"<<endl;
    return 0;
}

