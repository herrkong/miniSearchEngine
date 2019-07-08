#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

namespace  hk
{


class WordQuery
{
public:
    WordQuery()
    {
        cout<<"WordQuery()"<<endl;
    }
    ~WordQuery()
    {
        cout<<"~WordQuery()"<<endl;
    }

    string & doQuery();

    void LoadLibrary();

private:
    vector<string> _PageLib;  //网页库
    unordered_map<int,pair<int,int>> _OffsetLib ; //偏移库
    unordered_map<string,pair<int,double>> _InvertIndexTable ; //倒排索引

};

}//end of namespace hk
