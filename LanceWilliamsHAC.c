/*Lance-Williams Algorithm for Agglomerative Clustering
  Written by 
  COMP2521 2019 T1
*/
#include <stdlib.h>
#include <stdio.h>
#include "LanceWilliamsHAC.h"
#include "Graph.h"
#include <math.h>
#include <limits.h>
#define numVertices numVerticies
#define TRUE 1
#define FALSE 0

typedef struct cluster *Cluster;
typedef struct cluster {
    int value;
    struct cluster *next;
} cluster;

static Dendrogram *initialiseArray (int nV);
static int         initDistances   (Graph g,int nV, double distanceArray[nV][nV]);
static Dendrogram  distanceMin     (int nV, double distanceArray[nV][nV], Dendrogram *dendArray);
static Dendrogram  distanceMax     (int nV, double distanceArray[nV][nV], Dendrogram *dendArray); 
static Dendrogram  newDendrogram   (int value);
//static Cluster insertToCluster     (Cluster head, int value);
static Dendrogram insertDend       (Dendrogram left, Dendrogram right);
static bool haveCluster            (int index, double distanceArray[]); 
static void initArray(int nV,  double distanceArray[nV][nV]); 
/* 
 * Finds Dendrogram using Lance-Williams algorithm (as discussed in the specs) 
   for the given graph g and the specified method for agglomerative clustering.
   Method value 1 represents 'Single linkage', and 2 represents 'Complete linkage'.
   For this assignment, you only need to implement the above two methods. 

   The function returns 'Dendrogram' structure (binary tree) with the required information.
 * 
 */


Dendrogram LanceWilliamsHAC(Graph g, int method) {
    /*
    // ==== TESTING ========
    Dendrogram v = malloc(sizeof(DNode));
    v->vertex = 9;
    v->right = NULL;
    v->left = NULL;
    return v;
    */
    int nV = numVerticies(g);
    
    
    Dendrogram* dendArray = initialiseArray(nV);
    //return insertDend (newDendrogram (1), newDendrogram (5)); 
    double distanceArray[nV][nV];
    
    initArray(nV, distanceArray);  
    int connection = initDistances(g, nV, distanceArray);
    if (connection == 0) {
        
        return NULL;
    }
    
    Dendrogram dend;
    if (method == 1) {
        for (int i = 0; i < 1; i++) {
            dend = distanceMin(nV, distanceArray, dendArray);
        }
    
    } else {
        for (int i = 0; i < connection; i++) {
            dend = distanceMax(nV, distanceArray, dendArray);
        }    
    
    }
    return dend;
}


void freeDendrogram(Dendrogram d) {
	if (d == NULL) return;
	freeDendrogram (d->left);
	freeDendrogram (d->right);
	free (d);

}

/*
static Cluster insertToCluster (Cluster head, int value) {
    
    if (head == NULL) {
        Cluster head = malloc(sizeof(cluster));
        head->value = value;
        head->next = NULL;
    } else {
        Cluster curr = head;
        for (;curr->next != NULL; curr = curr->next){
            if (curr->value == value) {
                return head;
            }
        }
        curr->next = malloc(sizeof(cluster));
        curr->next->value = value;
        curr->next->next = NULL;     
    }
    return head;
    

}

static bool clusteredElement (Cluster head, int value) {
    Cluster curr = head;
    while (curr->next != NULL) {
        if (curr->value == value) {
            return TRUE;
        }
        curr = curr->next;
    }
    return FALSE;
}
*/
static Dendrogram distanceMin(int nV, double distanceArray[nV][nV], Dendrogram *dendArray) {
    int length = distanceArray[0][1];
    int left = 0;
    int right = 0;
    for (int i = 0; i < nV; i++) {
        for (int j = 0; j < nV; j++) {
            if (distanceArray[i][j] != INT_MAX && distanceArray[i][j] < length && distanceArray[i][j] != -1) {
                length = distanceArray[i][j];
                left = i;
                right = j;
            } 
        }
    }
    //if (clusteredElement
    
    //insertToCluster (linkedClust, left);
    //insertToCluster (linkedClust, right);
    
    int clusterLeft  = haveCluster (left,  distanceArray[left]);
    int clusterRight = haveCluster (right, distanceArray[right]);
    Dendrogram new;
    if (clusterLeft && clusterRight) {
        new = insertDend(dendArray[right], dendArray[left]);
    } else if (clusterRight) {
        new = insertDend(newDendrogram(left), dendArray[left]); 
    } else if (clusterLeft) {
        new = insertDend(newDendrogram(right), dendArray[left]);
    } else {
        new = insertDend(newDendrogram(right), newDendrogram(left));
    }
    dendArray[left] = new;
    dendArray[right] = NULL;    
    distanceArray[left][right] = -1;
    distanceArray[right][left] = -1;
    return new;   
        
}

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
    new->right = right;
    new->left = left;
    return new;
}

static Dendrogram distanceMax(int nV, double distanceArray[nV][nV], Dendrogram *dendArray) {
    int length = 0;
    int left = 0;
    int right = 0;
    for (int i = 0; i < nV; i++) {
        for (int j = 0; j < nV; j++) {
            if (distanceArray[i][j] != INT_MAX && distanceArray[i][j] > length && distanceArray[i][j] != -1) {
                length = distanceArray[i][j];
                left = i;
                right = j;
            }
        }
    }
    //insertToCluster (linkedClust, left);
    //insertToCluster (linkedClust, right);    
    /*
    distanceArray[left][right] = INT_MAX;
    distanceArray[left][right] = INT_MAX;
    Dendrogram new =  malloc(sizeof(DNode));
    new->right = newDendrogram(right);
    new->left = newDendrogram(left);
    return new;    
    */
    int clusterLeft = haveCluster (left, distanceArray[left]);
    int clusterRight = haveCluster (right, distanceArray[right]);
    Dendrogram new;
    if (clusterLeft && clusterRight) {
        new = insertDend(dendArray[right], dendArray[left]);
    } else if (clusterRight) {
        new = insertDend(newDendrogram(left), dendArray[left]); 
    } else if (clusterLeft) {
        new = insertDend(newDendrogram(right), dendArray[left]);
    } else {
        new = insertDend(newDendrogram(right), newDendrogram(left));
    }
    dendArray[left] = new;
    dendArray[right] = NULL;    
    distanceArray[left][right] = -1;
    distanceArray[right][left] = -1;
    return new;      
}


static Dendrogram newDendrogram (int value) {
    Dendrogram new =  malloc(sizeof(DNode));
    new->vertex = value;
    new->right = NULL;
    new->left = NULL;
    return new;
}

static void initArray(int nV,  double distanceArray[nV][nV]) {
    for (int i = 0; i < nV; i++) {
        for (int j = 0; j < nV; j++) {
            distanceArray[i][j] = 0;        
        }
    }

}

static int initDistances(Graph g, int nV, double distanceArray[nV][nV]) {
    int notConnection = 0;
    for (int i = 0; i < nV; i++) {
        for (int j = 0; j < nV; j++) {
            if ((i != j) && (adjacent(g, i, j) || adjacent(g, j, i))) {
                AdjList outList = outIncident(g, i);
                while (outList != NULL) {
                    if (outList->w == j) {
                        distanceArray[i][j] = 1/(outList->weight);
                    }
                    outList = outList->next;
                }
                AdjList inList = inIncident(g, i);
                while (inList != NULL) {
                    if (inList->w == j) {
                    // look for greater distance, which is smaller 2/distance
                        if (((1/(inList->weight)) < distanceArray[i][j]) || (distanceArray[i][j] == 0)) {
                            distanceArray[i][j] = 1/(inList->weight);
                        }
                    }
                    inList = inList->next;
                }
            // set as infinity is j = i or if they're not adjacent    
            } else {
                distanceArray[i][j] = INT_MAX;
                notConnection++;
            }
        }
    }
    //returns number of connections between 2 vertices
    return ((nV * nV) - notConnection)/2;

}

static Dendrogram *initialiseArray (int nV) {

    Dendrogram *dendArray = malloc(sizeof(Dendrogram) * nV); 
    
    int i = 0;
   
    while (i < nV) {
        dendArray[i] = malloc(sizeof(DNode));        
        dendArray[i]->vertex = i;  
        dendArray[i]->right = NULL;
        dendArray[i]->left = NULL;      
        i++;
    }
    return dendArray;
}
