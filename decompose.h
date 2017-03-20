/*
 * decompose.h
 *
 *  Created on: Mar 20, 2017
 *      Author: Petra
 */

#ifndef DECOMPOSE_H_
#define DECOMPOSE_H_
#include "matrix.h"
#include "int-queue.h"


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

}

///////////////////////////////////////////////////////////////////////////
//						End of Decompose Function						 //
///////////////////////////////////////////////////////////////////////////


//==========================================================================================================================
//==========================================================================================================================



#endif /* DECOMPOSE_H_ */
