#include <iostream>
#include <math.h>
#include <vector>
#include <algorithm>
using namespace std;

typedef long long ll;
class Node
{
public:
    bool val;
    bool isLeaf;
    Node *topLeft;
    Node *topRight;
    Node *bottomLeft;
    Node *bottomRight;

    Node()
    {
        val = false;
        isLeaf = false;
        topLeft = NULL;
        topRight = NULL;
        bottomLeft = NULL;
        bottomRight = NULL;
    }

    Node(bool _val, bool _isLeaf)
    {
        val = _val;
        isLeaf = _isLeaf;
        topLeft = NULL;
        topRight = NULL;
        bottomLeft = NULL;
        bottomRight = NULL;
    }

    Node(bool _val, bool _isLeaf, Node *_topLeft, Node *_topRight, Node *_bottomLeft, Node *_bottomRight)
    {
        val = _val;
        isLeaf = _isLeaf;
        topLeft = _topLeft;
        topRight = _topRight;
        bottomLeft = _bottomLeft;
        bottomRight = _bottomRight;
    }
};

Node *helper(int sx, int sy, int ex, int ey, vector<vector<int>> &grid)
{
    int cntr = 0;
    for (int i = sx; i <= ex; i++)
    {
        for (int j = sy; j <= ey; j++)
        {
            cntr += grid[i][j];
        }
    }
    if (cntr == 0)
    {
        return new Node(0, 1);
    }
    if (cntr == (ey - sy + 1) * (ex - sx + 1))
    {
        return new Node(1, 1);
    }

    Node *topLeft = helper(sx, sy, (ex - sx) / 2, (ey - sy) / 2, grid);

    Node *topRight = helper(sx, (ey - sy + 1) / 2, (ex - sx) / 2, ey, grid);

    Node *bottomLeft = helper((ex - sx + 1) / 2, sy, ex, (ey - sy) / 2, grid);

    Node *bottomRight = helper((ex - sx + 1) / 2, (ey - sy + 1) / 2, ex, ey, grid);

    return new Node(1, 0, topLeft, topRight, bottomLeft, bottomRight);
}

Node *construct(vector<vector<int>> &grid)
{
    int n = grid.size();
    return helper(0, 0, n - 1, n - 1, grid);
}
int main()
{
    vector<vector<int>> grid = {{1, 1, 1, 1, 0, 0, 0, 0}, {1, 1, 1, 1, 0, 0, 0, 0}, {1, 1, 1, 1, 1, 1, 1, 1}, {1, 1, 1, 1, 1, 1, 1, 1}, {1, 1, 1, 1, 0, 0, 0, 0}, {1, 1, 1, 1, 0, 0, 0, 0}, {1, 1, 1, 1, 0, 0, 0, 0}, {1, 1, 1, 1, 0, 0, 0, 0}};
    construct(grid);
    return 0;
}