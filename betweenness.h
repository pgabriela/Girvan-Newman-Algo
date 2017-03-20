/*
 * betweenness.h
 *
 *  Created on: Mar 20, 2017
 *      Author: Petra
 */

#ifndef BETWEENNESS_H_
#define BETWEENNESS_H_
#include "matrix.h"
#include "int-queue.h"

using namespace std;



//Function for calculating betweenness using BFS
//==========================================================================================================================
//==========================================================================================================================



///////////////////////////////////////////////////////////////////////////
//						Start of Betweenness Function					 //
///////////////////////////////////////////////////////////////////////////

Matrix* betweenness(Matrix* test, Node* nodes[], Matrix* totalBet){

	//Initializing all elements in the totalBet matrix to 0
	for(int i=0; i<totalBet->row; i++){
		for(int j=0; j<totalBet->col; j++){
			totalBet->data[i][j] = 0;
		}
	}


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

		/*----------Calculate the number of shortest path for node i to each nodes----------*/

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






#endif /* BETWEENNESS_H_ */
