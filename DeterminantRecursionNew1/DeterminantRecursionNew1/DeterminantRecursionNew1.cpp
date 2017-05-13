// DeterminantRecursionNew1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <locale>
#include <vector>
#include <ctime>
#include <math.h>
#include <stdlib.h>
#include <cstdlib>
#include <iomanip>
#include <windows.h>

using namespace std;

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


int _tmain(int argc, _TCHAR* argv[])
{
	
	
	ifstream in("data.txt");
	if (!in.is_open())
		return 1;
	//����������� �������
	int m;
	double d;
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
			//cout << mas[i][j]<<" ";//����� ������� � �������
		}
		//printf("\n");
	}
	//printf("\n");
	unsigned int timeStart = clock();
	d = matrixDet(mas, m);
	cout << "Determinant = " << d << "\n";

	unsigned int timeEnd = clock();
	unsigned int timeRezult = timeEnd - timeStart;
	cout << "Time Work Program = " << timeRezult << "\n";

	for (int i = 0; i<m; i++) delete[] mas[i];
	delete[] mas;
	in.close();

	system("pause");
	return 0;
}

