/*
    15 Puzzle Solver using Branch and Bound Algorithm

    Description:
    This program solves the classic 15 puzzle (sliding tile puzzle) using the
    Branch and Bound algorithm with Manhattan distance heuristic. The puzzle
    consists of 15 numbered tiles in a 4x4 grid with one empty space. Tiles
    adjacent to the empty space can be moved into it.

    How it works:
        1. Branch and Bound is a state space search algorithm that explores the most
        promising node first
        2. States are stored in a priority queue based on f(n) = g(n) + h(n) where:
           - g(n) is the cost so far (number of moves)
           - h(n) is the Manhattan distance heuristic (sum of distances of tiles from
        their goal positions)
        3. The algorithm expands nodes with the lowest f(n) value until the goal
        state is reached

    Time Complexity: O(b^d) where b is the branching factor (up to 4) and d is 
                    the solution depth Space Complexity: O(b^d) for storing nodes in the priority
                    queue.

    Note: The 15-puzzle is not solvable for all initial configurations. The program checks solvability first.
 */

#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <cmath>

using namespace std;

const int N = 4;

int dx[4] = {0, 1, 0, -1}; // Left, Down, Right, Up
int dy[4] = {-1, 0, 1, 0};
char moveNames[4] = {'L', 'D', 'R', 'U'};

vector<vector<int>> goal = {
    {1, 2, 3, 4},
    {5, 6, 7, 8},
    {9, 10, 11, 12},
    {13, 14, 15, 0}
};

struct State {
    vector<vector<int>> mat;
    int x, y;
    int g, f;
    string path; // Record move sequence

    bool operator>(const State &other) const {
        return f > other.f;
    }
};

// Manhattan distance heuristic
int manhattan(const vector<vector<int>> &mat) {
    int dist = 0;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            int val = mat[i][j];
            if (val != 0) {
                int targetX = (val - 1) / N;
                int targetY = (val - 1) % N;
                dist += abs(i - targetX) + abs(j - targetY);
            }
        }
    }
    return dist;
}

// Convert board to a unique number (used for visited state tracking)
long long hashState(const vector<vector<int>> &mat) {
    long long hash = 0;
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            hash = hash * 16 + mat[i][j];
    return hash;
}

bool isSolvable(vector<vector<int>> board) {
    vector<int> linear;
    int blankRow = 0;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (board[i][j] == 0) blankRow = i;
            linear.push_back(board[i][j]);
        }
    }
    int inv = 0;
    for (int i = 0; i < 16; ++i) {
        for (int j = i + 1; j < 16; ++j) {
            if (linear[i] && linear[j] && linear[i] > linear[j]) ++inv;
        }
    }
    return ((N - blankRow) % 2 == 0) ? (inv % 2 == 1) : (inv % 2 == 0);
}

void printBoard(const vector<vector<int>> &mat) {
    for (const auto &row : mat) {
        for (int val : row) {
            if (val == 0)
                cout << " _ ";
            else
                cout << (val < 10 ? " " : "") << val << " ";
        }
        cout << endl;
    }
}

int main() {
    vector<vector<int>> board(N, vector<int>(N));
    cout << "Enter the 15-puzzle configuration (use 0 for empty space):\n";
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            cin >> board[i][j];

    cout << "\nInitial Board:\n";
    printBoard(board);

    if (!isSolvable(board)) {
        cout << "\nThis puzzle configuration is not solvable.\n";
        return 0;
    }

    int startX, startY;
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            if (board[i][j] == 0) {
                startX = i;
                startY = j;
            }

    State start{board, startX, startY, 0, manhattan(board), ""};
    priority_queue<State, vector<State>, greater<State>> pq;
    unordered_set<long long> visited;

    pq.push(start);

    while (!pq.empty()) {
        State curr = pq.top(); pq.pop();
        long long key = hashState(curr.mat);
        if (visited.count(key)) continue;
        visited.insert(key);

        if (manhattan(curr.mat) == 0) {
            cout << "\nSolved in " << curr.g << " moves.\n";
            cout << "Move sequence: " << curr.path << "\n";
            return 0;
        }

        for (int i = 0; i < 4; ++i) {
            int newX = curr.x + dx[i], newY = curr.y + dy[i];
            if (newX >= 0 && newX < N && newY >= 0 && newY < N) {
                State next = curr;
                swap(next.mat[curr.x][curr.y], next.mat[newX][newY]);
                next.x = newX;
                next.y = newY;
                next.g = curr.g + 1;
                next.f = next.g + manhattan(next.mat);
                next.path += moveNames[i];
                pq.push(next);
            }
        }
    }
    cout << "No solution found.\n";
    return 0;
}

/*

// To print each intermediate configuration
    if (manhattan(curr.mat) == 0) {
        vector<vector<int>> tempBoard = board;
        int zeroX = startX, zeroY = startY;
    
        cout << "Solution -\n";
        printBoard(tempBoard);  // Print the initial configuration
        cout << endl;
    
        int step = 1;
        for (char move : curr.path) {
            int dir;
            if (move == 'L') dir = 0;  // Left
            else if (move == 'D') dir = 1;  // Down
            else if (move == 'R') dir = 2;  // Right
            else dir = 3;  // Up
    
            int newX = zeroX + dx[dir];
            int newY = zeroY + dy[dir];
            swap(tempBoard[zeroX][zeroY], tempBoard[newX][newY]);  // Swap the empty space with the tile
            zeroX = newX;
            zeroY = newY;
    
            cout << "Step " << step++ << ": Move " << move << "\n";
            printBoard(tempBoard);  // Print the intermediate configuration after the move
            cout << endl;
        }
    
        cout << "Solved in " << curr.g << " moves.\n";
        cout << "Move sequence: " << curr.path << "\n";
        return 0;
    }

*/