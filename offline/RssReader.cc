#include "../include/RssReader.h"
#include <regex>
//#include <boost/regex.hpp>
#include <sstream>
#include <fstream>
#include <iostream>


using namespace std;

namespace hk
{

RssReader::RssReader(vector<string> & files)
:_files(files)
{
    cout<<"RssReader()"<<endl;
}

RssReader::~RssReader()
{
    cout<<"~RssReader()"<<endl;
}
//处理所有rss文件
void RssReader::loadFeedFiles()
{
    for(size_t idx = 0 ;idx != _files.size();++idx)
    {
        loadFeedFile(_files[idx]);
       // cout<<"已加载"<<idx + 1 <<" 个rss文件"<<endl;
    }
}

//处理一个rss文件
void RssReader::loadFeedFile(const string & filename)
{
    XMLDocument doc ;
    doc.LoadFile(filename.c_str());
   // cout<<"LoadFile: "<<filename<<endl;
    XMLError errorID = doc.ErrorID();
    if(errorID)
    {
        cout<<"XMLDocument LoadFile error!"<<endl;
        return ;
    }
    else
    {
        parseRss(doc);//解析这篇xml文档
    }

}


void RssReader::makePages(vector<string> & pages)
{
   // cout<<"pages's size = "<< _items.size() <<endl;
    for(size_t idx = 0 ; idx != _items.size();++idx)
    {
        ostringstream oss ;
        oss<<"<doc>\n"
            <<" <docid>"<<(idx+1)<<"</docid>\n"
            <<" <title>"<<_items[idx]->_title <<"</title>\n"
            <<" <link>"<<_items[idx]->_link<<"</link>\n"
            <<" <content>"<<_items[idx]->_content<<"</content>\n"
            <<" </doc>\n";
        string page = oss.str();
        pages.push_back(page);
    }
}


//解析一篇xml文档
void RssReader::parseRss(XMLDocument & doc)
{
   // cout<<"parseRss(XMLDocument & doc)"<<endl;
    XMLElement * root = doc.FirstChildElement();
    //<channel> 
    XMLElement * channel = root->FirstChildElement("channel");
    // <item>
    XMLElement * item = channel->FirstChildElement("item");
    for(;item;item = item->NextSiblingElement())
    {
        shared_ptr<RssItem> rssItemPtr(new RssItem);
        const char * itemTitle = item->FirstChildElement("title")->GetText();
        const char * itemLink = item->FirstChildElement("link")->GetText();
        const char * itemDescription = item->FirstChildElement("description")->GetText();
    
        const char * iContent = nullptr ;
        XMLElement * contentEncoded = item->FirstChildElement("content:encoded");
        if(contentEncoded)
        {
            iContent = contentEncoded->GetText();
        }
        else
        {
            iContent = itemDescription;
        }
        
         string szReg = "<.*?>";
        
         regex fmt(szReg);
        
         string content = regex_replace(string(iContent),fmt,string(""));
        
        rssItemPtr->_title = itemTitle ? itemTitle : string("");
        rssItemPtr->_link = itemLink ? itemLink : string("");
        rssItemPtr->_content = content ;

        _items.push_back(rssItemPtr);
    
    }
}

}//end of namespace hk

