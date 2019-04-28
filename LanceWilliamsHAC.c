
/*Lance-Williams Algorithm for Agglomerative Clustering
  Written by Livia and Yin Huey 
  COMP2521 2019 T1
*/
#include <stdlib.h>
#include <stdio.h>
#include "LanceWilliamsHAC.h"
#include "Graph.h"
#include <math.h>
#include <limits.h>
#include <assert.h>
#include <string.h>
#define numVertices numVerticies
#define TRUE 1
#define FALSE 0


static Dendrogram *initialiseArray (int nV);
static int         initDistances   (Graph g,int nV, double distanceArray[nV][nV]);
static Dendrogram  distanceMin     (int nV, double distanceArray[nV][nV], Dendrogram *dendArray, int *numD);
static Dendrogram distanceMax      (int nV, double distanceArray[nV][nV], Dendrogram *dendArray, int *numD);
static Dendrogram  newDendrogram   (int value);
static Dendrogram insertDend       (Dendrogram left, Dendrogram right);
static bool haveCluster            (int index, double distanceArray[]); 
static void initArray              (int nV,  double distanceArray[nV][nV]); 
static void putClusterIntoArray    (Dendrogram d, Dendrogram *array, int *index);
static void updateArray            (Dendrogram *dendArray, Dendrogram d, int position);
static int  whichClust  (Dendrogram *dendArray, int value, int arraySize); 
static bool searchIndex (Dendrogram d, int value);
static bool sameClust   (Dendrogram *dendArray, int valueL, int valueR, int arraySize);
static void updateDistanceMax (int nV, double distanceArray[nV][nV], int left, int right);

/* 
 * Finds Dendrogram using Lance-Williams algorithm (as discussed in the specs) 
   for the given graph g and the specified method for agglomerative clustering.
   Method value 1 represents 'Single linkage', and 2 represents 'Complete linkage'.
   For this assignment, you only need to implement the above two methods. 

   The function returns 'Dendrogram' structure (binary tree) with the required information.
 * 
 */

// assumptions: every node is connected to all other nodes
// whether not direct or directly within the graph
Dendrogram LanceWilliamsHAC(Graph g, int method) {

    int nV = numVerticies(g);
    
    
    Dendrogram* dendArray = initialiseArray(nV);
    double distanceArray[nV][nV];
    
    initArray(nV, distanceArray);  
    

    int connection = initDistances(g, nV, distanceArray);
    
    if (connection < 2) {        
        return NULL;        
    } 
    
    int numD = 0;
    Dendrogram dend;
    if (method == 1) {
        for (int i = 0; i < (nV - 1); i++) {
            dend = distanceMin(nV, distanceArray, dendArray, &numD);
        }
        
    } else {
        for (int i = 0; i < (nV - 1); i++) {
            dend = distanceMax(nV, distanceArray, dendArray, &numD);
        }    
    
    }
    assert(dend != NULL);
    // Cannot free my array because inside dend there is pointing
    // somewhere in the array, which is another dendrogram

    return dend;
}


void freeDendrogram(Dendrogram d) {
	if (d == NULL) return;
	freeDendrogram (d->left);
	freeDendrogram (d->right);
	free (d);

}

//numD = number of dendrograms inside the array
static Dendrogram distanceMin(int nV, double distanceArray[nV][nV], Dendrogram *dendArray, int *numD) {
    //int index = *numD;
    
    double length = INT_MAX;
    int left = 0;
    int right = 0;
    for (int i = 0; i < nV; i++) {
        for (int j = 0; j < nV; j++) {
            if ((distanceArray[i][j] != INT_MAX) && (distanceArray[i][j] < length) && (distanceArray[i][j] != -1)) {                
                if (sameClust (dendArray, i, j, *numD) == FALSE) {
                    length = distanceArray[i][j];
                    left = i;
                    right = j;
                }
            } 
        }
    }
    
    int clusterLeft  = haveCluster (nV,  distanceArray[left]);
    int clusterRight = haveCluster (nV, distanceArray[right]);
    Dendrogram new;
    
    if (clusterLeft && clusterRight) {
        int positionL = whichClust (dendArray, left, *numD);
        int positionR = whichClust (dendArray, right, *numD);
        new = insertDend(dendArray[positionR], dendArray[positionL]); 
        if (positionL < positionR) {
            dendArray[positionL] = new;
            dendArray[positionR] = NULL;
        } else {
            dendArray[positionL] = NULL;
            dendArray[positionR] = new;        
        }
    } else if (clusterRight) {       
        int position = whichClust (dendArray, right, *numD);
        new = insertDend(newDendrogram(left), dendArray[position]); 
        updateArray (dendArray, new, position);
    } else if (clusterLeft) {
        int position = whichClust (dendArray, left, *numD);
        new = insertDend(newDendrogram(right), dendArray[position]);
        // update dendArray -> put the new dendrogram into the correct index
        updateArray (dendArray, new, position);
    // the first cluster to be make
    } else {
        new = insertDend(newDendrogram(right), newDendrogram(left));
        putClusterIntoArray(new, dendArray, numD);
        

    }
    distanceArray[left][right] = -1;
    distanceArray[right][left] = -1;
    
    return new;   
        
}


static Dendrogram distanceMax(int nV, double distanceArray[nV][nV], Dendrogram *dendArray,  int *numD) {
    double length = INT_MAX;
    int left = 0;
    int right = 0;
    for (int i = 0; i < nV; i++) {
        for (int j = 0; j < nV; j++) {
            if ((distanceArray[i][j] != INT_MAX) && (distanceArray[i][j] < length) && (distanceArray[i][j] != -1)) {                
                if (sameClust (dendArray, i, j, *numD) == FALSE) {
                    length = distanceArray[i][j];
                    left = i;
                    right = j;
                }
            } 
        }
    }

    
    int clusterLeft  = haveCluster (nV,  distanceArray[left]);
    int clusterRight = haveCluster (nV, distanceArray[right]);

    Dendrogram new;
    
    if (clusterLeft && clusterRight) {
        int positionL = whichClust (dendArray, left, *numD);
        int positionR = whichClust (dendArray, right, *numD);
        new = insertDend(dendArray[positionR], dendArray[positionL]); 
        if (positionL < positionR) {
            dendArray[positionL] = new;
            dendArray[positionR] = NULL;
        } else {
            dendArray[positionL] = NULL;
            dendArray[positionR] = new;        
        }
    } else if (clusterRight) {     
        int position = whichClust (dendArray, right, *numD);
        new = insertDend(newDendrogram(left), dendArray[position]); 
        updateArray (dendArray, new, position);
    } else if (clusterLeft) {
        int position = whichClust (dendArray, left, *numD);
        new = insertDend(newDendrogram(right), dendArray[position]);
        // update dendArray -> put the new dendrogram into the correct index
        updateArray (dendArray, new, position);
    // the first cluster to be make
    } else {
        new = insertDend(newDendrogram(right), newDendrogram(left));
        putClusterIntoArray(new, dendArray, numD);
    }
    updateDistanceMax (nV, distanceArray, left, right);
    distanceArray[left][right] = -1;
    distanceArray[right][left] = -1;
    
    return new;   
        
}

static void updateArray (Dendrogram *dendArray, Dendrogram d, int position) {
    dendArray[position] = d;
}

// a checker if two values are in the same array
static bool sameClust (Dendrogram *dendArray, int valueL, int valueR, int aSize) {
    for (int i = 0; i < aSize; i++) {        
        if (searchIndex (dendArray[i], valueL) && searchIndex (dendArray[i], valueR)) {
            return TRUE;
        }
    }
    return FALSE;

}

static int whichClust (Dendrogram *dendArray, int value, int arraySize) {
    for (int i = 0; i < arraySize; i++) {
        
        if (searchIndex (dendArray[i], value)) {
            return i;
        }
    }
    return -1;

}

// look for nodes with value of int value inside dendrogram
// recursively, it will return TRUE if found
static bool searchIndex (Dendrogram d, int value) {
    
    int returnVal = FALSE;
    if (d == NULL) return FALSE;
    if (d->vertex == value) {
        return TRUE;
    }
        
    if (d->left != NULL) {
        returnVal = returnVal || searchIndex (d->left, value);
    } 
    if (d->right != NULL) {
        returnVal = returnVal || searchIndex (d->right, value);
    }
    return returnVal;

}

static Dendrogram newDendrogram (int value) {
    Dendrogram new =  malloc(sizeof(DNode));
    new->vertex = value;
    new->right = NULL;
    new->left = NULL;
    return new;
}

// a checker is a value is already a part of a cluster (a dendrogram)
static bool haveCluster (int index, double distanceArray[]) {
    for (int i = 0; i < index; i++) {
        if(distanceArray[i] == -1) {
            return TRUE;
        }
    }
    return FALSE;

}

static Dendrogram insertDend (Dendrogram left, Dendrogram right) {
    Dendrogram new =  malloc(sizeof(DNode));
    new->vertex = -1;
    new->right = right;
    new->left = left;
    return new;
}

//update array so that it only records the greater distance between two clusters
// or between two vertices
static void updateDistanceMax (int nV, double distanceArray[nV][nV], int left, int right) {
    for (int i = 0; i < nV; i++) {
        if ((distanceArray[i][left] > distanceArray[i][right]) && 
            (distanceArray[i][left] != INT_MAX) && (distanceArray[i][right] != -1)) {
            distanceArray[i][right] = distanceArray[i][left];
            distanceArray[right][i] = distanceArray[left][i];
            
        } else if ((distanceArray[i][right] > distanceArray[i][left]) && 
                   (distanceArray[i][right] != INT_MAX) && (distanceArray[i][left] != -1)) {
            distanceArray[i][left] = distanceArray[i][right];
            distanceArray[left][i] = distanceArray[right][i];
            
        } 
    }
    
}

//placing clusters into array of dendrogram
static void putClusterIntoArray (Dendrogram d, Dendrogram *array, int *size) {
    
    int temp = *size;    
    array[temp] = d;
    *size = *size + 1;
}
// initilise 2D array of distances or weights between the nodes
static void initArray(int nV,  double distanceArray[nV][nV]) {
    for (int i = 0; i < nV; i++) {
        for (int j = 0; j < nV; j++) {
            distanceArray[i][j] = 0;        
        }
    }

}

// calculating the distances or weights and place them into the array 
static int initDistances(Graph g, int nV, double distanceArray[nV][nV]) {
    int notConnection = 0;
    for (int i = 0; i < nV; i++) {
        for (int j = 0; j < nV; j++) {
            if ((i != j) && (adjacent(g, i, j) || adjacent(g, j, i))) {
                AdjList outList = outIncident(g, i);
                while (outList != NULL) {
                    if (outList->w == j) {
                        distanceArray[i][j] = 1/(double)(outList->weight);
                    }
                    outList = outList->next;
                }
                AdjList inList = inIncident(g, i);
                while (inList != NULL) {
                    if (inList->w == j) {
                    // look for greater distance, which is smaller 2/distance
                        if (((1/(double)(inList->weight)) < distanceArray[i][j]) || (distanceArray[i][j] == 0)) {
                            distanceArray[i][j] = 1/(double)(inList->weight);
                            
                        }
                    }
                    inList = inList->next;
                }
            // set as INT_MAX is j = i or if they're not adjacent    
            } else {
                // only change into -1 if there is a cluster
                distanceArray[i][j] = INT_MAX;
                notConnection++;
            }
        }
    }
    //returns number of connections between 2 vertices
    return (nV * nV) - notConnection;

}

// initialise the array of dendrograms
static Dendrogram *initialiseArray (int nV) {

    Dendrogram *dendArray = malloc(sizeof(Dendrogram)*nV); 
    for (int i = 0; i < nV; i++) {
        dendArray[i] = malloc(sizeof(Dendrogram));
        dendArray[i]->vertex = -1;
        dendArray[i]->right = NULL;
        dendArray[i]->left = NULL;
    }

    return dendArray;
}
