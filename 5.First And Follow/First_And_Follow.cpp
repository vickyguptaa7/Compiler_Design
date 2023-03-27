#include <iostream>
#include <math.h>
#include <vector>
#include <algorithm>
#include <fstream>
#include <set>
#include <map>
#include <string.h>
#include <sstream>
#include <queue>
using namespace std;

typedef long long ll;

int stoii(string s)
{
    bool isNeg = (s[0] == '-') ? true : false;
    int start = isNeg;
    int num = 0;
    for (int i = start; i < s.size(); i++)
    {
        num = num * 10 + (s[i] - '0');
    }
    return (isNeg) ? -num : num;
}
// -123

void cfgInputFromFile(map<char, vector<string>> &cfgList, char &startSymbol, map<char, set<char>> &firsts)
{
    string line;
    ifstream cfgFile("CFG.txt");
    if (cfgFile.is_open())
    {
        string word;
        getline(cfgFile, line);
        startSymbol = line[0];
        do
        {
            stringstream strm(line);
            while (strm >> word)
            {
                cfgList[word[0]].push_back(word.substr(3));
                for (int i = 3; i < word.size(); i++)
                {
                    if (word[i] >= 'a' && word[i] <= 'z')
                        firsts[word[i]].insert(word[i]);
                }
            }
        } while (getline(cfgFile, line));
        cfgFile.close();
    }
    else
    {
        cout << "Unable to open dfa file.\n";
    }
}

void Union(set<char> &s1, set<char> &s2, bool isAddEpsilon = true, bool isFollow = false)
{
    bool isEpsilonInS1 = s1.count('#');
    for (set<char>::iterator ele = s2.begin(); ele != s2.end(); ele++)
    {
        if (s1.count(*ele))
            continue;
        s1.insert(*ele);
    }
    if (isFollow && s1.count('#'))
    {
        s1.erase('#');
        return;
    }
    if (!isEpsilonInS1 && s2.count('#') && !isAddEpsilon)
    {
        s1.erase('#');
    }
}

void calculateFirstHelper(char src, char par, map<char, vector<string>> &cfgList, map<char, set<char>> &first)
{
    // terminal
    if ((src >= 'a' && src <= 'z') || src == '#')
    {
        // cout<<par<<" "<<src<<" \n";
        first[par].insert(src);
        return;
    }

    // variables
    for (int i = 0; i < cfgList[src].size(); i++)
    {
        for (int j = 0; j < cfgList[src][i].size(); j++)
        {
            if (src == par)
                continue;
            calculateFirstHelper(cfgList[src][i][j], src, cfgList, first);
            // bcoz when we recieve terminal we just end that childList (production)
            if (cfgList[src][i][j] >= 'a' && cfgList[src][i][j] <= 'z')
            {
                break;
            }
            // if its child epsilon then we don't need to proceed further
            if (cfgList[src][i][j] == '#')
                continue;
            Union(first[src], first[cfgList[src][i][j]], j + 1 == cfgList[src][i].size());
            // if we did not find a epsilon then we do not proceed for further children processing of the same childList (or same production)
            if (!first[cfgList[src][i][j]].count('#'))
                break;
            // cout<<src<<" "<<cfgList[src][i][j]<<" \n";
            // add the child first to the parent first
            // Union(first[src], first[cfgList[src][i][j]]);
        }
    }
}

void calculateFirst(map<char, vector<string>> &cfgList, map<char, set<char>> &firsts)
{
    for (map<char, vector<string>>::iterator src = cfgList.begin(); src != cfgList.end(); ++src)
    {
        calculateFirstHelper(src->first, '@', cfgList, firsts);
    }
}

void calculateFollow(map<char, vector<string>> cfgList, map<char, set<char>> &follows, map<char, set<char>> &firsts, char startSymbol)
{
    // add the $ to start symbol
    follows[startSymbol].insert('$');

    map<char, vector<char>> addToFollowList;
    for (map<char, vector<string>>::iterator rhs = cfgList.begin(); rhs != cfgList.end(); ++rhs)
    {
        vector<string> lhs = rhs->second;
        for (int i = 0; i < lhs.size(); i++)
        {
            for (int j = 0; j < lhs[i].size(); j++)
            {
                if (lhs[i][j] >= 'a' && lhs[i][j] <= 'z' || lhs[i][j] == '#')
                    continue;
                bool isEnded = false;
                for (int k = j + 1; k < lhs[i].size(); k++)
                {
                    Union(follows[lhs[i][j]], firsts[lhs[i][k]], false, true);
                    if (!firsts[lhs[i][k]].count('#'))
                    {
                        isEnded = true;
                        break;
                    }
                }
                if (!isEnded && lhs[i][j] != rhs->first)
                {
                    // if reach to end then we have to add the follow of rhs to that variable
                    addToFollowList[rhs->first].push_back(lhs[i][j]);
                    // storing it in reverse order
                }
            }
        }
    }
    // here toposort must be used to find the dependencies whom will calculated first

    map<char, int> inorder;
    for (map<char, vector<char>>::iterator it = addToFollowList.begin(); it != addToFollowList.end(); it++)
    {
        // adding the nodes to the inorder list
        inorder[it->first] = inorder[it->first];
        for (int i = 0; i < it->second.size(); i++)
        {
            inorder[it->second[i]]++;
        }
    }
    queue<char> que;
    for (map<char, int>::iterator it = inorder.begin(); it != inorder.end(); it++)
    {
        if (it->second == 0)
            que.push(it->first);
    }
    if (que.empty())
    {
        cout << "Circular Dependency!\n";
        return;
    }
    while (!que.empty())
    {
        char src = que.front();
        que.pop();
        for (int i = 0; i < addToFollowList[src].size(); i++)
        {
            inorder[addToFollowList[src][i]]--;
            if (inorder[addToFollowList[src][i]] == 0)
            {
                cout << src << " " << addToFollowList[src][i] << "\n";
                // union in reverse bcoz stored in reverse order
                Union(follows[addToFollowList[src][i]], follows[src], false, true);
                que.push(addToFollowList[src][i]);
            }
        }
    }
}

int main()
{
    char startSymbol;
    map<char, vector<string>> cfgList;
    map<char, set<char>> firsts;
    map<char, set<char>> follows;
    cfgInputFromFile(cfgList, startSymbol, firsts);
    calculateFirst(cfgList, firsts);
    calculateFollow(cfgList, follows, firsts, startSymbol);

    cout << "CFG LIST : \n";
    for (map<char, vector<string>>::iterator it = cfgList.begin(); it != cfgList.end(); it++)
    {
        cout << it->first << " ->";
        for (int i = 0; i < it->second.size(); i++)
        {
            cout << it->second[i] << " ";
        }
        cout << "\n";
    }
    cout << "\n\n";
    cout << "First : \n";
    for (map<char, set<char>>::iterator it = firsts.begin(); it != firsts.end(); it++)
    {
        if ((it->first >= 'a' && it->first <= 'z') || it->first == '#')
            continue;
        cout << it->first << " { ";
        for (set<char>::iterator pt = it->second.begin(); pt != it->second.end(); pt++)
            cout << *pt << " ";
        cout << "}\n";
    }
    cout << "\n\n";
    cout << "Follows : \n";
    for (map<char, set<char>>::iterator it = follows.begin(); it != follows.end(); it++)
    {
        cout << it->first << " { ";
        for (set<char>::iterator pt = it->second.begin(); pt != it->second.end(); pt++)
            cout << *pt << " ";
        cout << "}\n";
    }

    return 0;
}

/*
#Before calculating the first and follow functions, eliminate Left Recursion from the grammar, if present.#

S->AbB
S->cS
A->BA
A->a
B->bB
B->#

S->aBDh
B->cC
C->bC
C->#
D->EF
E->g
E->#
F->f
F->#

S->AaAb
S->BbBa
A->#
B->#

*/