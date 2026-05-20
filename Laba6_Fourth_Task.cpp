#include <iostream>
#include <queue>
#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <limits>

using namespace std;

void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

bool isValidSize(int N, int M) {
    return (N > 0 && M > 0 && N * M <= 16);
}

bool isValidChar(char c) {
    return (c == '0' || c == '1');
}

bool isValidString(const string& s, int expectedLen) {
    if (s.length() != expectedLen) return false;
    for (char c : s) {
        if (!isValidChar(c)) return false;
    }
    return true;
}

bool inputDimensions(int& N, int& M) {
    cout << "Введите размеры доски N M: ";
    cin >> N >> M;
    
    if (cin.fail()) {
        cout << "Ошибка: введите два целых числа!\n";
        clearInput();
        return false;
    }
    
    if (!isValidSize(N, M)) {
        cout << "Ошибка: размеры должны быть положительными, а общее число клеток не более 16\n";
        cout << "Рекомендуемые размеры: 1x1, 1x2, 2x2, 2x3, 3x3, 3x4, 4x4\n";
        return false;
    }
    
    return true;
}

vector<vector<int>> inputBoard(int N, int M) {
    vector<vector<int>> board(N, vector<int>(M));
    
    cout << "Введите начальную доску (" << N << " строк по " << M << " цифр 0/1):\n";
    
    for (int i = 0; i < N; i++) {
        string line;
        cin >> line;
        
        if (cin.fail() || !isValidString(line, M)) {
            cout << "Ошибка: строка " << i+1 << " должна содержать ровно " << M << " цифр (0 или 1)\n";
            cout << "Повторите ввод: ";
            cin >> line;
            
            if (!isValidString(line, M)) {
                cout << "Критическая ошибка: ввод прерван\n";
                return {};
            }
        }
        
        for (int j = 0; j < M; j++) {
            board[i][j] = line[j] - '0';
        }
    }
    
    return board;
}

string boardToString(const vector<vector<int>>& board) {
    string s;
    for (const auto& row : board)
        for (int cell : row)
            s += to_string(cell);
    return s;
}

vector<vector<int>> stringToBoard(const string& s, int N, int M) {
    vector<vector<int>> board(N, vector<int>(M));
    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++)
            board[i][j] = s[i * M + j] - '0';
    return board;
}

vector<vector<int>> makeTarget(int N, int M, int borderValue) {
    vector<vector<int>> target(N, vector<int>(M));
    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++)
            target[i][j] = (i == 0 || i == N-1 || j == 0 || j == M-1) ? borderValue : 1 - borderValue;
    return target;
}

pair<int, vector<vector<int>>> bfsMinMoves(const vector<vector<int>>& start, const vector<vector<int>>& target) {
    int N = start.size(), M = start[0].size();
    string startStr = boardToString(start);
    string targetStr = boardToString(target);
    
    if (startStr == targetStr) return {0, start};
    
    queue<string> q;
    map<string, int> dist;
    map<string, string> parent;
    
    q.push(startStr);
    dist[startStr] = 0;
    
    while (!q.empty()) {
        string cur = q.front();
        q.pop();
        int d = dist[cur];
        
        vector<vector<int>> board = stringToBoard(cur, N, M);
        
        // Горизонтальные пары
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M - 1; j++) {
                vector<vector<int>> newBoard = board;
                newBoard[i][j] ^= 1;
                newBoard[i][j+1] ^= 1;
                string next = boardToString(newBoard);
                
                if (dist.find(next) == dist.end()) {
                    dist[next] = d + 1;
                    parent[next] = cur;
                    q.push(next);
                    
                    if (next == targetStr) {
                        return {d + 1, newBoard};
                    }
                }
            }
        }
        
        // Вертикальные пары
        for (int i = 0; i < N - 1; i++) {
            for (int j = 0; j < M; j++) {
                vector<vector<int>> newBoard = board;
                newBoard[i][j] ^= 1;
                newBoard[i+1][j] ^= 1;
                string next = boardToString(newBoard);
                
                if (dist.find(next) == dist.end()) {
                    dist[next] = d + 1;
                    parent[next] = cur;
                    q.push(next);
                    
                    if (next == targetStr) {
                        return {d + 1, newBoard};
                    }
                }
            }
        }
    }
    return {-1, {}};
}

void selectAndPrintBestSolution(int moves0, int moves1, const vector<vector<int>>& board0, const vector<vector<int>>& board1) {
    int bestMoves = -1;
    vector<vector<int>> bestBoard;
    
    if (moves0 != -1 && (bestMoves == -1 || moves0 < bestMoves)) {
        bestMoves = moves0;
        bestBoard = board0;
    }
    if (moves1 != -1 && (bestMoves == -1 || moves1 < bestMoves)) {
        bestMoves = moves1;
        bestBoard = board1;
    }
    
    if (bestMoves == -1) {
        cout << -1 << endl;
    } else {
        cout << "\nМинимальное число ходов - " << bestMoves << "\n" << endl;
        for (const auto& row : bestBoard) {
            for (int cell : row) cout << cell;
            cout << endl;
        }
    }
}

int main() {
    int N, M;
    
    // Ввод размеров
    while (!inputDimensions(N, M)) {
        // повторяем до правильного ввода
    }
    
    // Ввод доски
    vector<vector<int>> start = inputBoard(N, M);
    if (start.empty()) {
        cout << "Ошибка при вводе доски. Программа завершена.\n";
        return 1;
    }
    
    // Создание целевых рамок
    auto target0 = makeTarget(N, M, 0);
    auto target1 = makeTarget(N, M, 1);
    
    // Поиск решения
    auto [moves0, board0] = bfsMinMoves(start, target0);
    auto [moves1, board1] = bfsMinMoves(start, target1);
    
    // Выбор и вывод лучшего решения
    selectAndPrintBestSolution(moves0, moves1, board0, board1);
    
    return 0;
}