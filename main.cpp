/*
 * main.cpp
 *
 *  Created on: Mar 17, 2017
 *      Author: Petra
 */


#include "int-queue.h"
#include "matrix.h"
#include "betweenness.h"
#include "decompose.h"
#include "modularity.h"
#include "printDecomposition.h"
#include <iostream>


using namespace std;



int main() {


	//for output stream
	ofstream onf("output.txt");
	if (!onf){
		cerr << "Error: Cannot open the file" << endl;
		return -1;
	}


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
		nodes[i]->num_of_paths = 0;					//Number of possible shortest paths
		nodes[i]->pos = i;							//Just storing a position value in the nodes[] for later searching
		nodes[i]->cluster = 0;						//To locate a node (which cluster a node belongs to)
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
	onf<<"network decomposition:"<<endl;
	for(; clusterNum<copymat->row ; ){
		totalBetweennessMat = betweenness(copymat, nodes, totalBetweennessMat);		//compute the total betweenness of all edges
		decompose(copymat, nodes, totalBetweennessMat, clusterNum);					//decomposition
		printDecomposition(copymat, nodes, clusterNum, onf);						//print the decomposition
	}

	onf<<endl;

	for(int i=0; i<copymat->row; i++){						//
		for(int j=0; j<copymat->col; j++){					//
			copymat->data[i][j] = mat->data[i][j];			//
		}													//
	}														// Re-initializing things
	for(int i=0; i<mat->col; i++){							//
		nodes[i]->num_of_paths = 0;							//
		nodes[i]->pos = i;									//
		nodes[i]->cluster = 0;								//
	}														//
	clusterNum=0;											//



	double maxMod = -50;

	for(; clusterNum<copymat->row ; ){
		double Q;
		totalBetweennessMat = betweenness(copymat, nodes, totalBetweennessMat);		//compute the total betweenness of all edges
		decompose(copymat, nodes, totalBetweennessMat, clusterNum);					//decomposition
		Q = modularity(mat, copymat, nodes, clusterNum);							//print the modularity
		maxMod = max(maxMod, Q);													//finding the maximum modularity
		onf<<clusterNum<<" clusters: modularity "<<Q<<endl;
	}



	for(int i=0; i<copymat->row; i++){						//
		for(int j=0; j<copymat->col; j++){					//
			copymat->data[i][j] = mat->data[i][j];			//
		}													//
	}														// Re-initializing things
	for(int i=0; i<mat->col; i++){							//
		nodes[i]->num_of_paths = 0;							//
		nodes[i]->pos = i;									//
		nodes[i]->cluster = 0;								//
	}														//
	clusterNum=0;											//


	onf<<endl;
	onf<<"optimal structure: "<<endl;

	for(; clusterNum<copymat->row ; ){
		double Q = -50;
		totalBetweennessMat = betweenness(copymat, nodes, totalBetweennessMat);		//compute the total betweenness of all edges
		decompose(copymat, nodes, totalBetweennessMat, clusterNum);					//decomposition
		Q = modularity(mat, copymat, nodes, clusterNum);							//check modularity
		if(maxMod == Q) printDecomposition(copymat, nodes, clusterNum, onf);				//print the decomposition
	}




	//releasing the dynamic allocated memory
	deleteMatrix(totalBetweennessMat);
	for(int i=0; i<mat->col; i++){
		delete nodes[i];
	}
	deleteMatrix(mat);
	deleteMatrix(copymat);
	onf.close();

	return 0;
}
