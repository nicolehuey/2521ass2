// Graph ADT interface for Ass2 (COMP2521)
#include <stdlib.h>
#include <stdio.h>
#include<assert.h>
#include "Graph.h"


struct GraphRep{
	int nV;		 // #vertices
	int nE;		 // #edges
	AdjList *List;
};

Graph newGraph(int noNodes) {
	assert (noNodes > 0);

	Graph new = malloc (sizeof *new);
	assert (new != NULL);
	new->nV = noNodes;
	new->nE = 0;
  new->List = malloc(noNodes * sizeof(AdjList));
	int i;
  for (i = 0; i < noNodes;i++)
      new->List[i] = NULL;

	return new;
}

// A utility function to create a new adjacency list node
static AdjList newAdjListNode(Vertex dest,int weight)
{
    AdjList newNode = malloc(sizeof(AdjList));
    newNode->w = dest;
		newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}

void  insertEdge(Graph g, Vertex src, Vertex dest, int weight) {
	assert (g != NULL);

	  // Add an edge from src to dest.  A new node is
    // added to the adjacency list of src.
    AdjList newNode = newAdjListNode(dest,weight);
    newNode->next = g->List[src];
    g->List[src]= newNode;

		g->nE++;

}

void  removeEdge(Graph g, Vertex src, Vertex dest) {
	assert(g != NULL);

	 AdjList curr = g->List[src]->next;
	 AdjList prev = g->List[src];
	 if (prev->w == dest){
		 g->List[src] = g->List[src]->next;
		 free(prev);
	 }
	 while (curr != NULL){
    if (curr->w == dest){
			// if its at the first node after source
			if (curr == prev->next) {
				prev->next = curr->next;
				free(curr);
				break;
			// if its the last node
			} else if (curr->next == NULL){
				prev->next = NULL;
				free(curr);
				break;
			} else {
			 	prev->next = curr->next;
			 	free(curr);
			 	break;
		 }

		}
	curr = curr->next;
	prev = prev->next;
	}
	g->nE--;
}

bool adjacent(Graph g, Vertex src, Vertex dest) {
	AdjList curr = g->List[src];
	while(curr != NULL){
    if (curr->w == dest){
			return 1;
		}
		curr = curr->next;
	}

	return 0;
}

int numVerticies(Graph g) {
	return g->nV;
}

// Returns a list of adjacent vertices on outgoing edges from a given vertex.
AdjList outIncident(Graph g, Vertex v) {
	return g->List[v];
}


// Returns a list of adjacent vertices on incoming edges from a given vertex.
AdjList inIncident(Graph g, Vertex v) {

	AdjList new;
  AdjList p;
	int j = 0;
	for (int i = 0;i < g->nV; i++){
	AdjList curr = g->List[i];
		while(curr != NULL){
	    if (curr->w == v){
				if (j == 0){
					p = newAdjListNode(i,curr->weight);
					j ++;
				} else {
					new = newAdjListNode(i,curr->weight);
					new->next = p;
					p = new;
				}

		  }
			curr= curr->next;
		}
	}
  if (j != 0){
		return p;
	}
	return NULL;
}

void  showGraph(Graph g) {
	assert (g != NULL);
	printf ("#vertices=%d, #edges=%d\n\n", g->nV, g->nE);

  int v;
  for (v = 0; v < g->nV; ++v)
  {
      AdjList adj = g->List[v];
      printf("\n Adjacency list of vertex %d\n ", v);
      while (adj)
      {
          printf("-> %d", adj->w);
          adj = adj->next;
      }
      printf("\n");
  }

}

void  freeGraph(Graph g) {
	assert (g != NULL);
	for (int v = 0; v < g->nV; v++){
		AdjList curr = g->List[v];
		while(curr != NULL){
			AdjList next = curr->next;
			free(curr);
			curr = next;
		}
	}

	free (g);
}
