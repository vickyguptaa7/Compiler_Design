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

void dfaInputFromFile(int &dfaSrc, map<int, vector<pair<int, pair<int,string> > > > &list)
{
    string line;
    ifstream dfaFile("mealey.txt");
    if (dfaFile.is_open())
    {
        // dfa source i/p
        int lineNo = 2;
        getline(dfaFile,line);
        dfaSrc=line[0]-'0';

        // edges i/p
        while (getline(dfaFile, line))
        {
                string num="";
                string output="";
                int state = lineNo - 2;
                int input = 0;
                bool isInput=true;
                for (int i = 0; i < line.size(); i++)
                {
                    if(isInput)
                    {
                        if (line[i] != ' ')
                        {
                            num += line[i];
                        }
                        else
                        {
                            isInput=!isInput;
                        }
                    }
                    else 
                    {
                        if(line[i]!=' ')
                        {
                            output+=line[i];
                        }
                        else
                        {
                            if(num==" "||num.empty()||stoi(num)==-1||output.empty()||output==" ")
                            {
                                num="";
                                output="";
                                input++;
                                isInput=!isInput;
                                continue;
                            }
                            cout<<state<<" "<<input<<" "<<stoi(num)<<" "<<output<<"\n";
                            list[state].push_back(make_pair(stoi(num),make_pair(input,output)));
                            num="";
                            output="";
                            isInput=!isInput;
                            input++;
                        }
                    }
                }
                if (num != " " ||  stoi(num)!=-1) // dead edge
                {
                    cout<<state<<" "<<input<<" "<<stoi(num)<<" "<<output<<"\n";
                    list[state].push_back(make_pair(stoi(num), make_pair(input,output)));
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

void depthFirstSearch(int iter, string &inputString, int dfaSrc, map<int, vector<pair<int, pair<int,string> > > > &list,string &output)
{
    cout<<dfaSrc<<" ";
    for (int child=0;child<list[dfaSrc].size();child++)
    {
        if (list[dfaSrc][child].second.first == inputString[iter]-'0')
        {
            output+=list[dfaSrc][child].second.second;
            depthFirstSearch(iter + 1, inputString, list[dfaSrc][child].first, list, output);
        }
    }
}

int main()
{
    int dfaSrc;
    map<int, vector<pair<int, pair<int,string> > > > list;
    dfaInputFromFile(dfaSrc, list);
    cout << "Enter Input String : \n";
    string inputString="";
    getline(cin,inputString);
    string output="";
    depthFirstSearch(0, inputString, dfaSrc, list,output);
    cout<<"\n";
    cout<<output<<"\n";
    if(output.empty())
    {
        cout<<"Empty output\n";
    }
    return 0;
}