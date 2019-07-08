#pragma once
#include "tinyxml2.h"
#include <string>
#include <iostream>
#include <vector>
#include <memory>
using std::endl;
using std::cout;
using std::string;
using std::vector;
using std::shared_ptr;

using namespace tinyxml2 ;

namespace  hk
{

struct RssItem
{
    string _title;
    string _link ;
    string _pubDate;
    string _content ;
};

struct RssData
{
    string _version;
    string _title;
    string _link;
    string _description;
    string _language;
    vector< shared_ptr<RssItem> > _items;
};


class RssReader
{
public:
    RssReader(vector<string> & files);
    ~RssReader();

    void loadFeedFiles();

    void loadFeedFile(const string & filename);//一个xml的绝对路径
    void initWithRssString(const string & rss);
    void makePages(vector<string> & pages);
    
private:
    void parseRss(XMLDocument & doc);
    //string debug();
    //void dump(const string & filename);
    
private:
    vector< shared_ptr<RssItem> >  _items;
    vector<string> & _files;

};


}//end of namespace hk
