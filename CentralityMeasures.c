// Graph ADT interface for Ass2 (COMP2521)
#include "CentralityMeasures.h"
#include "Dijkstra.h"
#include "PQ.h"
#include <stdlib.h>
#include <stdio.h>

NodeValues outDegreeCentrality(Graph g){
	int nV = numVerticies(g);;
	NodeValues* nodeV = malloc(sizeof(NodeValues));
	nodeV->noNodes = nV;
	nodeV->values = malloc(nV* sizeof (double));
	int num = 0;
	for(int i = 0;i < nV;i++){
		num = 0;
		AdjList out = outIncident(g,i);
		while (out != NULL){
			num++;
			nodeV->values[i] = num;
			out = out->next;
		}
	}

	return *nodeV;
}
NodeValues inDegreeCentrality(Graph g){
	int nV = numVerticies(g);
	NodeValues* nodeV = malloc(sizeof(NodeValues));
	nodeV->noNodes = nV;
	nodeV->values = malloc(nV* sizeof (double));
	int num = 0;
	for(int i = 0;i < nV;i++){
		num = 0;
		AdjList in = inIncident(g,i);
		while (in != NULL){
			num++;
			nodeV->values[i] = num;
			in = in->next;
		}
	}

	return *nodeV;
}
NodeValues degreeCentrality(Graph g) {
	int nV = numVerticies(g);
	NodeValues* nodeV = malloc(sizeof(NodeValues));
	nodeV->noNodes = nV;
	nodeV->values = malloc(nV* sizeof (double));
	int num = 0;
	for(int i = 0;i < nV;i++){
		num = 0;
		AdjList in = inIncident(g,i);
		AdjList out = outIncident(g,i);
		while (out != NULL){
			num++;
			nodeV->values[i] = num;
			out = out->next;
		}
		while (in != NULL){
			num++;
			nodeV->values[i] = num;
			in = in->next;
		}
	}
	return *nodeV;
}

NodeValues closenessCentrality(Graph g){
	int nV = numVerticies(g);
	NodeValues* nodeV = malloc(sizeof(NodeValues));
	nodeV->noNodes = nV;
	nodeV->values = malloc(nV* sizeof (double));
	// initialise to 0
	for(int i = 0;i < nV;i++){
		nodeV->values[i] = 0;
	}

	for(int i = 0;i < nV;i++){
		ShortestPaths p = dijkstra(g, i);
		//printf("p->src is %d\n",p.src);
		int reach = 1;
		for (int j = 0;j < nV;j++){
			if (i != j && p.dist[j] != 0){
					//printf("src %d to %d is dist is %d\n",p.src,j,p.dist[j]);
					nodeV->values[i] = nodeV->values[i] +	p.dist[j];
					//printf("values for %d is %f\n",i,nodeV->values[i]);
					reach++;
			}
		}
		//printf("reach is %d\n",reach);
		if (reach == 1){
			nodeV->values[i] = 0;
		} else {
			double eqn = (double)((reach-1))/(double)((nV-1));
			//printf("(reach-1) is %d (nV-1) is %d eqn is %f\n",reach-1,nV-1, eqn);
			nodeV->values[i] = eqn * (double)(reach-1) / (double)(nodeV->values[i]);
		}//printf("HERE i is %d final values is %f\n",i,nodeV->values[i]);

	}
	return *nodeV;
}

NodeValues betweennessCentrality(Graph g){
	int nV = numVerticies(g);
	NodeValues* nodeV = malloc(sizeof(NodeValues));
	nodeV->noNodes = nV;
	nodeV->values = malloc(nV* sizeof (double));
	// initialise to 0
	for(int i = 0;i < nV;i++){
		nodeV->values[i] = 0;
	}
	// create an array to store number

	int array[nV];
	for(int i = 0;i < nV;i++){
		array[i] = 0;
	}

	for(int i = 0;i < nV;i++){
		ShortestPaths p = dijkstra(g, i);
		printf("Node: %d\n",i);
		for (int j = 0;j < nV;j++){
			printf("j : %d\n",j);
			int path = 0;
			PredNode* pred_node = p.pred[j];
			while (pred_node != NULL){
				pred_node = pred_node->next;
				path++;

			}
			printf("PATH is %d\n",path);
			pred_node = p.pred[j];
			while (pred_node != NULL){
				printf("Pred %d : [%d] \n",j,pred_node->v);
				//printf("%d: p.pred[%d]->v is %d\n",i,j,pred_node->v);
				if (pred_node->v != i ){
						printf("Add values\n");
						nodeV->values[pred_node->v] ++;
						printf(" values[%d] = %f\n",pred_node->v,nodeV->values[pred_node->v]);
						if (path > 1){
							array[pred_node->v] = path;
							printf("array[%d] is %d\n",pred_node->v,array[pred_node->v]);
						}
				}
				// if (path != 0){
				// 	printf("prednode->v  is %d\n",pred_node->v);
				// 	printf(" %f = %f /(%d + 1)\n",nodeV->values[prev_pred],nodeV->values[prev_pred],path);
				// 	nodeV->values[prev_pred] = nodeV->values[prev_pred] / (path+1);
				// 	printf("HEREEEEEEEEE %d : new calc is %f\n",j,nodeV->values[pred_node->v]);
				// }
				pred_node = pred_node->next;
				//printf("path is %d\n",path);
			}
		}

	}
	for(int i = 0;i < nV;i++){
		if (array[i] != 0){
			nodeV->values [i] = nodeV->values[i] / array[i];
		}

		printf("values for %d is %f \n",i,nodeV->values[i]);
	}
	return *nodeV;
}

NodeValues betweennessCentralityNormalised(Graph g){
	NodeValues throwAway = {0};
	return throwAway;
}

void showNodeValues(NodeValues values){
	//printf("Number of nodes = %d\n",values.noNodes);
	for(int i = 0;i < values.noNodes;i++){
		printf("%d: %f\n",i,values.values[i]);
	}
}

void freeNodeValues(NodeValues val){

	// for(int i = 0;i < val.noNodes;i++){
	// 	free(val.values);
	// }
	free(val.values);
}
