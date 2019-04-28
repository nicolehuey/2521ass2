// Dijkstra ADT interface for Ass2 (COMP2521)
#include "Dijkstra.h"
#include "PQ.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>

static void delete_predlist(PredNode **list, Vertex vertex);
static void insert_predlist(PredNode **list, Vertex vertex, Vertex pred);

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
		path->dist[i] = INT_MAX;
		path->pred[i] = NULL;
		ItemPQ vertex;
		if (i != v){
			vertex.key = i;
			vertex.value = INT_MAX;
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

	// set source key as visited
	visited [source.key] = 1;
	int n = 0;
	int distance = 0;

	while (!PQEmpty(q)) {
		n = 0;
		//  deq the smallest v
		ItemPQ deq = dequeuePQ(q);
		visited[deq.key] = 1;
		// check adjacent Nodes
		// if vertex has a distance of INTMAX,
		// implies that it's not any way connected to source node
		// hence initialise to 0
		if (path->dist[deq.key] == INT_MAX){
			path->dist[deq.key] = 0;
			n = 1;
		}
		// get the adjacency list of the deq node
	    AdjList list = outIncident(g,deq.key);
			while (list != NULL && n == 0){
				int adj_key = list->w;
                int adj_value = list->weight;
				
				// check whether the node is visited already
				if (visited[adj_key] == 0) {
				    // calculate new distance
					distance = path->dist[deq.key] + adj_value;

					// If the distance is same as another path taken,
					// Add it to the predecessor list.
					if (distance == path->dist[adj_key]) {
						insert_predlist(path->pred, adj_key, deq.key);

					// if the distance is smaller than current distance
					// chg path->dist[adj_key]
					} else if (distance < path->dist[adj_key]) {
						path->dist[adj_key] = distance;
						// clear the current pred list
						delete_predlist(path->pred, adj_key);
						// insert new adj value into pred list
						insert_predlist(path->pred, adj_key, deq.key);
						// update value to the new distance
						ItemPQ update;
						update.key = adj_key;
						update.value = distance;
						updatePQ(q, update);
					}
				}
				list = list->next;
			}
		}

	freePQ(q);
	return *path;
}

// add pred node into the pred list
static void insert_predlist(PredNode **list, Vertex vertex, Vertex pred) {

	// Allocate memory for new list.
	PredNode *new = malloc(sizeof(PredNode));
	new->v = pred;
	new->next = NULL;

	// If the list is empty, the new node is the start of the list.
	// Else, place the new node at the end of the list.
	if (list[vertex] == NULL) {
		list[vertex] = new;
	} else {
		PredNode *curr = list[vertex];
		PredNode *prev = list[vertex];

		// check if the node is alrd in the list
		while (curr != NULL){
			if (curr->v == pred) {
				return;
			}
			prev = curr;
			curr = curr->next;
		}
		prev->next = new;
	}
	
}

// Frees all nodes int the predecessor list.
static void delete_predlist(PredNode **list, Vertex vertex) {

	if (list[vertex] == NULL) {
		return;
	} else if (list[vertex]->next == NULL) {
		PredNode *curr = list[vertex];
		free(curr);
	} else {
		PredNode *prev = list[vertex];
		PredNode *curr = prev->next;
		while (curr != NULL) {
			free (prev);
			prev = curr;
			curr = curr->next;
		}
	}
	list[vertex] = NULL;
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
