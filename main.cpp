#include <iostream>
#include <map>
#include <set>
#include <unordered_map>
#include <ctime>
#include <sys/time.h>
#include <fstream>

using namespace std;
class DFANode : public unordered_map<uint8_t, DFANode *>
{
};
DFANode *DFA = NULL;
const char replaceWord = '*';

int initHashMap(const set<string> &keyword)
{
    DFA = new DFANode();
    DFANode *nowMap;
    DFANode *newWordMap;
    for (set<string>::iterator it = keyword.begin(); it != keyword.end(); it++)
    {
        const string key = *it;
        nowMap = DFA;
        for (uint i = 0; i < key.length(); i++)
        {
            uint8_t keychar = key[i];
            DFANode *wordMap = (*nowMap)[keychar];

            if (wordMap != NULL)
            {
                nowMap = wordMap;
            }
            else
            {
                newWordMap = new DFANode();
                (*newWordMap)[0] = NULL;
                (*nowMap)[keychar] = newWordMap;
                nowMap = newWordMap;
            }

            if (i == key.length() - 1)
            {
                (*nowMap)[0] = nowMap;
            }
        }
    }
    return 0;
}

int check(const string &txt, const int &beginIndex)
{
    bool flag = false;
    uint ansLen = 0;
    DFANode *nowMap = DFA;
    for (uint i = beginIndex; i < txt.length(); i++)
    {
        nowMap = (*nowMap)[txt[i]];
        if (nowMap != NULL)
        {
            if ((*nowMap)[0] == nowMap)
            {
                ansLen = i - beginIndex + 1;
                flag = true;
            }
        }
        else
        {
            break;
        }
    }
    if (!flag)
        ansLen = 0;

    return ansLen;
}

string shield(const string &txt)
{
    string ans(txt.length(), 0);
    uint pos_txt = 0;
    uint pos_ans = 0;
    for (uint i = 0; i < txt.length(); i++)
    {
        uint len = check(txt, i);
        if (len != 0)
        {
            ans.replace(pos_ans, i - pos_txt, txt.c_str() + pos_txt, i - pos_txt);
            pos_ans += i - pos_txt;
            ans[pos_ans++] = replaceWord;
            i += len;
            pos_txt = i;
        }
    }
    ans.replace(pos_ans, ans.length() - pos_ans, txt.c_str() + pos_txt);
    ans.reserve(pos_ans + txt.length() - pos_txt);
    return ans;
}

long long getCurrentTime()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000000 + tv.tv_usec;
}

int main()
{
    ifstream in, in2;
    set<string> keyword;
    in.open("./badword.txt");
    string keywordstr;
    while (in >> keywordstr)
    {
        keyword.insert(keywordstr);
    }
    long long initstart = getCurrentTime();
    initHashMap(keyword);
    long long initend = getCurrentTime();
    cout << "初始化耗时:" << initend - initstart << "μs" << endl;
    in2.open("./text.txt");
    string txt;
    getline(in2, txt);
    long long start = getCurrentTime();
    cout << shield(txt) << endl;
    long long end = getCurrentTime();
    cout << "查询耗时:" << end - start << "μs" << endl;
    return 0;
}
