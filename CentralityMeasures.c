// Graph ADT interface for Ass2 (COMP2521)
#include "CentralityMeasures.h"
#include "Dijkstra.h"
#include "PQ.h"
#include <stdlib.h>
#include <stdio.h>

NodeValues outDegreeCentrality(Graph g){
	int nV = numVerticies(g);;
	NodeValues nodeV;
	nodeV.noNodes = nV;
	nodeV.values = malloc(nV* sizeof (double));
	int num = 0;
	for(int i = 0;i < nV;i++){
		num = 0;
		AdjList out = outIncident(g,i);
		if (out == NULL){
			nodeV.values[i] = 0;

		}
		while (out != NULL){
			num++;
			nodeV.values[i] = num;
			out = out->next;
		}
	}

	return nodeV;
}
NodeValues inDegreeCentrality(Graph g){
	int nV = numVerticies(g);
	NodeValues nodeV;
	nodeV.noNodes = nV;
	nodeV.values = malloc(nV* sizeof (double));
	int num = 0;
	for(int i = 0;i < nV;i++){
		num = 0;
		AdjList in = inIncident(g,i);
		if (in == NULL){
			nodeV.values[i] = 0;
		}
		while (in != NULL){
			num++;
			nodeV.values[i] = num;
			in = in->next;
		}
	}

	return nodeV;
}
NodeValues degreeCentrality(Graph g) {
	int nV = numVerticies(g);
	NodeValues nodeV;
	nodeV.noNodes = nV;
	nodeV.values = malloc(nV* sizeof (double));
	int num = 0;
	for(int i = 0;i < nV;i++){
		num = 0;
		AdjList in = inIncident(g,i);
		AdjList out = outIncident(g,i);
		while (out != NULL){
			num++;
			nodeV.values[i] = num;
			out = out->next;
		}
		while (in != NULL){
			num++;
			nodeV.values[i] = num;
			in = in->next;
		}
	}
	return nodeV;
}

NodeValues closenessCentrality(Graph g){
	int nV = numVerticies(g);
	NodeValues nodeV;
	nodeV.noNodes = nV;
	nodeV.values = malloc(nV* sizeof (double));

	// initialise values to 0
	for(int i = 0;i < nV;i++){
		nodeV.values[i] = 0;
	}

	for(int i = 0;i < nV;i++){
		ShortestPaths p = dijkstra(g, i);
		int reach = 1;
		for (int j = 0;j < nV;j++){
			if (i != j && p.dist[j] != 0){
					//printf("src %d to %d is dist is %d\n",p.src,j,p.dist[j]);
					nodeV.values[i] = nodeV.values[i] +	p.dist[j];
					//printf("values for %d is %f\n",i,nodeV.values[i]);
					reach++;
			}
		}
		//printf("reach is %d\n",reach);
		if (reach == 1){
			nodeV.values[i] = 0;
		} else {
			double eqn = (double)((reach-1))/(double)((nV-1));
			//printf("(reach-1) is %d (nV-1) is %d eqn is %f\n",reach-1,nV-1, eqn);
			nodeV.values[i] = eqn * (double)(reach-1) / (double)(nodeV.values[i]);
		}//printf("HERE i is %d final values is %f\n",i,nodeV.values[i]);

	}
	return nodeV;
}

//static int find_betweeness(int path,int *array,PredNode *pred_node,NodeValues nodeV,int src,Graph g);

// static void printarray(int *array,int nV){
// 	for(int i = 0;i < nV;i++){
// 		printf("array[%d] is %d\n",i,array[i]);
// 	}
// }

// static double searchnode(ShortestPaths p, int curr, int dest, double *count) {
// 	if (curr == p.src) {
// 		return 1;
// 	}
//
// 	double paths = 0;
//
// 	PredNode *temp = p.pred[curr];
// 	while (temp != NULL) {
// 		paths += searchnode(p, temp->v, dest, count);
// 		temp = temp->next;
// 	}
// 	if (curr == dest) {
// 		(*count) += paths;
// 	}
// 	return paths;
// }
static double find_betweeness(ShortestPaths p,int node,int j,double *count);
NodeValues betweennessCentrality(Graph g){
	int nV = numVerticies(g);
	NodeValues nodeV;
	nodeV.noNodes = nV;
	nodeV.values = malloc(nV* sizeof (double));
	// initialise to 0
	for(int i = 0;i < nV;i++){
		nodeV.values[i] = 0;
	}

	for(int i = 0;i < nV;i++){
		ShortestPaths p = dijkstra(g, i);
		//printf(" ********** NODE: %d *************\n",i);

		for(int j = 0;j < nV;j++){
			if (i == j) continue;
			//printf("------ j = %d ---- \n",j);
			for (int pred = 0; pred < nodeV.noNodes; pred++) {
				// for node that is not source or destination
				// find the path and check if it leads to destination
				if ((pred != i) && (pred != j)){
					//printf("pred: %d\n",pred);
					double count = 0;
					double paths = 0;
					paths = find_betweeness (p,pred,j, &count);
					//printf("path[%d] is %f and count is %f\n",j,paths,count);
					if (paths != 0) {
						nodeV.values[j] += (count / paths);
						//printf("nodevalues[%d] = %f\n",j,nodeV.values[j]);
					}
				}
			}
		}
		freeShortestPaths(p);
	}

	return nodeV;
}

static double find_betweeness(ShortestPaths p,int node,int dest,double *count){
		// return if the pred node is the source
		double paths = 0;
		if (node == p.src){
			return 1;
		}
		else {
			//printf("node %d, dest %d\n",node,dest);
			PredNode *curr = p.pred[node];
			while (curr != NULL){
				paths = paths + find_betweeness(p,curr->v,dest,count);
				//printf("path in func = %f,curr->v is %d\n",paths,curr->v);
				curr = curr->next;
			}

			if (node == dest){
				*count += paths;
			}
		}

	//printf("PATH is %d\n",path);
	return paths;
}

NodeValues betweennessCentralityNormalised(Graph g){
	double nV = numVerticies(g);
	NodeValues nodeV = betweennessCentrality(g);
	for(int i = 0;i < nV;i++){
		double cal = 1 /((nV-1) *(nV-2));
		nodeV.values[i] = cal * nodeV.values[i];
	}
	return nodeV;
}

void showNodeValues(NodeValues values){
	//printf("Number of nodes = %d\n",values.noNodes);
	for(int i = 0;i < values.noNodes;i++){
		printf("%d: %f\n",i,values.values[i]);
	}
}

void freeNodeValues(NodeValues values){

	free(values.values);
}
