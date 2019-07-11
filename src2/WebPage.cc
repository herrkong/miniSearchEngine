#include "WebPage.h"

namespace  hk
{

struct WordFreqCompare
{
    bool operator()(const pair<string, int> & left, const pair<string, int> & right)
    {
        if(left.second < right.second)
        {   return true;    }
        else if(left.second == right.second && left.first > right.first)
        {   return true;    }
        else
        {   return false;   }

    }

};

WebPage::WebPage(string & doc,Configuration &  config,WordSegmentation & jieba )
{
   // cout<<"WebPage()"<<endl;
    _topWords.reserve(10);
    processDoc(doc,config,jieba);
}

string WebPage::summary(const vector<string> & queryWords)
{
    vector<string> summaryVec ;

    istringstream iss(_docContent);
    string line ;
    while(iss>>line)
    {
        for(auto word : queryWords)
        {
            if(line.find(word) != string::npos)
            {
                summaryVec.push_back(line);
                break ;
            }
        }

        if(summaryVec.size() >= 5)
        {
            break ;
        }
    }

    string summary ;
    for(auto line:summaryVec)
    {
        summary.append(line).append("\n");
    }
    return summary ;
}



void WebPage::processDoc(const string & doc,Configuration & _config ,WordSegmentation & jieba)
{
    string docIdHead = "<docid>";
    string docIdTail = "</docid>";
    string docUrlHead = "<link>";
    string docUrlTail = "</link>";
    string docTitleHead = "<title>";
    string docTitleTail = "</title>";
    string docContentHead = "<content>";
    string docContentTail = "</content>";

    //提取文档的docid std::find
    int bpos = doc.find(docIdHead);
    int epos = doc.find(docIdTail);
    //string substr取子串
    string docId = doc.substr(bpos + docIdHead.size(),
                              epos - bpos -docIdHead.size());
    _docId = atoi(docId.c_str());

    //title
    bpos = doc.find(docTitleHead);
    epos = doc.find(docTitleTail);
    _docTitle = doc.substr(bpos + docTitleHead.size(),
                           epos -bpos - docTitleHead.size());
    //cout<<"=========="<<endl;
   // cout<<endl<<_docTitle<<endl;

    //content
    bpos = doc.find(docContentHead);
    epos = doc.find(docContentTail);
    _docContent = doc.substr(bpos + docContentHead.size(),
                             epos - bpos - docContentHead.size());

    //cout<<"=========="<<endl;
   // cout<<endl<<_docContent<<endl;

    //分词
    vector<string> wordsVec = jieba(_docContent.c_str());
    StopWordList & stopWordList = _config.getStopWordList();
    calcTopK(wordsVec,TOPK_NUMBER,stopWordList);

}

//计算权重
void WebPage::calcTopK(vector<string> & wordsVec,int k,set<string> & stopWordList)
{
    for(auto iter = wordsVec.begin();iter != wordsVec.end();++iter)
    {
        auto sit = stopWordList.find(*iter);
        if(sit == stopWordList.end())
        {
            ++_wordsMap[*iter];
        }
    }

    priority_queue< pair<string,int>,
        vector< pair<string,int> > ,WordFreqCompare  >
    wordFreqQue(_wordsMap.begin(),_wordsMap.end());

    while(!wordFreqQue.empty())
    {
        string top = wordFreqQue.top().first ;
        wordFreqQue.pop();
        if(top.size() == 1 && (static_cast<unsigned int>(top[0]) == 10
                               || static_cast<unsigned int>(top[0])== 13))
        {
            continue ;
        }
        _topWords.push_back(top);
        if(_topWords.size() >= static_cast<size_t>(k))
        {
            break;
        }
    }
}

//判断两篇文档是否相同

bool operator == (const WebPage & lhs,const WebPage & rhs)
{
    int commNum = 0 ;
    auto lIter = lhs._topWords.begin();
    for(;lIter !=lhs._topWords.end();++lIter)
    {
        commNum += count(rhs._topWords.begin(),rhs._topWords.end(),*lIter);
    }   

        int lhsNum = lhs._topWords.size();
        int rhsNum = rhs._topWords.size();
        int totalNum = lhsNum < rhsNum ? lhsNum : rhsNum ;

        if(static_cast<double>(commNum) / totalNum > 0.75)
        {
            return true ;
        }
        else 
        {
            return false ;
        }
 }



//对文档按照docId进行排序

bool operator < (const WebPage & lhs,const WebPage & rhs)
{
    if(lhs._docId < rhs._docId)
    {
        return true ;
    }
    else 
    {
        return false ;
    }
}


}//end of namespace hk
