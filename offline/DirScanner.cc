#include "../include/DirScanner.h"

namespace  hk
{

VecFiles & DirScanner::files()
{
    return _vecFiles ;
}


void DirScanner::traverse(Configuration * pConfig,const string & dirname)
{  
   // cout<<"111"<<endl;
    auto configMap =pConfig->getConfigMap();
   // cout<<"222"<<endl;
    string dirpath = configMap[dirname]; 
   // cout<<"333"<<endl;
    DIR * d = opendir(dirpath.c_str());
    //cout<<"2222"<<endl;
    if(!d)
    {
        perror("opendir");
    }
    
    struct dirent * p ;

    while((p = readdir(d)))
    {
        if(!strcmp(p->d_name,".") || !strcmp(p->d_name,".."))
        {
            continue ;
        }
        string s = dirpath + p->d_name ;
       // cout<<s<<endl;   
        _vecFiles.push_back(s);
    }
    closedir(d);
}



void DirScanner::print()
{
    for(auto & filepath:_vecFiles)
    {
        cout<<filepath<<endl;
    }
}


DirScanner * DirScanner::_pDirScanner = nullptr ;
//懒汉模式

}//end of namespace hk
