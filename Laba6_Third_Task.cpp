#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>

using namespace std;

const int N = 4; // Размер системы

// Функция вывода расширенной матрицы
void printMatrix(const vector<vector<double>>& A, const vector<double>& B) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cout << setw(10) << fixed << setprecision(4) << A[i][j] << " ";
        }
        cout << "| " << setw(10) << fixed << setprecision(4) << B[i] << endl;
    }
    cout << endl;
}

// Метод Гаусса с выбором главного элемента
void gaussElimination(vector<vector<double>> A, vector<double> B, vector<double>& X) {

    // Прямой ход с выбором главного элемента
    for (int k = 0; k < N; k++) {
        // Поиск главного элемента в столбце k
        int maxRow = k;
        double maxVal = fabs(A[k][k]);
        for (int i = k + 1; i < N; i++) {
            if (fabs(A[i][k]) > maxVal) {
                maxVal = fabs(A[i][k]);
                maxRow = i;
            }
        }

        // Перестановка строк
        if (maxRow != k) {
            swap(A[k], A[maxRow]);
            swap(B[k], B[maxRow]);
            cout << "Перестановка строк " << k + 1 << " и " << maxRow + 1 << endl;
        }

        // Проверка на вырожденность
        if (fabs(A[k][k]) < 1e-12) {
            cout << "Матрица вырождена! Решения нет." << endl;
            return;
        }

        // Нормализация строки k
        double pivot = A[k][k];
        for (int j = k; j < N; j++) {
            A[k][j] /= pivot;
        }
        B[k] /= pivot;

        // Исключение элемента в столбце k для всех строк ниже
        for (int i = k + 1; i < N; i++) {
            double factor = A[i][k];
            for (int j = k; j < N; j++) {
                A[i][j] -= factor * A[k][j];
            }
            B[i] -= factor * B[k];
        }

        cout << "После шага " << k + 1 << ":" << endl;
        printMatrix(A, B);
    }

    // Обратный ход
    X.resize(N);
    for (int i = N - 1; i >= 0; i--) {
        X[i] = B[i];
        for (int j = i + 1; j < N; j++) {
            X[i] -= A[i][j] * X[j];
        }
    }
}

int main() {
    setlocale(LC_ALL, "Russian");

    // Данные для варианта 8: M = 0.91, N_val = -0.23, P = -1.04
    double M = 0.91;
    double N_val = -0.23;
    double P = -1.04;

    // Матрица A и вектор B
    vector<vector<double>> A(N, vector<double>(N));
    vector<double> B(N);

    // Заполнение матрицы по варианту 8
    // 1-е уравнение: M*x1 - 0.04*x2 + 0.21*x3 - 1.16*x4 = -1.24
    A[0][0] = M;        A[0][1] = -0.04; A[0][2] = 0.21;  A[0][3] = -1.16; B[0] = -1.24;
    // 2-е уравнение: 0.25*x1 - 1.23*x2 + N*x3 - 0.09*x4 = P
    A[1][0] = 0.25;     A[1][1] = -1.23; A[1][2] = N_val; A[1][3] = -0.09; B[1] = P;
    // 3-е уравнение: -0.21*x1 + N*x2 + 0.8*x3 - 0.13*x4 = 2.56
    A[2][0] = -0.21;    A[2][1] = N_val; A[2][2] = 0.8;   A[2][3] = -0.13; B[2] = 2.56;
    // 4-е уравнение: 0.15*x1 - 1.31*x2 + 0.06*x3 + P*x4 = M
    A[3][0] = 0.15;     A[3][1] = -1.31; A[3][2] = 0.06;  A[3][3] = P;     B[3] = M;

    cout << "========================================" << endl;
    cout << "\nИсходная система:" << endl;
    printMatrix(A, B);

    // Решение методом Гаусса
    vector<double> X;
    gaussElimination(A, B, X);

    // Вывод результата
    cout << "\n========================================" << endl;
    cout << "Результат" << endl;
    for (int i = 0; i < N; i++) {
        cout << "x" << i + 1 << " = " << fixed << setprecision(6) << X[i] << endl;
    }

    // Проверка подстановкой
    cout << "\n========================================" << endl;
    cout << "Проверка подстановкой" << endl;
    for (int i = 0; i < N; i++) {
        double sum = 0;
        for (int j = 0; j < N; j++) {
            sum += A[i][j] * X[j];
        }
        cout << "Уравнение " << i + 1 << ": " << fixed << setprecision(6)
             << sum << " = " << B[i] << " ";
        if (fabs(sum - B[i]) < 1e-5) {
            cout << "верно";
        } else {
            cout << "ошибка";
        }
        cout << endl;
    }

    return 0;
}