#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <iomanip>
#include <cstring>

using namespace std;
using namespace chrono;

// Глобальный генератор случайных чисел
mt19937 rng(steady_clock::now().time_since_epoch().count());

// ======================= ЗАДАНИЕ 1 =======================
void task1() {
    int M, N;
    cout << "\n ЗАДАНИЕ 1 \n";
    cout << "Введите M  и N : ";
    cin >> M >> N;
    
    if (M <= 5 || N <= 5) {
        cout << "Ошибка: M и N должны быть > 5\n";
        return;
    }
    
    // Выделяем динамический двумерный массив
    int** matrix = new int*[M];
    for (int i = 0; i < M; i++) {
        matrix[i] = new int[N];
    }
    
    // Заполняем случайными числами [-50, 50]
    uniform_int_distribution<int> dist(-50, 50);
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            matrix[i][j] = dist(rng);
        }
    }
    
    // Выводим матрицу
    cout << "\nИсходная матрица:\n";
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            cout << setw(5) << matrix[i][j];
        }
        cout << endl;
    }
    
    // Поиск самой длинной строго возрастающей последовательности
    int bestRow = -1;
    int bestLength = 1;
    int* bestSequence = nullptr;
    
    for (int i = 0; i < M; i++) {
        int maxLenInRow = 1;
        int currentLen = 1;
        int bestStartIdx = 0;
        
        // Ищем самую длинную строго возрастающую последовательность в строке i
        for (int j = 1; j < N; j++) {
            if (matrix[i][j] > matrix[i][j - 1]) {
                currentLen++;
                if (currentLen > maxLenInRow) {
                    maxLenInRow = currentLen;
                    bestStartIdx = j - currentLen + 1;
                }
            } else {
                currentLen = 1;
            }
        }
        
        // Обновляем глобальный максимум
        if (maxLenInRow > bestLength) {
            bestLength = maxLenInRow;
            bestRow = i;
            
            // Очищаем старую последовательность
            if (bestSequence != nullptr) {
                delete[] bestSequence;
            }
            
            // Создаем новую последовательность
            bestSequence = new int[bestLength];
            
            // Копируем последовательность
            for (int k = 0; k < bestLength; k++) {
                bestSequence[k] = matrix[i][bestStartIdx + k];
            }
        }
    }
    
    // Выводим результат
    cout << "\nРезультат задания 1:\n";
    if (bestRow != -1 && bestLength > 1) {
        cout << "Номер строки: " << bestRow << endl;
        cout << "Длина последовательности: " << bestLength << endl;
        cout << "Сама последовательность: ";
        for (int k = 0; k < bestLength; k++) {
            cout << bestSequence[k] << " ";
        }
        cout << endl;
    } else {
        cout << "Строго возрастающие последовательности длины >1 не найдены\n";
    }
    
    // Очистка памяти
    for (int i = 0; i < M; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
    
    if (bestSequence != nullptr) {
        delete[] bestSequence;
    }
}

// ======================= ЗАДАНИЕ 2 =======================
void task2() {
    int M;
    cout << "\nЗАДАНИЕ 2\n";
    cout << "Введите M : ";
    cin >> M;
    
    if (M <= 5) {
        cout << "Ошибка: M должно быть > 5\n";
        return;
    }
    
    // Создаем матрицу через vector
    vector<vector<int>> matrix(M, vector<int>(M));
    
    // Заполняем случайными числами [100, 200]
    uniform_int_distribution<int> dist(100, 200);
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < M; j++) {
            matrix[i][j] = dist(rng);
        }
    }
    
    cout << "\nИсходная матрица:\n";
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < M; j++) {
            cout << setw(5) << matrix[i][j];
        }
        cout << endl;
    }
    
    // Вычисляем среднее арифметическое четных СТОЛБЦОВ (j = 0, 2, 4...)
    double sumEvenCols = 0;
    int countEvenCols = 0;
    for (int j = 0; j < M; j += 2) {
        for (int i = 0; i < M; i++) {
            sumEvenCols += matrix[i][j];
            countEvenCols++;
        }
    }
    double avgEvenCols = sumEvenCols / countEvenCols;
    
    // Вычисляем среднее арифметическое нечетных СТРОК (i = 1, 3, 5...)
    double sumOddRows = 0;
    int countOddRows = 0;
    for (int i = 1; i < M; i += 2) {
        for (int j = 0; j < M; j++) {
            sumOddRows += matrix[i][j];
            countOddRows++;
        }
    }
    double avgOddRows = sumOddRows / countOddRows;
    
    cout << fixed << setprecision(2);
    cout << "\nСреднее арифметическое четных столбцов: " << avgEvenCols << endl;
    cout << "Среднее арифметическое нечетных строк: " << avgOddRows << endl;
    
    // Сохраняем главную диагональ
    vector<int> diagonal(M);
    for (int i = 0; i < M; i++) {
        diagonal[i] = matrix[i][i];
    }
    
    // Сортируем диагональ по условию
    if (avgEvenCols > avgOddRows) {
        sort(diagonal.begin(), diagonal.end(), greater<int>());
        cout << "\nСреднее четных столбцов больше → сортировка главной диагонали по УБЫВАНИЮ\n";
    } else {
        sort(diagonal.begin(), diagonal.end());
        cout << "\nСреднее четных столбцов меньше или равно → сортировка главной диагонали по ВОЗРАСТАНИЮ\n";
    }
    
    // Записываем отсортированную диагональ обратно
    for (int i = 0; i < M; i++) {
        matrix[i][i] = diagonal[i];
    }
    
    cout << "\nМатрица после сортировки главной диагонали:\n";
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < M; j++) {
            cout << setw(5) << matrix[i][j];
        }
        cout << endl;
    }
}

// ======================= ЗАДАНИЕ 3 =======================
// Функция поиска слова в матрице (DFS - поиск в глубину)
bool dfs(vector<vector<char>>& matrix, string& word, int idx, int i, int j, 
         vector<vector<bool>>& visited, int M, int N) {
    
    if (idx == word.length()) return true;
    if (i < 0 || i >= M || j < 0 || j >= N) return false;
    if (visited[i][j]) return false;
    if (matrix[i][j] != word[idx]) return false;
    
    visited[i][j] = true;
    
    // Проверяем соседей: вверх, вниз, влево, вправо (без диагоналей)
    bool found = dfs(matrix, word, idx + 1, i + 1, j, visited, M, N) ||
                 dfs(matrix, word, idx + 1, i - 1, j, visited, M, N) ||
                 dfs(matrix, word, idx + 1, i, j + 1, visited, M, N) ||
                 dfs(matrix, word, idx + 1, i, j - 1, visited, M, N);
    
    visited[i][j] = false;
    return found;
}

bool wordExists(vector<vector<char>>& matrix, string& word) {
    int M = matrix.size();
    int N = matrix[0].size();
    vector<vector<bool>> visited(M, vector<bool>(N, false));
    
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            if (matrix[i][j] == word[0]) {
                if (dfs(matrix, word, 0, i, j, visited, M, N)) {
                    return true;
                }
            }
        }
    }
    return false;
}

void task3() {
    int M, N;
    cout << "\nЗАДАНИЕ 3\n";
    cout << "Введите M и N: ";
    cin >> M >> N;
    
    if (M < 5 || M > 10 || N < 5 || N > 10) {
        cout << "Ошибка: M и N должны быть от 5 до 10\n";
        return;
    }
    
    // Создаем матрицу символов
    vector<vector<char>> matrix(M, vector<char>(N));
    
    // Заполняем случайными заглавными латинскими буквами
    uniform_int_distribution<int> dist('A', 'Z');
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            matrix[i][j] = static_cast<char>(dist(rng));
        }
    }
    
    // Выводим матрицу
    cout << "\nМатрица символов:\n";
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            cout << setw(3) << matrix[i][j];
        }
        cout << endl;
    }
    
    // Ввод слова для поиска
    string word;
    cout << "\nВведите слово для поиска (заглавные латинские буквы): ";
    cin >> word;
    
    // Поиск регистрозависимый
    if (wordExists(matrix, word)) {
        cout << "\nРЕЗУЛЬТАТ: true - слово \"" << word << "\" найдено в матрице!\n";
    } else {
        cout << "\nРЕЗУЛЬТАТ: false - слово \"" << word << "\" не найдено в матрице.\n";
    }
}

// ======================= MAIN =======================
int main() {
    setlocale(LC_ALL, "Russian");
    
    task1();
    task2();
    task3();
    
    cout << "\n========================================\n";
    cout << "Программа завершена\n";
    
    return 0;
}