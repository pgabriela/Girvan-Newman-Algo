/*
 * matrix.h
 *
 *  Created on: Mar 20, 2017
 *      Author: Petra
 */

#ifndef MATRIX_H_
#define MATRIX_H_
#include <iostream>
#include <fstream>

using namespace std;


//Creating a struct for matrix
struct Matrix{
	int row;
	int col;
	double **data;
};

//Function for printing a matrix
void printMatrix(const Matrix* m){
	if(m == NULL){
		cout<<"The matrix does not exist anymore"<<endl;
	}
	for(int i=0; i<m->row; i++){
		for(int j=0; j<m->col; j++){
			cout<<m->data[i][j]<<' ';
		}
		cout<<endl;
	}
}

//Function for creating a matrix from the input file
Matrix* createMatrix(const char file[]){
	ifstream inf(file);
	if (!inf){
		cerr << "Error: Cannot open the file" << endl;
		return NULL;
	}

	Matrix* mat = new Matrix;
	inf>>mat->row;
	mat->col = mat->row;

	mat->data = new double* [mat->row];
	for(int i=0; i<mat->row; i++){
		mat->data[i] = new double[mat->col];
	}
	for(int i=0; i<mat->row; i++){
		for(int j=0; j<mat->col; j++){
			inf>>mat->data[i][j];
		}
	}

	inf.close();
	return mat;
}

//Helper function that will be used by deleteMatrix function
void deleteRow(Matrix *m, int r){
	for(int i = r-1; i < m->row-1; i++){
		for(int j = 0; j < m->col; j++){
			m->data[i][j] = m->data[i+1][j];
		}
	}
	delete[] m->data[m->row-1];
	m->row--;
}

//Function for deleting (releasing memory) a matrix
void deleteMatrix(Matrix *m){
	while (m->row > 0){
		deleteRow(m, 1);
	}
	delete[] m->data;
	m->data = NULL;
	delete m;
}

#endif /* MATRIX_H_ */
