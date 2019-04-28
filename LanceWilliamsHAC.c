
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
#include <assert.h>
#include <string.h>
#define numVertices numVerticies
#define TRUE 1
#define FALSE 0

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

typedef struct cluster *Cluster;
typedef struct cluster {
    int value;
    struct cluster *next;
} cluster;

//static void doShowDendrogram (Dendrogram d);
static Dendrogram *initialiseArray (int nV);
static int         initDistances   (Graph g,int nV, double distanceArray[nV][nV]);
static Dendrogram  distanceMin     (int nV, double distanceArray[nV][nV], Dendrogram *dendArray, int *connection, int *numD);
static Dendrogram  distanceMax     (int nV, double distanceArray[nV][nV], Dendrogram *dendArray); 
static Dendrogram  newDendrogram   (int value);
//static Cluster insertToCluster     (Cluster head, int value);
static Dendrogram insertDend       (Dendrogram left, Dendrogram right);
static bool haveCluster            (int index, double distanceArray[]); 
static void initArray(int nV,  double distanceArray[nV][nV]); 
static void putClusterIntoArray (Dendrogram d, Dendrogram *array, int *index);
static void updateArray (Dendrogram *dendArray, Dendrogram d, int position);
static int  whichClust (Dendrogram *dendArray, int value, int arraySize); 
static bool searchIndex (Dendrogram d, int value);
/* 
 * Finds Dendrogram using Lance-Williams algorithm (as discussed in the specs) 
   for the given graph g and the specified method for agglomerative clustering.
   Method value 1 represents 'Single linkage', and 2 represents 'Complete linkage'.
   For this assignment, you only need to implement the above two methods. 

   The function returns 'Dendrogram' structure (binary tree) with the required information.
 * 
 */


Dendrogram LanceWilliamsHAC(Graph g, int method) {

    int nV = numVerticies(g);
    
    
    Dendrogram* dendArray = initialiseArray(nV);
    //return insertDend (newDendrogram (1), newDendrogram (5)); 
    double distanceArray[nV][nV];
    
    initArray(nV, distanceArray);  
    

    int connection = initDistances(g, nV, distanceArray);
    if (connection == 0) {
        
        return NULL;
    }
    /*
    for (int i = 0; i < nV; i++) {
        for (int j = 0; j < nV; j++) {
            if (distanceArray[i][j] == INT_MAX) {
                printf("   INF ");
            } else { 
                printf("%6.2f ", distanceArray[i][j]);
            }
        }
        printf("\n");
    
    }
    */
    
    
    
    int numD = 0;
    Dendrogram dend;
    if (method == 1) {
    //printf("=============\n");
        for (int i = 0; i < (nV - 2); i++) {
            dend = distanceMin(nV, distanceArray, dendArray, &connection, &numD);
        }
        //dend = insertDend (newDendrogram (0), dend);
        
        for (int i = 0; i < nV; i++) {
            if ((searchIndex (dend, i)) == FALSE) {
                dend = insertDend (newDendrogram (i), dend);
                break;
            } 
        }


        
    } else {
        for (int i = 0; i < connection; i++) {
            dend = distanceMax(nV, distanceArray, dendArray);
        }    
    
    }
    /*
    for (int i = 0; i < nV; i++) {
        for (int j = 0; j < nV; j++) {
            if (distanceArray[i][j] == INT_MAX) {
                printf("   INF ");
            } else { 
                printf("%6.2f ", distanceArray[i][j]);
            }
        }
        printf("\n");
    }
    */
    
    
    assert(dend != NULL);
    
    //doShowDendrogram (dend);
    //printf("ending\n");
    return dend;
}


void freeDendrogram(Dendrogram d) {
	if (d == NULL) return;
	freeDendrogram (d->left);
	freeDendrogram (d->right);
	//free (d);

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
//numD = number of dendrograms inside the array
static Dendrogram distanceMin(int nV, double distanceArray[nV][nV], Dendrogram *dendArray, int *connection, int *numD) {
    //int index = *numD;
    
    double length = INT_MAX;
    int left = 0;
    int right = 0;
    for (int i = 0; i < nV; i++) {
        for (int j = 0; j < nV; j++) {
            if ((distanceArray[i][j] != INT_MAX) && (distanceArray[i][j] < length) && (distanceArray[i][j] != -1)) {
                length = distanceArray[i][j];
                left = i;
                right = j;
            } 
        }
    }
    //printf("length is %6.2f\n", length);
    //printf("first time left is %d right is %d\n", left, right);
    //if (clusteredElement
    
    //insertToCluster (linkedClust, left);
    //insertToCluster (linkedClust, right);
    
    int clusterLeft  = haveCluster (nV,  distanceArray[left]);
    int clusterRight = haveCluster (nV, distanceArray[right]);
    //printf("left is %d right is %d\n", left, right);
    //printf("Cluster: left is %d right is %d\n", clusterLeft, clusterRight);
    //printf("numD is %d\n", *numD);
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
        //printf("test hereee\n");      
        int position = whichClust (dendArray, right, *numD);
        new = insertDend(newDendrogram(left), dendArray[position]); 
        updateArray (dendArray, new, position);
    } else if (clusterLeft) {
        //printf("hello\n");
        int position = whichClust (dendArray, left, *numD);
        new = insertDend(newDendrogram(right), dendArray[position]);
        // update dendArray -> put the new dendrogram into the correct index
        updateArray (dendArray, new, position);
    // the first cluster to be make
    } else {
        new = insertDend(newDendrogram(right), newDendrogram(left));
        putClusterIntoArray(new, dendArray, numD);

    }
    //*connection = *connection - 2;
    distanceArray[left][right] = -1;
    distanceArray[right][left] = -1;
    /*
    int temp = -1;
    int tempL = 0;
    for (;temp < nV; temp++) {
        if (dendArray[left][temp] > dendArray[right][temp] && dendArray[left][temp] != INT_MAX && dendArray[right][temp] != INT_MAX) {
            dendArray[left][temp] = INT_MAX;
        } else if (dendArray[left][temp] < dendArray[right][temp] && dendArray[left][temp] != INT_MAX && dendArray[right][temp] != INT_MAX) {
            dendArray[right][temp] = INT_MAX;
        }
    }
    */
    
    return new;   
        
}

static void updateArray (Dendrogram *dendArray, Dendrogram d, int position) {
    //dendArray[position] = realloc(dendArray[position], sizeof(d));
    dendArray[position] = d;
}

static int whichClust (Dendrogram *dendArray, int value, int arraySize) {
    for (int i = 0; i < arraySize; i++) {
        //printf("i is %d\n", i);
        //printf("vertex value is %d\n", dendArray[i]->vertex);
        //printf("i is %d\n", i);
        //printf("arraySiez is %d, value is %d, and i is %d\n", arraySize, value, i);
        
        if (searchIndex (dendArray[i], value)) {
            return i;
        }
    }
    return -1;

}
static bool searchIndex (Dendrogram d, int value) {
    
    int returnVal = FALSE;
    if (d == NULL) return FALSE;
    //printf("value is %d\n", value);
    //printf("vertex is %d\n", d->vertex);
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

    Dendrogram new = insertDend(dendArray[right], dendArray[left]);
    dendArray[left] = new;
    dendArray[right] = new;    
    distanceArray[left][right] = -1;
    distanceArray[right][left] = -1;
    return new;      
}


static void putClusterIntoArray (Dendrogram d, Dendrogram *array, int *size) {
    
    int temp = *size;
    //int newSize = sizeof(DNode) * (temp + 1);
    
    array[temp] = d;
    *size = *size + 1;
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

/*
////////////////////////////////////////////////////////////////////////

// ASCII tree printer
// Courtesy: ponnada
// Via: http://www.openasthra.com/c-tidbits/printing-binary-trees-in-ascii/

// data structures
typedef struct asciinode_struct asciinode;
struct asciinode_struct {
	asciinode *left, *right;
	// length of the edge from this node to its children
	int edge_length;
	int height;
	int lablen;
	//-1=I am left, 0=I am root, 1=I am right
	int parent_dir;
	// max supported unit32 in dec, 10 digits max
	char label[11];
};

// functions
static void print_level (asciinode *node, int x, int level);
static void compute_edge_lengths (asciinode *node);
static void compute_lprofile (asciinode *node, int x, int y);
static void compute_rprofile (asciinode *node, int x, int y);
static asciinode *build_ascii_tree (Dendrogram t);
static void free_ascii_tree (asciinode *node);

#define MAX_HEIGHT 1000
static int lprofile[MAX_HEIGHT];
static int rprofile[MAX_HEIGHT];
//#define INT_MAX (1 << 20)

static int gap = 3; // gap between left and right nodes

// used for printing next node in the same level,
// this is the x coordinate of the next char printed
static int print_next;

// prints ascii tree for given Tree structure
static void doShowDendrogram (Dendrogram t)
{
	asciinode *proot;
	int xmin, i;
	if (t == NULL)
		return;
	proot = build_ascii_tree (t);
	compute_edge_lengths (proot);
	for (i = 0; i < proot->height && i < MAX_HEIGHT; i++)
		lprofile[i] = INT_MAX;
	compute_lprofile (proot, 0, 0);
	xmin = 0;
	for (i = 0; i < proot->height && i < MAX_HEIGHT; i++)
		xmin = MIN (xmin, lprofile[i]);
	for (i = 0; i < proot->height; i++) {
		print_next = 0;
		print_level (proot, -xmin, i);
		printf ("\n");
	}
	if (proot->height >= MAX_HEIGHT) {
		printf (
			"(Tree is taller than %d; may be drawn incorrectly.)\n",
			MAX_HEIGHT);
	}
	free_ascii_tree (proot);
}

// This function prints the given level of the given tree, assuming
// that the node has the given x cordinate.
static void print_level (asciinode *node, int x, int level)
{
	int i, isleft;
	if (node == NULL)
		return;
	isleft = (node->parent_dir == -1);
	if (level == 0) {
		for (i = 0;
			 i < (x - print_next - ((node->lablen - isleft) / 2));
			 i++)
			printf (" ");
		print_next += i;
		printf ("%s", node->label);
		print_next += node->lablen;
	} else if (node->edge_length >= level) {
		if (node->left != NULL) {
			for (i = 0; i < (x - print_next - (level)); i++) {
				printf (" ");
			}
			print_next += i;
			printf ("/");
			print_next++;
		}
		if (node->right != NULL) {
			for (i = 0; i < (x - print_next + (level)); i++) {
				printf (" ");
			}
			print_next += i;
			printf ("\\");
			print_next++;
		}
	} else {
		print_level (
			node->left,
			x - node->edge_length - 1,
			level - node->edge_length - 1);
		print_level (
			node->right,
			x + node->edge_length + 1,
			level - node->edge_length - 1);
	}
}

// This function fills in the edge_length and
// height fields of the specified tree
static void compute_edge_lengths (asciinode *node)
{
	int h, hmin, i, delta;
	if (node == NULL)
		return;
	compute_edge_lengths (node->left);
	compute_edge_lengths (node->right);

	// first fill in the edge_length of node 
	if (node->right == NULL && node->left == NULL)
		node->edge_length = 0;
	else {
		if (node->left == NULL)
			hmin = 0;
		else {
			for (i = 0; i < node->left->height && i < MAX_HEIGHT; i++)
				rprofile[i] = -INT_MAX;
			compute_rprofile (node->left, 0, 0);
			hmin = node->left->height;
		}
		if (node->right == NULL)
			hmin = 0;
		else {
			for (i = 0; i < node->right->height && i < MAX_HEIGHT; i++)
				lprofile[i] = INT_MAX;
			compute_lprofile (node->right, 0, 0);
			hmin = MIN (node->right->height, hmin);
		}
		delta = 4;
		for (i = 0; i < hmin; i++) {
			int w = gap + 1 + rprofile[i] - lprofile[i];
			delta = (delta > w) ? delta : w;
		}

		// If the node has two children of height 1, then we allow the
		// two leaves to be within 1, instead of 2
		if (((node->left != NULL && node->left->height == 1) ||
			 (node->right != NULL && node->right->height == 1)) &&
			delta > 4)
			delta--;
		node->edge_length = ((delta + 1) / 2) - 1;
	}

	// now fill in the height of node
	h = 1;
	if (node->left != NULL)
		h = MAX (node->left->height + node->edge_length + 1, h);
	if (node->right != NULL)
		h = MAX (node->right->height + node->edge_length + 1, h);
	node->height = h;
}

static asciinode *build_ascii_tree_recursive (Dendrogram t)
{
	asciinode *node;

	if (t == NULL)
		return NULL;
	node = malloc (sizeof (asciinode));
	node->left = build_ascii_tree_recursive (t->left);
	node->right = build_ascii_tree_recursive (t->right);
	if (node->left != NULL)
		node->left->parent_dir = -1;
	if (node->right != NULL)
		node->right->parent_dir = 1;
	sprintf (node->label, "%d", t->vertex);
	node->lablen = (int) strlen (node->label);

	return node;
}

// Copy the tree into the ascii node structre
static asciinode *build_ascii_tree (Dendrogram t)
{
	asciinode *node;
	if (t == NULL)
		return NULL;
	node = build_ascii_tree_recursive (t);
	node->parent_dir = 0;
	return node;
}

// Free all the nodes of the given tree
static void free_ascii_tree (asciinode *node)
{
	if (node == NULL)
		return;
	free_ascii_tree (node->left);
	free_ascii_tree (node->right);
	free (node);
}

// The following function fills in the lprofile array for the given
// tree. It assumes that the center of the label of the root of this tree
// is located at a position (x,y).  It assumes that the edge_length
// fields have been computed for this tree.
static void compute_lprofile (asciinode *node, int x, int y)
{
	int i, isleft;
	if (node == NULL)
		return;
	isleft = (node->parent_dir == -1);
	lprofile[y] = MIN (lprofile[y], x - ((node->lablen - isleft) / 2));
	if (node->left != NULL) {
		for (i = 1; i <= node->edge_length && y + i < MAX_HEIGHT; i++)
			lprofile[y + i] = MIN (lprofile[y + i], x - i);
	}
	compute_lprofile (
		node->left,
		x - node->edge_length - 1,
		y + node->edge_length + 1);
	compute_lprofile (
		node->right,
		x + node->edge_length + 1,
		y + node->edge_length + 1);
}

static void compute_rprofile (asciinode *node, int x, int y)
{
	int i, notleft;
	if (node == NULL)
		return;
	notleft = (node->parent_dir != -1);
	rprofile[y] = MAX (rprofile[y], x + ((node->lablen - notleft) / 2));
	if (node->right != NULL) {
		for (i = 1; i <= node->edge_length && y + i < MAX_HEIGHT; i++)
			rprofile[y + i] = MAX (rprofile[y + i], x + i);
	}
	compute_rprofile (
		node->left,
		x - node->edge_length - 1,
		y + node->edge_length + 1);
	compute_rprofile (
		node->right,
		x + node->edge_length + 1,
		y + node->edge_length + 1);
}
*/
