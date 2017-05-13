// DeterminantRecursionPthread.cpp : Defines the entry point for the console application.
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

using namespace std;

std::mutex g_lock;

//���������� ������� matrix ��� row-�� ������ � col-���� �������, ��������� � newMatrix
void getMatrixWithoutRowAndCol(double **matrix, int size, int row, int col, double **newMatrix) {
	int offsetRow = 0; //�������� ������� ������ � �������
	int offsetCol = 0; //�������� ������� ������� � �������
	for (int i = 0; i < size - 1; i++) {
		//���������� row-�� ������
		if (i == row) {
			offsetRow = 1; //��� ������ ��������� ������, ������� ���� ����������, ������ �������� ��� �������� �������
		}

		offsetCol = 0; //�������� �������� �������
		for (int j = 0; j < size - 1; j++) {
			//���������� col-�� �������
			if (j == col) {
				offsetCol = 1; //��������� ������ �������, ��������� ��� ���������
			}

			newMatrix[i][j] = matrix[i + offsetRow][j + offsetCol];
		}
	}
}

//���������� ������������ ������� ���������� �� ������ ������
double matrixDet(double **matrix, int size) {
	double det = 0;
	int degree = 1; // (-1)^(1+j) �� ������� ������������

	//������� ������ �� ��������
	if (size == 1) {
		return matrix[0][0];
	}
	//������� ������ �� ��������
	else if (size == 2) {
		return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
	}
	else {
		//������� ��� ������ � �������
		double **newMatrix = new double*[size - 1];
		for (int i = 0; i < size - 1; i++) {
			newMatrix[i] = new double[size - 1];
		}

		//������������ �� 0-�� ������, ���� ����� �� ��������
		for (int j = 0; j < size; j++) {
			//������� �� ������� i-� ������ � j-�� �������
			//��������� � newMatrix
			getMatrixWithoutRowAndCol(matrix, size, 0, j, newMatrix);

			//����������� �����
			//�� �������: ����� �� j, (-1)^(1+j) * matrix[0][j] * minor_j (��� � ���� ����� �� �������)
			//��� minor_j - �������������� ����� �������� matrix[0][j]
			// (�������, ��� ����� ��� ������������ ������� ��� 0-�� ������ � j-�� �������)
			det = det + (degree * matrix[0][j] * matrixDet(newMatrix, size - 1));
			//"�����������" ������� ���������
			degree = -degree;
		}

		//������ ������ �� ������ ���� ��������(�����!)
		for (int i = 0; i < size - 1; i++) {
			delete[] newMatrix[i];
		}
		delete[] newMatrix;
	}

	return det;
}
void treadFunc(double **matrix, int size, double &det, int start, int end) {
	
	double detreg;
	//int degree = 1; // (-1)^(1+j) �� ������� ������������
	//������� ��� ������ � �������

	double **newMatrix = new double*[size - 1];
	for (int i = 0; i < size - 1; i++) {
		newMatrix[i] = new double[size - 1];
	}
		for (start; start < end; start++) {
			getMatrixWithoutRowAndCol(matrix, size, 0, start, newMatrix);
			detreg = matrixDet(newMatrix, size - 1);
			det = det + pow((-1), start) * matrix[0][start] * detreg;
			//degree = -degree;
		}
	//cout << det << endl;

	for (int i = 0; i < size - 1; i++) {
		delete[] newMatrix[i];
	}
	delete[] newMatrix;
}

int _tmain(int argc, _TCHAR* argv[])
{


	ifstream in("data1.txt");
	if (!in.is_open())
		return 1;
	//����������� �������
	int m;
	int numberOfProcesses = 4;
	double *dett = new double[numberOfProcesses];

	double summ = 0;
	double det = 0;
	double det1 = 0;
	double det2 = 0;
	double det3 = 0;
	double det4 = 0;
	double det5 = 0;
	int step = 0;
	//������ n
	in >> m;
	printf("%d\n", m);

	//���������� ������ �������� mxm
	double **mas;
	mas = new double*[m];

	for (int i = 0; i < m; i++) {
		mas[i] = new double[m];
		for (int j = 0; j < m; j++) {
			in >> mas[i][j];//���������� ������� �� �����
			//cout << mas[i][j]<<" ";//����� ������� 
		}
	}


	if (numberOfProcesses != 0)
		step = m / numberOfProcesses;


	//vector<thread*> vecThreads;

	unsigned int timeStart = clock();
	if (m >= 3)
	{

		/*for (int k = 0; k < numberOfProcesses; k++)
		{
			if (k == numberOfProcesses - 1)
				{
				auto th = new std::thread(treadFunc, mas, m, std::ref(dett[k]), step*k, m);
				vecThreads.push_back(th);
				cout << step*k << " " << m << "\n";
				}
			else
				{
				auto th = new std::thread(treadFunc, mas, m, std::ref(dett[k]), step*k, step*(k + 1) - 1);
				vecThreads.push_back(th);
				cout << step*k << " " << step*(k + 1) - 1 << "\n";
				}

		}


		for (auto &th : vecThreads)
		{
		th->join();
		delete th;
		}
		vecThreads.clear();*/

		//treadFunc(mas, m, det1, 0, 10);
		//std::thread t1(treadFunc, mas, m, std::ref(det1), 0, 10);


		if (numberOfProcesses == 0)
		{
			treadFunc(mas, m, det1, 0, m);
		}
		
		if (numberOfProcesses == 1)
		{
			std::thread t1(treadFunc, mas, m, std::ref(det1), 0, m);
			t1.join();
		}
		if (numberOfProcesses == 2)
		{
			std::thread t1(treadFunc, mas, m, std::ref(det1), step * 0, step*(0 + 1) - 1);
			std::thread t2(treadFunc, mas, m, std::ref(det2), step * 1, m);
			t1.join();
			t2.join();
		}
		if (numberOfProcesses == 3)
		{
			std::thread t1(treadFunc, mas, m, std::ref(det1), step * 0, step*(0 + 1) - 1);
			std::thread t2(treadFunc, mas, m, std::ref(det2), step * 1, step*(1 + 1) - 1);
			std::thread t3(treadFunc, mas, m, std::ref(det3), step * 2, m);
			t1.join();
			t2.join();
			t3.join();
		}
		if (numberOfProcesses == 4)
		{
			std::thread t1(treadFunc, mas, m, std::ref(det1), step * 0, step*(0 + 1) - 1);
			std::thread t2(treadFunc, mas, m, std::ref(det2), step * 1, step*(1 + 1) - 1);
			std::thread t3(treadFunc, mas, m, std::ref(det3), step * 2, step*(2 + 1) - 1);
			std::thread t4(treadFunc, mas, m, std::ref(det4), step * 3, m);
			t1.join();
			t2.join();
			t3.join();
			t4.join();
		}
		if (numberOfProcesses == 5)
		{
			std::thread t1(treadFunc, mas, m, std::ref(det1), step * 0, step*(0 + 1) - 1);
			std::thread t2(treadFunc, mas, m, std::ref(det2), step * 1, step*(1 + 1) - 1);
			std::thread t3(treadFunc, mas, m, std::ref(det3), step * 2, step*(2 + 1) - 1);
			std::thread t4(treadFunc, mas, m, std::ref(det4), step * 3, step*(3 + 1) - 1);
			std::thread t5(treadFunc, mas, m, std::ref(det5), step * 4, m);
			t1.join();
			t2.join();
			t3.join();
			t4.join();
			t5.join();
		}

		det = det1 + det2 + det3 + det4 + det5;

		/*for (int i = 0; i < numberOfProcesses; i++)
			summ = summ + dett[i];*/
	}
	else
	det = matrixDet(mas, m);
	

	unsigned int timeEnd = clock();

	cout << "Determinant = " << det << "\n";

	unsigned int timeRezult = timeEnd - timeStart;
	cout << "Time Work Program = " << timeRezult << "\n";

	for (int i = 0; i<m; i++) delete[] mas[i];
	delete[] mas;
	delete[] dett;
	in.close();

	system("pause");
	return 0;
}