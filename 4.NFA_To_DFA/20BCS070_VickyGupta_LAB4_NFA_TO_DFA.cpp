#include <iostream>
#include <math.h>
#include <vector>
#include <algorithm>
#include <fstream>
#include <set>
#include <map>
#include <string.h>
#include <sstream>
#include <stack>
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

void dfaInputFromFile(int &dfaSrc, set<int> &finalStates, map<int, map<int, set<int>>> &list, int &inputLen)
{
    string line;
    ifstream dfaFile("NFA_TO_DFA.txt");
    if (dfaFile.is_open())
    {
        string word;
        getline(dfaFile, line);
        stringstream s0(line);

        while (s0 >> word)
        {
            dfaSrc = word[0] - '0';
        }

        getline(dfaFile, line);
        stringstream s1(line);

        while (s1 >> word)
        {
            finalStates.insert(stoii(word));
        }
        int parentNode = 0;
        while (getline(dfaFile, line))
        {
            stringstream s2(line);
            int inputs = 0;
            while (s2 >> word)
            {
                stringstream s3(word);
                string state;
                while (getline(s3, state, ','))
                {
                    if (stoii(state) == -1)
                        continue;
                    list[parentNode][inputs].insert(stoii(state));
                }
                inputs++;
            }
            // to get the size of the input len
            inputLen = max(inputs, inputLen);
            parentNode++;
        }
        dfaFile.close();
    }
    else
    {
        cout << "Unable to open dfa file.\n";
    }
}

void printMapContent(map<int, map<int, set<int>>> &list)
{
    // printing the list for verification of the edges created
    map<int, map<int, set<int>>>::iterator itr;
    for (itr = list.begin(); itr != list.end(); itr++)
    {
        map<int, set<int>>::iterator ptr;
        for (ptr = itr->second.begin(); ptr != itr->second.end(); ptr++)
        {
            cout << "(" << itr->first << "->{";
            set<int>::iterator qtr;
            for (qtr = ptr->second.begin(); qtr != ptr->second.end(); qtr++)
            {
                cout << *qtr << ",";
            }
            cout << "} " << ptr->first << ")\n";
        }
        cout << "\n";
    }
}

void helper(map<int, map<int, set<int>>> &nfaList, map<set<int>, int> &stateMapping, map<int, set<int>> &reverseStateMapping, map<int, map<int, int>> &dfaList, stack<set<int>> &problematicState, int &newStateNumber)
{
    map<int, map<int, set<int>>>::iterator itr;
    for (itr = nfaList.begin(); itr != nfaList.end(); itr++)
    {
        map<int, set<int>>::iterator ptr;
        for (ptr = itr->second.begin(); ptr != itr->second.end(); ptr++)
        {
            if (ptr->second.size() > 1)
            {
                problematicState.push(ptr->second);
                reverseStateMapping[newStateNumber] = ptr->second;
                stateMapping[ptr->second] = newStateNumber++;
            }
            dfaList[itr->first][ptr->first] = stateMapping[ptr->second];
        }
    }
}

set<int> newFinalStateFinder(map<set<int>, int> &stateMapping, set<int> &finalStates)
{
    set<int> newFinalStates;
    for (map<set<int>, int>::iterator itr = stateMapping.begin(); itr != stateMapping.end(); itr++)
    {
        set<int>::iterator jtr;
        for (jtr = itr->first.begin(); jtr != itr->first.end(); jtr++)
        {
            if (finalStates.count(*jtr))
            {
                newFinalStates.insert(itr->second);
                break;
            }
        }
    }
    return newFinalStates;
}

void conversionNfaToDfa(map<int, map<int, int>> &dfaList, int inputLen, map<set<int>, int> &stateMapping, map<int, set<int>> &reverseStateMapping, stack<set<int>> &problematicStates, int &newStateNumber)
{
    while (!problematicStates.empty())
    {
        set<int> state = problematicStates.top();
        problematicStates.pop();
        for (int input = 0; input < inputLen; input++)
        {
            set<int> newState;
            set<int>::iterator itr;
            for (itr = state.begin(); itr != state.end(); itr++)
            {
                if (dfaList[*itr].count(input))
                {
                    // adding the original individual state to this newState bcoz if we add the newStateNumber there will be a problem. ex if q3={q0, q1, q2} and q1 ={q1} we combine this to new state {q3,q1} but its not a new state it's an old state if we decompose the q3 and add individual states we get that this states already exist.
                    if (reverseStateMapping.count(dfaList[*itr][input]))
                    {
                        for (auto st : reverseStateMapping[dfaList[*itr][input]])
                        {
                            newState.insert(st);
                        }
                    }
                }
            }
            if (newState.empty())
                continue;
            if (!stateMapping.count(newState))
            {
                reverseStateMapping[newStateNumber] = newState;
                stateMapping[newState] = newStateNumber++;
                problematicStates.push(newState);
            }
            dfaList[stateMapping[state]][input] = stateMapping[newState];
        }
    }
    for (map<set<int>, int>::iterator itr = stateMapping.begin(); itr != stateMapping.end(); itr++)
    {
        dfaList[itr->second];
    }
}

void printDfa(map<int, map<int, int>> &dfaList, set<int> &newFinalStates, int inputLen, int dfaSrc)
{
    cout << "\n\n";
    cout << dfaSrc << "\n";
    for (set<int>::iterator itr = newFinalStates.begin(); itr != newFinalStates.end(); itr++)
    {
        cout << *itr << " ";
    }
    cout << "\n";
    for (map<int, map<int, int>>::iterator itr = dfaList.begin(); itr != dfaList.end(); itr++)
    {
        for (int input = 0; input < inputLen; input++)
        {
            if (itr->second.count(input))
            {
                cout << itr->second[input] << " ";
            }
            else
            {
                cout << "-1 ";
            }
        }
        cout << "\n";
    }
}

int main()
{
    int dfaSrc;
    set<int> finalStates;
    map<int, map<int, set<int>>> nfaList;
    map<int, map<int, int>> dfaList;
    map<set<int>, int> stateMapping;
    map<int, set<int>> reverseStateMapping;
    stack<set<int>> problematicStates;
    int inputLen = 0;
    dfaInputFromFile(dfaSrc, finalStates, nfaList, inputLen);

    int newStateNumber = nfaList.rbegin()->first + 1;
    // mapping
    for (int i = 0; i < newStateNumber; i++)
    {
        stateMapping[{i}] = i;
        reverseStateMapping[i] = {i};
    }
    // helper find the problematic states creates the initial dfa state list
    helper(nfaList, stateMapping, reverseStateMapping, dfaList, problematicStates, newStateNumber);

    
    conversionNfaToDfa(dfaList, inputLen, stateMapping, reverseStateMapping, problematicStates, newStateNumber);
    
    // new final states
    set<int> newFinalStates = newFinalStateFinder(stateMapping, finalStates);
    printDfa(dfaList, newFinalStates, inputLen, dfaSrc);

    return 0;
}
/*

Inputs
0
1
0,1,2 1 -1
-1 2 1
-1 0,2 -1

0
3
-1 0,1,2
3 2
4 -1
-1 -1
-1 3

*/