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
	// initialise to 0
	for(int i = 0;i < nV;i++){
		nodeV.values[i] = 0;
	}

	for(int i = 0;i < nV;i++){
		ShortestPaths p = dijkstra(g, i);
		//printf("p->src is %d\n",p.src);
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

static int find_betweeness(int value,PredNode *node,int src,NodeValues nodeV,Graph g);

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

		for (int j = 0;j < nV;j++){
			//printf("----%d----\n",j);
			int path = 0;
			PredNode* pred_node = p.pred[j];
			while (pred_node != NULL){
				pred_node = pred_node->next;
				path++;
			}
			//printf("PATH in main is %d\n",path);
			pred_node = p.pred[j];
			while (pred_node != NULL){
				//printf("In main predloop Pred %d : [%d] \n",j,pred_node->v);
				if (pred_node->v != i ){
						//printf("here again");
						find_betweeness(path,pred_node,i,nodeV,g);
				}
				break;
				pred_node = pred_node->next;
				//printf("path is %d\n",path);
			}
		}

	}

	return nodeV;
}

static int find_betweeness(int path,PredNode *pred_node,int src,NodeValues nodeV,Graph g){
	int node = 0;
	while (pred_node != NULL){
		//printf("pred->node->v is %d\n",pred_node->v);
		if (pred_node->v == src){
			//printf("here\n");
			return 1;
		}
		if (path > 1){
			double cal = 1/(double)path;
			//printf("cal is %f\n",cal);
			//printf("before add cal for %d is %f\n",pred_node->v,nodeV.values[pred_node->v]);
			nodeV.values[pred_node->v] = nodeV.values[pred_node->v] + cal;
			//printf("nodeV.values[%d] after cal is %f\n",pred_node->v,nodeV.values[pred_node->v]);
			//path = 1;
		} else {
			//printf("nodev values for %d is %f\n",pred_node->v,nodeV.values[pred_node->v]);
			nodeV.values[pred_node->v] = nodeV.values[pred_node->v] + 1;
			//printf("nodeV.values[%d] is %f\n",pred_node->v,nodeV.values[pred_node->v]);
		}

		node = pred_node->v;
		pred_node = pred_node->next;
	}
	//printf("after loop");

	ShortestPaths p = dijkstra(g, src);
	PredNode *nodep = p.pred[node];
	//printf("nodepv for %d = %d\n",node,nodep->v);
	return find_betweeness(path,nodep,src,nodeV,g);
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

void freeNodeValues(NodeValues val){

	// for(int i = 0;i < val.noNodes;i++){
	// 	free(val.values);
	// }
	free(val.values);
}
