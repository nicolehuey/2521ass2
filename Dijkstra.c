// Dijkstra ADT interface for Ass2 (COMP2521)
#include "Dijkstra.h"
#include "PQ.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>

ShortestPaths dijkstra(Graph g, Vertex v) {

	 PQ q = newPQ();
	 int nV = numVerticies(g);

   // int visited[nV];
	 // for (int i = 0;i < nV;i++){
		//  visited[i] = 0;
	 // }
	 ShortestPaths* path = malloc(sizeof (ShortestPaths));
	 path->noNodes = nV;
	 path->src = v;
	 path->dist = malloc(nV* sizeof (int));
	 path->pred = malloc(nV * sizeof (PredNode *));

	// intialise all the dist and prev for vertex in Graph
	for (int i = 0;i < nV;i++){
			path->dist[i] = INT_MAX;
			path->pred[i] = NULL;
			ItemPQ vertex;
			if (i != v){
				vertex.key = i;
				vertex.value = 100;
				addPQ(q,vertex);
			}
	}

	path->pred[v]= NULL;
	path->dist[v] = 0;

	// item is the source node
 	ItemPQ source;
	source.key = v;
	source.value = 0;
	addPQ(q,source);

	//visited [source.key] = 1;
	int n = 0;
	while (!PQEmpty(q)) {
		//  deq the smallest v
		ItemPQ deq = dequeuePQ(q);
		// // check adjacent Nodes
		if (path->dist[deq.key] == INT_MAX){
			//printf("change to 0\n");
			path->dist[deq.key] = 0;
			n = 1;
		//	printf("path->dist[%d] = %d\n",i,path->dist[i]);
		}
	  AdjList list = outIncident(g,deq.key);

		// fill adjacent nodes into q
			while (list != NULL && n == 0){
				// put into item
				ItemPQ adjitem;
				adjitem.key = list->w;
				adjitem.value = list->weight;

				int alt = path->dist[deq.key] + adjitem.value;
				//printf("alt %d = deq.key [%d] %d + adjitem [%d] %d\n",
				//malt,deq.key,path->dist[deq.key],adjitem.key,adjitem.value );
				if (alt < path->dist[adjitem.key]){
					path->dist[adjitem.key] = alt;
					PredNode* node = malloc (sizeof(node));
					node->v = deq.key;
					node->next = NULL;
					path->pred[adjitem.key] = node;
					ItemPQ upditem;
					upditem.key = adjitem.key;
					upditem.value = adjitem.value;
					updatePQ(q,upditem);
				} else if (alt == path->dist[adjitem.key]){
					PredNode *new = malloc (sizeof(new));
					new->v = deq.key;
					new->next = path->pred[adjitem.key];
					path->pred[adjitem.key] = new;
					// ItemPQ upditem;
					// upditem.key = adjitem.key;
					// upditem.value = adjitem.value;
					// updatePQ(q,upditem);
				}
				list = list->next;

			}

		}

	return *path;
}

void showShortestPaths(ShortestPaths paths) {
	printf("source node is %d \n",paths.src);
	for (int i = 0;i < paths.noNodes;i++){
			if (i != paths.src){
				printf("distance from source is %d\n",paths.dist[i]);
			}

			printf("predecessors are ");
			PredNode* p = paths.pred[i];
			while (p != NULL){
				printf("%d->",p->v);
				p = p->next;
			}
			printf("NULL\n");
	}

}


void  freeShortestPaths(ShortestPaths paths) {

		for (int i = 0;i < paths.noNodes;i++){
			PredNode *next = paths.pred[i];
			while(next != NULL){
	    PredNode *temp = next -> next;
	    free(next);
	    next = temp;
	  	}
		}

		free(paths.dist);

}
