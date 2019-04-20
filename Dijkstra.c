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

   int visited[nV];
	 for (int i = 0;i < nV;i++){
		 visited[i] = 0;
	 }
	 ShortestPaths* path = malloc(sizeof (ShortestPaths));
	 path->noNodes = nV;
	 path->src = v;
	 path->dist = malloc(nV* sizeof (int));
	 path->pred = malloc(nV * sizeof (PredNode *));




	// intialise all the dist and prev for vertex in Graph
	for (int i = 0;i < nV;i++){
			path->dist[i] = 0;
			path->pred[i] = NULL;
	}

	path->pred[v]= NULL;
	path->dist[v] = 0;

	// item is the source node
 	ItemPQ source;
	source.key = v;
	source.value = 0;
	addPQ(q,source);

	visited [source.key] = 1;

	while (!PQEmpty(q)) {
		//  deq the smallest v
		ItemPQ deq = dequeuePQ(q);
		// // check adjacent Nodes
	  //printf("deq item is {%d,%d}\n",deq.key,deq.value);
	  AdjList list = outIncident(g,deq.key);

		// fill adjacent nodes into q
			while (list != NULL){
				// put into item
				ItemPQ adjitem;
				adjitem.key = list->w;
				adjitem.value = list->weight;

				if (visited[adjitem.key] == 0){
					addPQ(q,adjitem);
					//printf("adj item is {%d,%d}\n",adjitem.key,adjitem.value);
					visited[adjitem.key] = 1;
					// insert the distance
					if (deq.key == source.key){
						path->dist[adjitem.key] = adjitem.value;
							//printf("path->dist[%d] is %d\n",adjitem.key,path->dist[adjitem.key]);
					} else {
						//printf("adjitem.value %d + path[dist] value %d\n",adjitem.value, path->dist[deq.key]);
						path->dist[adjitem.key] = adjitem.value + path->dist[deq.key];
						//printf("path->dist[%d] is %d\n",adjitem.key,path->dist[adjitem.key]);
					}

					// add predecessor node
						PredNode* node = malloc (sizeof(node));
						node->v = deq.key;
						node->next = NULL;
						path->pred[adjitem.key] = node;

				} else if (visited[adjitem.key] == 1){
					  //printf("visited adjitem.key %d\n",adjitem.key);

						int alt = adjitem.value + path->dist[deq.key];
						//printf("comapre with %d\n",path->dist[adjitem.key] );
						//printf("alt = %d + %d\n",adjitem.value,path->dist[deq.key]);
						if (path->dist[adjitem.key] > alt){
							//printf("ALT IS SMALLER\n");
							path->dist[adjitem.key] = alt;
							// use update pq
							ItemPQ upditem;
							upditem.key = adjitem.key;
							upditem.value = adjitem.value;
							updatePQ(q,upditem);
							PredNode* node = malloc (sizeof(node));
							node->v = deq.key;
							node->next = NULL;
							path->pred[adjitem.key] = node;

						} else if (path->dist[adjitem.key] == alt){
								PredNode *new = malloc (sizeof(new));
								new->v = deq.key;
								new->next = path->pred[adjitem.key];
								path->pred[adjitem.key] = new;

						}
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
