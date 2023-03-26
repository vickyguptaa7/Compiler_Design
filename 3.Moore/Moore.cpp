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

void mooreInputFromFile(int &MooreSrc, map<int, vector<pair<int, int> > > &list,map<int,string>&stateOutput)
{
    string line;
    ifstream dfaFile("Moore.txt");
    if (dfaFile.is_open())
    {
        int lineNo = 1;
        getline(dfaFile,line);
        MooreSrc=line[0]-'0';
        int state=0;
        while (getline(dfaFile, line))
        {
            string num;
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
                    cout<<input<<" "<<num<<"\n";
                    list[state].push_back(make_pair(stoi(num), input));
                    num = "";
                    input++;
                }
            }
            if (num != " " && !num.empty() && stoi(num) != -1) // dead edge
                stateOutput[state]=num;

            state++;
        }
        dfaFile.close();
    }
    else
    {
        cout << "Unable to open moore file.\n";
    }
}

bool depthFirstSearch(int iter, string &inputString, int mooreSrc, map<int, vector<pair<int, int> > > &list, map<int,string> &stateOutput,string &output)
{
    output+=stateOutput[mooreSrc];
    for (int child=0;child<list[mooreSrc].size();child++)
    {
        if (list[mooreSrc][child].second == inputString[iter]-'0' )
        {
            depthFirstSearch(iter + 1, inputString, list[mooreSrc][child].first, list, stateOutput,output);
        }
    }
}

int main()
{
    int mooreSrc;
    map<int, vector<pair<int, int> > > list;
    map<int,string>stateOutput;
    mooreInputFromFile(mooreSrc, list,stateOutput);
    cout << "Enter Input String : \n";
    string inputString="";
    getline(cin,inputString);
    string output;
    depthFirstSearch(0, inputString, mooreSrc, list, stateOutput,output);
    cout<<output<<"\n";
    return 0;
}