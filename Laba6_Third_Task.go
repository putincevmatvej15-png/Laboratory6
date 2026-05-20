package main

import (
	"fmt"
	"math"
)

const N = 4 // Размер системы

// Функция вывода расширенной матрицы
func printMatrix(A [N][N]float64, B [N]float64) {
	for i := 0; i < N; i++ {
		for j := 0; j < N; j++ {
			fmt.Printf("%10.4f ", A[i][j])
		}
		fmt.Printf("| %10.4f\n", B[i])
	}
	fmt.Println()
}

// Метод Гаусса с выбором главного элемента
func gaussElimination(A [N][N]float64, B [N]float64) ([N]float64, bool) {

	// Копируем матрицы, чтобы не изменять оригиналы
	workA := A
	workB := B
	var X [N]float64

	// Прямой ход с выбором главного элемента
	for k := 0; k < N; k++ {
		// Поиск главного элемента в столбце k
		maxRow := k
		maxVal := math.Abs(workA[k][k])
		for i := k + 1; i < N; i++ {
			if math.Abs(workA[i][k]) > maxVal {
				maxVal = math.Abs(workA[i][k])
				maxRow = i
			}
		}

		// Перестановка строк
		if maxRow != k {
			workA[k], workA[maxRow] = workA[maxRow], workA[k]
			workB[k], workB[maxRow] = workB[maxRow], workB[k]
			fmt.Printf("Перестановка строк %d и %d\n", k+1, maxRow+1)
		}

		// Проверка на вырожденность
		if math.Abs(workA[k][k]) < 1e-12 {
			fmt.Println("Матрица вырождена! Решения нет.")
			return X, false
		}

		// Нормализация строки k
		pivot := workA[k][k]
		for j := k; j < N; j++ {
			workA[k][j] /= pivot
		}
		workB[k] /= pivot

		// Исключение элемента в столбце k для всех строк ниже
		for i := k + 1; i < N; i++ {
			factor := workA[i][k]
			for j := k; j < N; j++ {
				workA[i][j] -= factor * workA[k][j]
			}
			workB[i] -= factor * workB[k]
		}

		fmt.Printf("После шага %d:\n", k+1)
		printMatrix(workA, workB)
	}

	// Обратный ход
	for i := N - 1; i >= 0; i-- {
		X[i] = workB[i]
		for j := i + 1; j < N; j++ {
			X[i] -= workA[i][j] * X[j]
		}
	}

	return X, true
}

func main() {
	// Данные для варианта 8: M = 0.91, N_val = -0.23, P = -1.04
	M := 0.91
	N_val := -0.23
	P := -1.04

	// Матрица A и вектор B
	var A [N][N]float64
	var B [N]float64

	// Заполнение матрицы по варианту 8
	// 1-е уравнение: M*x1 - 0.04*x2 + 0.21*x3 - 1.16*x4 = -1.24
	A[0][0] = M
	A[0][1] = -0.04
	A[0][2] = 0.21
	A[0][3] = -1.16
	B[0] = -1.24

	// 2-е уравнение: 0.25*x1 - 1.23*x2 + N*x3 - 0.09*x4 = P
	A[1][0] = 0.25
	A[1][1] = -1.23
	A[1][2] = N_val
	A[1][3] = -0.09
	B[1] = P

	// 3-е уравнение: -0.21*x1 + N*x2 + 0.8*x3 - 0.13*x4 = 2.56
	A[2][0] = -0.21
	A[2][1] = N_val
	A[2][2] = 0.8
	A[2][3] = -0.13
	B[2] = 2.56

	// 4-е уравнение: 0.15*x1 - 1.31*x2 + 0.06*x3 + P*x4 = M
	A[3][0] = 0.15
	A[3][1] = -1.31
	A[3][2] = 0.06
	A[3][3] = P
	B[3] = M

	fmt.Println("========================================")
	fmt.Println("\nИсходная система:")
	printMatrix(A, B)

	// Решение методом Гаусса
	X, ok := gaussElimination(A, B)
	if !ok {
		return
	}

	// Вывод результата
	fmt.Println("\n========================================")
	fmt.Println("Результат")
	for i := 0; i < N; i++ {
		fmt.Printf("x%d = %.6f\n", i+1, X[i])
	}

	// Проверка подстановкой
	fmt.Println("\n========================================")
	fmt.Println("Проверка подстановкой")
	for i := 0; i < N; i++ {
		sum := 0.0
		for j := 0; j < N; j++ {
			sum += A[i][j] * X[j]
		}
		fmt.Printf("Уравнение %d: %.6f = %.6f ", i+1, sum, B[i])
		if math.Abs(sum-B[i]) < 1e-5 {
			fmt.Println("верно")
		} else {
			fmt.Println("ошибка")
		}
	}
}
