/*
 * printDecomposition.h
 *
 *  Created on: Mar 20, 2017
 *      Author: Petra
 */

#ifndef PRINTDECOMPOSITION_H_
#define PRINTDECOMPOSITION_H_
#include "matrix.h"
#include "int-queue.h"

using namespace std;



///////////////////////////////////////////////////////////////////////////
//						Start of Print Decomposition Function			 //
///////////////////////////////////////////////////////////////////////////
void printDecomposition(Matrix* copymat, Node* nodes[], int &clusterNum, ofstream &onf){

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
	onf<<"(";
	for(int i=0; i<clusterNum; i++){
		onf<<"[";
		int first=0;
		for(int j=0; j<copymat->row; j++){
			if(nodes[j]->cluster==cluster[i]){
				first++;
				if(first!=1){
					onf<<", "<<(j+1);
				}else{
					onf<<(j+1);
				}
			}
		}
		if(i==clusterNum-1){
			onf<<"]";
		}else{
			onf<<"], ";
		}
	}
	onf<<")"<<endl;
}

///////////////////////////////////////////////////////////////////////////
//						End of Print Decomposition Function				 //
///////////////////////////////////////////////////////////////////////////




//==========================================================================================================================
//==========================================================================================================================



#endif /* PRINTDECOMPOSITION_H_ */
