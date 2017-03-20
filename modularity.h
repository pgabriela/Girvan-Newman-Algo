/*
 * modularity.h
 *
 *  Created on: Mar 20, 2017
 *      Author: Petra
 */

#ifndef MODULARITY_H_
#define MODULARITY_H_
#include "matrix.h"
#include "int-queue.h"

using namespace std;



//==========================================================================================================================
//==========================================================================================================================


///////////////////////////////////////////////////////////////////////////
//						Start of Modularity Function					 //
///////////////////////////////////////////////////////////////////////////

double modularity(Matrix* originalMat, Matrix* copymat, Node* nodes[], int &clusterNum){

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

	//Creating a new membership matrix [with (row = number of nodes) and (col = number of clusters)] called S
	//S-ij, the element of S, is 1 if node i belongs to cluster j, and 0 otherwise
	Matrix* S = new Matrix;
	S->row = copymat->row;
	S->col = clusterNum;
	S->data = new double*[S->row];
	for(int i=0; i<S->row; i++){
		S->data[i] = new double[S->col];
	}
	for(int i=0; i<copymat->row; i++){
		for(int j=0; j<clusterNum; j++){
			if(nodes[i]->cluster == cluster[j]){
				S->data[i][j] = 1;
			}else{
				S->data[i][j] = 0;
			}
		}
	}

	//----------------------------------------------------------------------//
	/*-------Calculating the Modularity(Q) of the current network-----------*/
	//----------------------------------------------------------------------//
	double twoM = 0;								//
	for(int i=0; i<originalMat->row; i++){			//
		for(int j=0; j<originalMat->col; j++){		// Calculating 2m (the total degree of the network)
			twoM += originalMat->data[i][j];		//
		}											//
	}												//


	Matrix* B = new Matrix;						//
	B->col = B->row = copymat->row;				//
	B->data = new double*[B->row];				// Creating a Modularity Matrix called B
	for(int i=0; i<B->row; i++){				//
		B->data[i] = new double[B->col];		//
	}											//

	for(int i=0; i<B->row; i++){									//
		double ki=0;												//
		for(int x=0; x<copymat->col; x++){							// Calculating ki(degree of node i)
			ki += originalMat->data[nodes[i]->pos][x];				// ki = total connection the node i have (total number of '1'
		}															// in the original matrix for node i)
		for(int j=0; j<B->col; j++){								//
			double kj=0;											//
			for(int y=0; y<copymat->col; y++){						// Calculating kj(degree of node j)
				kj += originalMat->data[nodes[j]->pos][y];			// ki = total connection the node i have (total number of '1'
			}														// in the original matrix for node i)
			B->data[i][j] = copymat->data[i][j] - (ki*kj/twoM);		// Modularity Matrix Bij = Aij - (ki*kj/2m)
		}															//
	}																//



	Matrix* STB = new Matrix;					//
	STB->col = S->row;							//
	STB->row = S->col;							//
	STB->data = new double*[STB->row];			// Creating a Matrix called STB, storing values of matrix S transpose multiplied by B
	for(int i=0; i<STB->row; i++){				//
		STB->data[i] = new double[STB->col];	//
	}											//

	for(int i=0; i<STB->row; i++){				//
		for(int j=0; j<STB->col; j++){			//
			STB->data[i][j] = 0;				// Initializing all elements of STB to 0
		}										//
	}											//

	for(int i=0; i<STB->row; i++){									//
		for(int j=0; j<STB->col; j++){								//
			for(int k=0; k<STB->col; k++){							//
				STB->data[i][j] += S->data[k][i] * B->data[k][j];	// STB = ST * B
			}														//
		}															//
	}																//



	Matrix* STBS = new Matrix;						//
	STBS->row = STBS->col = STB->row;				//
	STBS->data = new double*[STBS->row];			// Creating a Matrix called STBS, storing values of matrix STB multiplied by S
	for(int i=0; i<STBS->row; i++){					//
		STBS->data[i] = new double[STBS->col];		//
	}												//

	for(int i=0; i<STBS->row; i++){				//
		for(int j=0; j<STBS->col; j++){			//
			STBS->data[i][j] = 0;				// Initializing all elements of STB to 0
		}										//
	}											//

	for(int i=0; i<STBS->row; i++){										//
		for(int j=0; j<STBS->col; j++){									//
			for(int k=0; k<STB->col; k++){								//
				STBS->data[i][j] += STB->data[i][k] * S->data[k][j];	// STBS = STB * S
			}															//
		}																//
	}																	//



	// Summing all to Q
	// Q = (1/2m) * Trace((S transpose) * B * S)
	//---------------------------------------------------
	double Q=0;								// Defining Total Modularity as Q
	for(int i=0; i<STBS->row; i++){			//
		Q += STBS->data[i][i];				// Q = Trace((S transpose) * B * S)
	}										//
	Q /= twoM;								// Q = (1/2m) * Trace((S transpose) * B * S)



	//Releasing the dynamic memory allocated
	deleteMatrix(STBS);
	deleteMatrix(STB);
	deleteMatrix(B);
	deleteMatrix(S);

	return Q;

}

///////////////////////////////////////////////////////////////////////////
//						End of Modularity Function						 //
///////////////////////////////////////////////////////////////////////////


#endif /* MODULARITY_H_ */
