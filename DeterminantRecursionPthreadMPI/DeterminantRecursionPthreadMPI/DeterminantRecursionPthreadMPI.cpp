// DeterminantRecursionPthreadMPI.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <locale>
#include <vector>
#include <ctime>
#include <cmath>
#include <thread>
#include <mutex>
#include <math.h>
#include <stdlib.h>
#include <cstdlib>
#include <iomanip>
#include <windows.h>
#include <mpi.h>

using namespace std;

std::mutex g_lock;

//Возвращает матрицу matrix без row-ой строки и col-того столбца, результат в newMatrix
void getMatrixWithoutRowAndCol(double **matrix, int size, int row, int col, double **newMatrix) {
	int offsetRow = 0; //Смещение индекса строки в матрице
	int offsetCol = 0; //Смещение индекса столбца в матрице
	for (int i = 0; i < size - 1; i++) {
		//Пропустить row-ую строку
		if (i == row) {
			offsetRow = 1; //Как только встретили строку, которую надо пропустить, делаем смещение для исходной матрицы
		}

		offsetCol = 0; //Обнулить смещение столбца
		for (int j = 0; j < size - 1; j++) {
			//Пропустить col-ый столбец
			if (j == col) {
				offsetCol = 1; //Встретили нужный столбец, проускаем его смещением
			}

			newMatrix[i][j] = matrix[i + offsetRow][j + offsetCol];
		}
	}
}

//Вычисление определителя матрицы разложение по первой строке
double matrixDet(double **matrix, int size) {
	double det = 0;
	int degree = 1; // (-1)^(1+j) из формулы определителя

	//Условие выхода из рекурсии
	if (size == 1) {
		return matrix[0][0];
	}
	//Условие выхода из рекурсии
	else if (size == 2) {
		return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
	}
	else {
		//Матрица без строки и столбца
		double **newMatrix = new double*[size - 1];
		for (int i = 0; i < size - 1; i++) {
			newMatrix[i] = new double[size - 1];
		}

		//Раскладываем по 0-ой строке, цикл бежит по столбцам
		for (int j = 0; j < size; j++) {
			//Удалить из матрицы i-ю строку и j-ый столбец
			//Результат в newMatrix
			getMatrixWithoutRowAndCol(matrix, size, 0, j, newMatrix);

			//Рекурсивный вызов
			//По формуле: сумма по j, (-1)^(1+j) * matrix[0][j] * minor_j (это и есть сумма из формулы)
			//где minor_j - дополнительный минор элемента matrix[0][j]
			// (напомню, что минор это определитель матрицы без 0-ой строки и j-го столбца)
			det = det + (degree * matrix[0][j] * matrixDet(newMatrix, size - 1));
			//"Накручиваем" степень множителя
			degree = -degree;
		}

		//Чистим память на каждом шаге рекурсии(важно!)
		for (int i = 0; i < size - 1; i++) {
			delete[] newMatrix[i];
		}
		delete[] newMatrix;
	}

	return det;
}


int _tmain(int argc, char* argv[])
{
	

	ifstream in("data1.txt");
	if (!in.is_open())
		return 1;
	//размерность матрицы
	int m;

	double det = 0;
	int step = 0;
	double summ = 0;
	
	//вводим n
	in >> m;
	printf("%d\n", m);

	//определяем вектор размером mxm
	double **mas;
	mas = new double*[m];

	for (int i = 0; i < m; i++) {
		mas[i] = new double[m];
		for (int j = 0; j < m; j++) {
			in >> mas[i][j];//считывание матрицы из файла
			//cout << mas[i][j]<<" ";//вывод матрицы 
		}
	}

	unsigned int timeStart = clock();

	if (m >= 3)
	{

		int myid, numprocs = 5;

		if (numprocs != 0)
			step = m / numprocs;


		if (int rc = MPI_Init(&argc, &argv))
		{
			cout << "Ошибка запуска, выполнение остановлено " << endl;
			MPI_Abort(MPI_COMM_WORLD, rc);
		}

		MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
		MPI_Comm_rank(MPI_COMM_WORLD, &myid);

		MPI_Bcast(&step, 1, MPI_INT, 0, MPI_COMM_WORLD);

		int start = 0;
		int end = m;

		double detreg;

		if (myid == numprocs)
		{
			start = step*(numprocs - 1);
			end = m;
		}
		else
		{
			start = step*myid;
			end = step*(myid + 1) - 1;
		}
		

		double **newMatrix = new double*[m - 1];
		for (int i = 0; i < m - 1; i++) {
			newMatrix[i] = new double[m - 1];
		}

		for (start; start < end; start++) {
			getMatrixWithoutRowAndCol(mas, m, 0, start, newMatrix);
			detreg = matrixDet(newMatrix, m - 1);
			det = det + pow((-1), start) * mas[0][start] * detreg;
		}
		for (int i = 0; i < m - 1; i++) {
			delete[] newMatrix[i];
		}
		delete[] newMatrix;

		MPI_Reduce(&det, &summ, 1, MPI_LONG_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
		MPI_Finalize();

	}

	else
		summ = matrixDet(mas, m);

		unsigned int timeEnd = clock();

		cout << "Determinant = " << summ << "\n";

		unsigned int timeRezult = timeEnd - timeStart;
		cout << "Time Work Program = " << timeRezult << "\n";

		for (int i = 0; i < m; i++) delete[] mas[i];
		delete[] mas;
		in.close();

		system("pause");
		
	return 0;
}

