// Dijkstra ADT interface for Ass2 (COMP2521)
#include "Dijkstra.h"
#include "PQ.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>


ShortestPaths dijkstra(Graph g, Vertex v) {

	PQ q = newPQ();
	int nV = g->nV;
	// set start node to 0
	// enq start node to PQ
  addPQ(pq,  );
	// do i have to malloc this
	ShortestPaths path = malloc(sizeof (ShortestPaths));
  // g->nV
	// set all vertices value to infinity

	// while queue is not Empty
	while(!PQEmpty(q)) {
		// deq the smallest v
		ItemPQ item = dequeuePQ(q);
		// check adjacent Nodes
		AdjList list = inIncident(g,item.value);
		// if adj nodes visited
		// calc the path from that node to the curr

		// check if new path is shorter

		// update path length in shortest paths
		// update the prev node as predecessor of the curr node

		// update the value priority in PQ

  }


	ShortestPaths throwAway = {0};
	return throwAway;
}

void showShortestPaths(ShortestPaths paths) {
	printf("%d",paths.src);
	// for (int i = 0;i < noNodes ;i++){
	// 	printf("-> %d",paths->pred);
	//
	// }

}


void  freeShortestPaths(ShortestPaths paths) {

}
