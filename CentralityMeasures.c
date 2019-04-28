// Graph ADT interface for Ass2 (COMP2521)
#include "CentralityMeasures.h"
#include "Dijkstra.h"
#include "PQ.h"
#include <stdlib.h>
#include <stdio.h>

static double find_betweeness(ShortestPaths p,int node,int j,double *count);

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
					nodeV.values[i] = nodeV.values[i] +	p.dist[j];
					reach++;
			}
		}

		if (reach == 1){
			nodeV.values[i] = 0;
		} else {
			double eqn = (double)((reach-1))/(double)((nV-1));
			nodeV.values[i] = eqn * (double)(reach-1) / (double)(nodeV.values[i]);
		}
	}
	
	return nodeV;
}


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

		for(int j = 0;j < nV;j++){
			if (i == j) continue;
			for (int pred = 0; pred < nodeV.noNodes; pred++) {
				// for node that is not source or destination
				// find the path and check if it leads to destination
				if ((pred != i) && (pred != j)){
					double count = 0;
					double paths = 0;
					paths = find_betweeness (p,pred,j, &count);
					if (paths != 0) {
						nodeV.values[j] += (count / paths);
					}
				}
			}
		}
	}

	return nodeV;
}

static double find_betweeness(ShortestPaths p,int node,int currnode,double *count){
		
		// return 1 if the pred node reaches the source
		double paths = 0;
		if (node == p.src){
			return 1;
		}
		else {
		    // find all the paths
			PredNode *curr = p.pred[node];
			while (curr != NULL){
				paths = paths + find_betweeness(p,curr->v,currnode,count);
				curr = curr->next;
			}
			// add count if node is the currnode
			if (node == currnode){
				*count += paths;
			}
		}

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

	for(int i = 0;i < values.noNodes;i++){
		printf("%d: %f\n",i,values.values[i]);
	}
}

void freeNodeValues(NodeValues values){

	free(values.values);
}
