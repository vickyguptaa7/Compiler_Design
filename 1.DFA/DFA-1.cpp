#include <iostream>
#include <math.h>
#include <vector>
#include <algorithm>
#include <fstream>
#include <set>
#include <map>
#include <string.h>
using namespace std;

typedef long long ll;

int stoi(string s)
{
	bool isNeg=(s[0]=='-')?true:false;
	int start=isNeg;
	int num=0;
	for(int i=start;i<s.size();i++)
	{
		num=num*10+(s[i]-'0');
	}
	return (isNeg)?-num:num;
}
// -123 

void dfaInputFromFile(int &dfaSrc, set<int> &finalStates, map<int, vector<pair<int, int> > > &list)
{
    string line;
    ifstream dfaFile("dfa.txt");
    if (dfaFile.is_open())
    {
        int lineNo = 1;
        while (getline(dfaFile, line))
        {
            if (lineNo == 1)
            {
                dfaSrc = line[0] - '0';
            }
            else if (lineNo == 2)
            {
                string num;
                for (int i = 0; i < line.size(); i++)
                {
                    if (line[i] != ' ')
                    {
                        num += line[i];
                    }
                    else
                    {
                        if (num == " " || num.empty())
                        {
                            num = "";
                            continue;
                        }
                        finalStates.insert(stoi(num));
                    }
                }
                if (num != " " && !num.empty())
                    finalStates.insert(stoi(num));
            }
            else
            {
                string num;
                int state = lineNo - 3;
                int input = 0;
                for (int i = 0; i < line.size(); i++)
                {
                    if (line[i] != ' ')
                    {
                        num += line[i];
                    }
                    else
                    {
                        if (num == " " || num.empty() || stoi(num) == -1) // dead edge
                        {
                            num = "";
                            input++;
                            continue;
                        }
                        list[state].push_back(make_pair(stoi(num), input));
                        num = "";
                        input++;
                    }
                }
                if (num != " " && !num.empty() && stoi(num) != -1) // dead edge
                    list[state].push_back(make_pair(stoi(num), input));
            }
            lineNo++;
        }

        dfaFile.close();
    }
    else
    {
        cout << "Unable to open dfa file.\n";
    }
}

bool depthFirstSearch(int iter, string &inputString, int dfaSrc, map<int, vector<pair<int, int> > > &list, set<int> &finalStates)
{
    if (iter == inputString.size())
        return finalStates.count(dfaSrc);
    for (int child=0;child<list[dfaSrc].size();child++)
    {
        if (list[dfaSrc][child].second == inputString[iter]-'0' )
        {
            if (depthFirstSearch(iter + 1, inputString, list[dfaSrc][child].first, list, finalStates))
                return true;
        }
    }
    return false;
}

int main()
{
    int dfaSrc;
    set<int> finalStates;
    map<int, vector<pair<int, int> > > list;
    dfaInputFromFile(dfaSrc, finalStates, list);
    cout << "Enter Input String : \n";
    string inputString="";
    getline(cin,inputString);
    if (depthFirstSearch(0, inputString, dfaSrc, list, finalStates))
    {
        cout << "Input String Is Accepted."
             << "\n";
    }
    else
    {
        cout << "Input String Is Not Accepted."
             << "\n";
    }
    return 0;
}