/*
 * 15 Puzzle Solver using Branch and Bound Algorithm
 *
 * Description:
 * This program solves the classic 15 puzzle (sliding tile puzzle) using the
 * Branch and Bound algorithm with Manhattan distance heuristic. The puzzle
 * consists of 15 numbered tiles in a 4x4 grid with one empty space. Tiles
 * adjacent to the empty space can be moved into it.
 *
 * How it works:
 * 1. Branch and Bound is a state space search algorithm that explores the most
 * promising node first
 *
 * 2. States are stored in a priority queue based on f(n) = g(n) + h(n) where:
 *    - g(n) is the cost so far (number of moves)
 *    - h(n) is the Manhattan distance heuristic (sum of distances of tiles from
 * their goal positions)
 *
 * 3. The algorithm expands nodes with the lowest f(n) value until the goal
 * state is reached
 *
 * Time Complexity: O(b^d) where b is the branching factor (up to 4) and d is
 * the solution depth Space Complexity: O(b^d) for storing nodes in the priority
 * queue
 *
 * Note: The 15-puzzle is not solvable for all initial configurations. The
 * program checks solvability first.
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 4
#define MAX_MOVES 100

// Structure to represent a state of the puzzle
typedef struct {
  int board[N][N];
  int x, y;             // Position of the blank tile (0)
  int cost;             // Number of moves so far (g value)
  int heuristic;        // Manhattan distance (h value)
  int totalCost;        // f(n) = g(n) + h(n)
  int moves[MAX_MOVES]; // Sequence of moves to reach this state
  int moveCount;        // Number of moves made so far
} PuzzleState;

// Structure for priority queue node
typedef struct Node {
  PuzzleState state;
  struct Node *next;
} Node;

// Priority Queue functions
Node *newNode(PuzzleState state) {
  Node *temp = (Node *)malloc(sizeof(Node));
  temp->state = state;
  temp->next = NULL;
  return temp;
}

// Insert in priority queue based on totalCost
void insert(Node **head, PuzzleState state) {
  Node *start = *head;
  Node *temp = newNode(state);

  // Empty list or new node has lower cost than head
  if (*head == NULL || state.totalCost < (*head)->state.totalCost) {
    temp->next = *head;
    *head = temp;
  } else {
    // Find position to insert
    while (start->next != NULL &&
           start->next->state.totalCost <= state.totalCost) {
      start = start->next;
    }
    temp->next = start->next;
    start->next = temp;
  }
}

// Remove the front node from priority queue
PuzzleState pop(Node **head) {
  Node *temp = *head;
  PuzzleState state = temp->state;
  *head = (*head)->next;
  free(temp);
  return state;
}

// Check if priority queue is empty
bool isEmpty(Node *head) { return head == NULL; }

// Calculate Manhattan distance
int calculateManhattanDistance(int board[N][N]) {
  int distance = 0;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (board[i][j] != 0) {
        // Calculate correct position of this tile
        int correctRow = (board[i][j] - 1) / N;
        int correctCol = (board[i][j] - 1) % N;
        distance += abs(i - correctRow) + abs(j - correctCol);
      }
    }
  }
  return distance;
}

// Check if puzzle is solved
bool isSolved(int board[N][N]) {
  int count = 1;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (i == N - 1 && j == N - 1) {
        if (board[i][j] != 0)
          return false;
      } else {
        if (board[i][j] != count++)
          return false;
      }
    }
  }
  return true;
}

// Check if the puzzle is solvable
bool isSolvable(int board[N][N]) {
  int invCount = 0;
  int linearBoard[N * N];
  int blankRow = 0;

  // Convert 2D board to 1D for counting inversions
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      linearBoard[i * N + j] = board[i][j];
      if (board[i][j] == 0) {
        blankRow = i;
      }
    }
  }

  // Count inversions
  for (int i = 0; i < N * N - 1; i++) {
    for (int j = i + 1; j < N * N; j++) {
      if (linearBoard[i] && linearBoard[j] && linearBoard[i] > linearBoard[j]) {
        invCount++;
      }
    }
  }

  // For a 4x4 puzzle:
  // If blank is on even row from bottom (row 4, 2), parity of inversions must
  // be odd If blank is on odd row from bottom (row 3, 1), parity of inversions
  // must be even
  if ((N - blankRow) % 2 == 0)
    return (invCount % 2 == 1);
  else
    return (invCount % 2 == 0);
}

// Direction arrays for possible moves
int rowDir[4] = {-1, 1, 0, 0}; // Up, Down, Left, Right
int colDir[4] = {0, 0, -1, 1};
char moveNames[4] = {'U', 'D', 'L', 'R'};

// Solve puzzle using Branch and Bound
bool solvePuzzle(int initialBoard[N][N]) {
  // Check if puzzle is solvable
  if (!isSolvable(initialBoard)) {
    printf("This puzzle configuration is not solvable.\n");
    return false;
  }

  // Create initial state
  PuzzleState initial;
  memcpy(initial.board, initialBoard, sizeof(initial.board));
  initial.cost = 0;
  initial.moveCount = 0;

  // Find blank position
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (initialBoard[i][j] == 0) {
        initial.x = i;
        initial.y = j;
        break;
      }
    }
  }

  initial.heuristic = calculateManhattanDistance(initial.board);
  initial.totalCost = initial.cost + initial.heuristic;

  // Create priority queue
  Node *pq = NULL;
  insert(&pq, initial);

  // To avoid processing duplicate states
  bool visited[65536] = {false}; // 2^16 possible states for 4x4 puzzle

  // Process states until solution is found or queue is empty
  while (!isEmpty(pq)) {
    PuzzleState current = pop(&pq);

    // Convert state to a unique hash for checking duplicates
    int hash = 0;
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        hash = hash * 16 + current.board[i][j];
      }
    }

    if (visited[hash % 65536])
      continue;
    visited[hash % 65536] = true;

    // Check if goal state reached
    if (isSolved(current.board)) {
      printf("Solution found in %d moves:\n", current.moveCount);
      for (int i = 0; i < current.moveCount; i++) {
        printf("%c ", moveNames[current.moves[i]]);
      }
      printf("\n");
      return true;
    }

    // Try all possible moves
    for (int dir = 0; dir < 4; dir++) {
      int newX = current.x + rowDir[dir];
      int newY = current.y + colDir[dir];

      // Check if move is valid
      if (newX >= 0 && newX < N && newY >= 0 && newY < N) {
        // Create new state by making the move
        PuzzleState newState = current;

        // Swap blank with adjacent tile
        newState.board[current.x][current.y] = newState.board[newX][newY];
        newState.board[newX][newY] = 0;

        // Update blank position
        newState.x = newX;
        newState.y = newY;

        // Update cost
        newState.cost = current.cost + 1;
        newState.heuristic = calculateManhattanDistance(newState.board);
        newState.totalCost = newState.cost + newState.heuristic;

        // Record this move
        if (newState.moveCount < MAX_MOVES) {
          newState.moves[newState.moveCount++] = dir;
        }

        // Add to priority queue
        insert(&pq, newState);
      }
    }
  }

  printf("No solution found.\n");
  return false;
}

// Print the board
void printBoard(int board[N][N]) {
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (board[i][j] == 0)
        printf(" _  ");
      else
        printf("%2d  ", board[i][j]);
    }
    printf("\n");
  }
}

int main() {
  int board[N][N];

  printf("Enter the 15-puzzle configuration (use 0 for empty space):\n");
  printf("Enter numbers from 0 to 15, row by row (4 numbers per row):\n");

  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      scanf("%d", &board[i][j]);
    }
  }

  printf("\nSolving the following 15-puzzle:\n");
  printBoard(board);
  printf("\n");

  solvePuzzle(board);

  return 0;
}