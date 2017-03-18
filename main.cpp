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
	double **data;
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

Matrix* betweenness(Matrix* test, Node* nodes[], Matrix* totalBet){

	for(int i=0; i<totalBet->row; i++){
		for(int j=0; j<totalBet->col; j++){
			totalBet->data[i][j] = 0;
		}
	}

	////---------------DEBUG------------
	/**///cout<<"This is TotalBetMat"<<endl;
	/**///printMatrix(totalBet);
	/**///cout<<endl;
	////--------------------------------


	//Calculating the total betweenness
	for(int i=0; i<test->row; i++){
		int_queue* Queue = new int_queue(test->col);	//BFSVisit Starts here
		for(int j=0; j<test->col; j++){					//Initialization
			nodes[j]->color = WHITE;
			nodes[j]->distance = -1;
			nodes[j]->num_of_paths = 0;
		}
		nodes[i]->distance = 0;
		Queue->enqueue(nodes[i]);
		while(!Queue->empty()){
			Node* u = Queue->front();					// for dequeue, just splitting the
			Queue->dequeue();							// usual dequeue function into 2 function
			for(int v=0; v<test->col; v++){
				if(test->data[u->pos][v]==1){
					if(nodes[v]->color==WHITE){
						nodes[v]->color=GRAY;									// Color the node that will be visited to GRAY

						nodes[v]->distance = nodes[u->pos]->distance + 1;		// The distance of the nodes that will be visited equals to
																				// 1 + the distance of current node

						Queue->enqueue(nodes[v]);								// Put the node that will be visited to the queue
					}
				}
			}
			u->color = BLACK;		//color the node that is done with BLACK
		}

		//Put the current betweenness matrix in tempBet matrix
		//----------------------------------------------------------
		Matrix* tempBet = new Matrix;						//
		tempBet->col = test->col;							//
		tempBet->row = test->row;							//
		tempBet->data = new double* [totalBet->row];		// Creating a matrix called tempBet
		for(int i=0; i<tempBet->row; i++){					//
			tempBet->data[i] = new double[tempBet->col];	//
		}													//
		for(int i=0; i<tempBet->row; i++){					//
			for(int j=0; j<tempBet->col; j++){				//
				tempBet->data[i][j] = 0;					//
			}												//
		}													//

		//Calculate the number of shortest path for node i to each nodes
		double max_path = 0;									//
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
					double total=1, predNum=0, totalPred_num_of_path=0;

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
		////---------------DEBUG------------
		/**///cout<<"This is TempBetMat"<<endl;
		/**///printMatrix(tempBet);
		/**///cout<<endl;
		////--------------------------------

		//Add the total betweenness matrix by the betweenness from this current tree
		for(int row=0; row<totalBet->row; row++){
			for(int col=0; col<totalBet->col; col++){
				totalBet->data[row][col] += tempBet->data[row][col];
			}
		}

		////---------------DEBUG------------
		/**///cout<<"This is TotalBetMat"<<endl;
		/**///printMatrix(totalBet);
		/**///cout<<endl;
		////--------------------------------

		delete Queue;
		deleteMatrix(tempBet);
	}


	//Divide all by 2
	for(int k=0; k<totalBet->row; k++){
		for(int p=0; p<totalBet->col; p++){
			totalBet->data[k][p] /= 2;
		}
	}

	return totalBet;
}
///////////////////////////////////////////////////////////////////////////
//						End of Betweenness Function						 //
///////////////////////////////////////////////////////////////////////////


//==========================================================================================================================
//==========================================================================================================================


///////////////////////////////////////////////////////////////////////////
//						Start of Decompose Function						 //
///////////////////////////////////////////////////////////////////////////

void decompose(Matrix* copymat, Node* nodes[], Matrix* totalBetweennessMat, int &clusterNum){

	//Initialize the cluster data of all nodes differently with each other (maximum possible number of clusters)
	for(int i=0; i<copymat->row; i++){
		nodes[i]->cluster = i;
	}

	//Search for the largest betweenness value
	double biggestBet=0;
	for(int i=0; i<copymat->row; i++){
		for(int j=0; j<copymat->col; j++){
			if(totalBetweennessMat->data[i][j]>0){
				biggestBet = max(biggestBet, totalBetweennessMat->data[i][j]);
			}
		}
	}

	//Erase the edge(s) with the biggest betweenness value
	for(int i=0; i<copymat->row; i++){
		for(int j=0; j<copymat->col; j++){
			if(totalBetweennessMat->data[i][j]==biggestBet){
				copymat->data[i][j] = 0;
			}
		}
	}

	//Split the nodes into clusters
	for(int i=0; i<copymat->row; i++){
		for(int j=0; j<copymat->col; j++){
			if(copymat->data[i][j]==1 && i!=j){
				nodes[j]->cluster = nodes[i]->cluster;
			}
		}
	}

	//Count the total number of clusters as well as put the groups of cluster into cluster[]
	int cluster[copymat->row];
	clusterNum=0;
	for(int i=0; i<copymat->row; i++){
		cluster[i] = -1;
	}
	for(int i=0; i<copymat->row; i++){
		int flag=0;
		for(int j=0; j<copymat->row; j++){
			if(cluster[j] == nodes[i]->cluster){
				flag = 1;
				break;
			}
		}
		if(!flag){
			cluster[clusterNum] = nodes[i]->cluster;
			clusterNum++;
		}
	}


	//Print out the decomposition
	cout<<"(";
	for(int i=0; i<clusterNum; i++){
		cout<<"[";
		int first=0;
		for(int j=0; j<copymat->row; j++){
			if(nodes[j]->cluster==cluster[i]){
				first++;
				if(first!=1){
					cout<<", "<<(j+1);
				}else{
					cout<<(j+1);
				}
			}
		}
		if(i==clusterNum-1){
			cout<<"]";
		}else{
			cout<<"], ";
		}
	}
	cout<<")"<<endl;

}

///////////////////////////////////////////////////////////////////////////
//						End of Decompose Function						 //
///////////////////////////////////////////////////////////////////////////


//==========================================================================================================================
//==========================================================================================================================

int main() {

	//original matrix stored in matrix called 'mat'
	Matrix* mat = createMatrix("input.txt");

	//Copy Original matrix called 'copymat'
	Matrix* copymat = new Matrix;
	copymat->row = copymat->col = mat->col;
	copymat->data = new double*[copymat->row];
	for(int i=0; i<copymat->row; i++){
		copymat->data[i] = new double[copymat->col];
	}
	for(int i=0; i<copymat->row; i++){
		for(int j=0; j<copymat->col; j++){
			copymat->data[i][j] = mat->data[i][j];
		}
	}


	//Create an array of Nodes called 'nodes'
	Node* nodes[mat->col];
	for(int i=0; i<mat->col; i++){
		nodes[i] = new Node;
		nodes[i]->num_of_paths = 0;
		nodes[i]->pos = i;
		nodes[i]->cluster = 0;
	}


	//Create a matrix storing the total betweenness called 'totalBet'
	Matrix* totalBet = new Matrix;
	totalBet->col = mat->col;
	totalBet->row = mat->row;
	totalBet->data = new double* [totalBet->row];
	for(int i=0; i<totalBet->row; i++){
		totalBet->data[i] = new double[totalBet->col];
	}
	for(int i=0; i<totalBet->row; i++){			//
		for(int j=0; j<totalBet->col; j++){		//
			totalBet->data[i][j] = 0;			// Initialize all item to 0
		}										//
	}											//


	//creating a pointer to 'totalBet' called 'totalBetweennessMat'
	Matrix* totalBetweennessMat = totalBet;


	//iterate until total number of clusters == total nodes
	int clusterNum=0;
	cout<<"network decomposition:"<<endl;
	for(; clusterNum<copymat->row ; ){
		totalBetweennessMat = betweenness(copymat, nodes, totalBetweennessMat);		//compute the total betweenness of all edges
		decompose(copymat, nodes, totalBetweennessMat, clusterNum);					//decomposition and print the result
	}


	//releasing the dynamic allocated memory
	deleteMatrix(totalBetweennessMat);
	for(int i=0; i<mat->col; i++){
		delete nodes[i];
	}
	deleteMatrix(mat);
	deleteMatrix(copymat);

	return 0;
}
