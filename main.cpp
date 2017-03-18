/*
 * main.cpp
 *
 *  Created on: Mar 17, 2017
 *      Author: Petra
 */


#include "int-queue.h"
#include <iostream>
#include <fstream>


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
//						Start of Betweenness Function					 //
///////////////////////////////////////////////////////////////////////////

Matrix* betweenness(Matrix* test){

	//Create an array of Nodes
	Node* nodes[test->col];
	for(int i=0; i<test->col; i++){
		nodes[i] = new Node;
		nodes[i]->num_of_paths = 0;
	}
	//int currentNode = -1;

	//Create a matrix storing the total betweenness
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

	//Calculating the total betweenness
	for(int i=0; i<test->row; i++){
		int_queue* Queue = new int_queue(test->col);	//BFSVisit Starts here
		//currentNode = i;								//currentNode = which root in current bfs tree
		for(int j=0; j<test->col; j++){					//Initialization
			nodes[j]->color = WHITE;
			nodes[j]->distance = -1;
		}
		nodes[i]->distance = 0;
		Queue->enqueue(nodes[i]);
		while(!Queue->empty()){
			Node* u = Queue->front();					//for dequeue,
			Queue->dequeue();							//just use different method
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

		//Put the current betweenness matrix in tempBet matrix
		//----------------------------------------------------------
		Matrix* tempBet = new Matrix;						//
		tempBet->col = test->col;							//
		tempBet->row = test->row;							//
		tempBet->data = new int* [totalBet->row];			// Making a matrix called tempBet
		for(int i=0; i<tempBet->row; i++){					//
			tempBet->data[i] = new int[tempBet->col];		//
		}													//
		for(int i=0; i<tempBet->row; i++){					//
			for(int j=0; j<tempBet->col; j++){				//
				tempBet->data[i][j] = 0;					//
			}												//
		}													//

		//Calculate the number of shortest path for node i to each nodes
		int max_path = 0;										//
		for(int iter=0;iter<test->row; iter++){					// Calculate diameter
			max_path = max(max_path, nodes[iter]->distance);	//
		}														//

		/* Iterate through all nodes to assign the number of possible shortest paths from node i to each nodes*/
		for(int n=0; n<tempBet->row; n++){
			if(nodes[n]->distance==0){
				nodes[n]->num_of_paths = 1;
				int deep = 0;

				do {
					deep++;
					for(int q=0; q<tempBet->row; q++){
						if(nodes[q]->distance == deep){
							for(int z=0; z<tempBet->col; z++){
								if(test->data[q][z]==1 && nodes[z]->distance<nodes[q]->distance){
									nodes[q]->num_of_paths += nodes[z]->num_of_paths;
								}
							}
						}
					}
				} while (deep<max_path);
				break;
			}
		}

		//Iterate to assign every edge betweenness to tempBet
		int deep = max_path;
		while(deep>0){
			for(int q=0; q<tempBet->row; q++){
				if(nodes[q]->distance == deep){
					int total=1, predNum=0, totalPred_num_of_path=0;

					//summing all the edge betweenness below this node that is connected to this node
					for(int z=0; z<tempBet->col; z++){
						if(test->data[q][z]==1 && nodes[q]->distance<nodes[z]->distance){
							total += tempBet->data[q][z];
						}
					}
					//calculating the total number of the predecessors
					for(int b=0; b<tempBet->row; b++){
						if(test->data[q][b]==1 && nodes[q]->distance>nodes[b]->distance){
							predNum++;
							totalPred_num_of_path += nodes[b]->num_of_paths;
						}
					}
					//Put the total betweenness to the predecessors
					//(if more than 1 pred, divide with ratio according to the predecessors' num_of_path)
					if(predNum>0){
						for(int b=0; b<tempBet->row; b++){
							if(test->data[q][b]==1 && nodes[q]->distance>nodes[b]->distance){
								tempBet->data[q][b] = total * (nodes[b]->num_of_paths / totalPred_num_of_path);
								tempBet->data[b][q] = total * (nodes[b]->num_of_paths / totalPred_num_of_path);
							}
						}
					}
				}
			}
			deep--;
		}

		//Add the total betweenness matrix by the betweenness from this current tree
		for(int row=0; row<totalBet->row; row++){
			for(int col=0; col<totalBet->col; col++){
				totalBet->data[row][col] += tempBet->data[row][col];
			}
		}

		delete Queue;
		deleteMatrix(tempBet);
	}


	//Divide all by 2



	for(int i=0; i<test->col; i++){
		delete nodes[i];
	}
	return totalBet;
}
///////////////////////////////////////////////////////////////////////////
//						End of Betweenness Function						 //
///////////////////////////////////////////////////////////////////////////

//==========================================================================================================================
//==========================================================================================================================

int main() {
	Matrix* mat = createMatrix("input.txt");
	Matrix* totalBetweennessMat = betweenness(mat);
	printMatrix(totalBetweennessMat);

	deleteMatrix(mat);
}
