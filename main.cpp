/*
 * main.cpp
 *
 *  Created on: Mar 17, 2017
 *      Author: Petra
 */


#include "int-queue.h"
#include <iostream>
#include <fstream>
//#include <cmath>


using namespace std;


//==========================================================================================================================
//==========================================================================================================================

///////////////////////////////////////////////////////////////////////////
//	 JUST HELPER FUNCTIONS FOR CREATING MATRIX FROM THE INPUT FILE		 //
///////////////////////////////////////////////////////////////////////////
struct Matrix{
	int row;
	int col;
	int **data;
};

Matrix* createMatrix(const char file[]){
	ifstream inf(file);
	if (!inf){
		cerr << "Error: Cannot open the file" << endl;
		return NULL;
	}

	Matrix* mat = new Matrix;
	inf>>mat->row;
	mat->col = mat->row;

	mat->data = new int* [mat->row];
	for(int i=0; i<mat->row; i++){
		mat->data[i] = new int[mat->col];
	}
	for(int i=0; i<mat->row; i++){
		for(int j=0; j<mat->col; j++){
			inf>>mat->data[i][j];
		}
	}

	inf.close();
	return mat;
}

void deleteRow(Matrix *m, int r){
	for(int i = r-1; i < m->row-1; i++){
		for(int j = 0; j < m->col; j++){
			m->data[i][j] = m->data[i+1][j];
		}
	}
	delete[] m->data[m->row-1];
	m->row--;
}

void deleteMatrix(Matrix *m){
	while (m->row > 0){
		deleteRow(m, 1);
	}
	delete[] m->data;
	m->data = NULL;
	delete m;
}
///////////////////////////////////////////////////////////////////////////
//	 					END OF HELPER FUNCTIONS							 //
///////////////////////////////////////////////////////////////////////////

//==========================================================================================================================
//==========================================================================================================================

///////////////////////////////////////////////////////////////////////////
//						Start of Betweeness Function					 //
///////////////////////////////////////////////////////////////////////////

Matrix* betweeness(Matrix* test, Matrix* output){
	//Create an array of Nodes
	Node* nodes[test->col];
	for(int i=0; i<test->col; i++){
		nodes[i] = new Node;
		nodes[i]->pred = new Node*[test->col-1];
	}
	int currentNode = -1;

	//Create a matrix storing the total betweeness
	Matrix* totalBet = new Matrix;
	totalBet->col = test->col;
	totalBet->row = test->row;
	totalBet->data = new int* [totalBet->row];
	for(int i=0; i<totalBet->row; i++){
		totalBet->data[i] = new int[totalBet->col];
	}
	for(int i=0; i<totalBet->row; i++){
		for(int j=0; j<totalBet->col; j++){
			totalBet->data[i][j] = 0;
		}
	}

	//Calculating the total betweeness
	for(int i=0; i<test->col; i++){
		int_queue* Queue = new int_queue(test->col);	//BFSVisit Starts here
		currentNode = i;								//currentNode = which root in current bfs tree
		for(int j=0; j<test->col; j++){					//Initialization
			nodes[j]->pred = NULL;
			nodes[j]->color = WHITE;
			nodes[j]->distance = -1;
		}
		nodes[i]->distance = 0;
		Queue->enqueue(nodes[i]);
		while(!Queue->empty()){
			Node* u = Queue->front();
			Queue->dequeue();
			for(int v=0; v<test->col; v++){
				if(test->data[i][v]==1){
					if(nodes[v]->color==WHITE){
						nodes[v]->color=GRAY;
						nodes[v]->distance = nodes[i]->distance + 1;
						Queue->enqueue(nodes[v]);
					}
				}
			}
			u->color = BLACK;
		}

		//Add the total betweeness matrix by the betweeness from this current tree
		int max_path = 0;
		for(int n=0; n<test->col; n++){
			max_path = max(max_path, nodes[n]->distance);
		}
		for(int x=0; x<test->col; x++){
			if(nodes[x]->distance==max_path){

			}
		}

		delete Queue;
	}


	//Divide all by 2



	for(int i=0; i<test->col; i++){
		delete [] nodes[i]->pred;
		delete nodes[i];
	}
	return totalBet;
}
///////////////////////////////////////////////////////////////////////////
//						End of Betweeness Function						 //
///////////////////////////////////////////////////////////////////////////

//==========================================================================================================================
//==========================================================================================================================

int main() {
	Matrix* mat = createMatrix("input.txt");



	deleteMatrix(mat);
}
