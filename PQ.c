// PQ ADT interface for Ass2 (COMP2521)
#include "PQ.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define SIZE 1000
struct PQRep {
    ItemPQ *array;  // array of Items
    int  nitems;  // #items in array
    int  size;  // #capacity of PQ
};


PQ newPQ() {

	int N = SIZE;
	PQ new = malloc(sizeof(PQ));
	assert(new != NULL);
	new->array  = malloc(N * sizeof(ItemPQ));
    new->nitems = 0;
    new->size = N;

	return new;
}

/* Adds item (ItemPQ) to the priority queue.
   If an item with 'key' exists, it's 'value' is updated.
*/
void addPQ(PQ pq, ItemPQ element) {
	pq->array[pq->nitems] = element;
  pq->nitems++;

}


/* Removes and returns the item (ItemPQ) with smallest 'value'.
   For items with equal 'value', observes FIFO.
   Returns null if this queue is empty.
*/
ItemPQ dequeuePQ(PQ pq) {
	
    assert(pq != NULL);
    // deq item of smallest value in q
    int min = pq->array[0].value;
    int j = 0;
    for (int i = 0;i < pq->nitems;i++){
        if (min > pq->array[i].value){
	        min = pq->array[i].value;
	        j = i;
        }
    }

	// swap array
	ItemPQ temp = pq->array[j];
	pq->array[j] = pq->array[pq->nitems-1];

	pq->array[pq->nitems-1] = temp;
	pq->nitems--;

	return pq->array[pq->nitems];

}

/* Updates item with a given 'key' value, by updating that
 item's value to the given 'value'.
   If item with 'key' does not exist in the queue, no action is taken
*/
void updatePQ(PQ pq, ItemPQ element) {
	int elementkey = element.key;
	int elementvalue = element.value;

	for (int i = 0;i < pq->nitems;i++){
		if (elementkey == pq->array[i].key){
		    pq->array[i].value = elementvalue;
		}
	}


}

int PQEmpty(PQ pq) {
    if (pq->nitems == 0){
		return 1;
	}
	return 0;
}

void  showPQ(PQ pq) {
	assert(pq != NULL);

	for (int i = 0;i < pq->nitems;i++){
		printf("%d : key %d value %d",i,
		pq->array[i].key,pq->array[i].value);
	}
}

void  freePQ(PQ pq) {
  assert (pq != NULL);

	free(pq->array);
	free(pq);
}
