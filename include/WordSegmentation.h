#pragma once
#include "cppjieba/Jieba.hpp"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

namespace  hk
{
const char * const DICT_PATH = 
    "/home/schalke04/miniSearchEngine/include/cppjieba/dict/jieba.dict.utf8";
const char * const HMM_PATH = 
    "/home/schalke04/miniSearchEngine/include/cppjieba/dict/hmm_model.utf8";
const char * const USER_DICT_PATH = 
    "/home/schalke04/miniSearchEngine/include/cppjieba/dict/user.dict.utf8";
const char * const IDF_PATH =
    "/home/schalke04/miniSearchEngine/include/cppjieba/dict/idf.utf8";
const char * const STOP_WORD_PATH =
    "/home/schalke04/miniSearchEngine/include/cppjieba/dict/stop_words.utf8";//分割词路径



class WordSegmentation
{
public:
    WordSegmentation()
    :_jieba(DICT_PATH,HMM_PATH,USER_DICT_PATH,IDF_PATH,STOP_WORD_PATH)
    {
        cout<<"cppjieba init !"<<endl;
    }
    ~WordSegmentation() 
    {
        cout<<"~WordSegmentation()"<<endl;
    }

    vector<string>  operator()(const char * src)
    {
        vector<string> words;
        _jieba.CutAll(src,words);//cut with hmm
        return words;
    }

private:
    cppjieba::Jieba _jieba;

};

}//end of namespace hk
