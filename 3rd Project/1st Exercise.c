//
//  main.c
//
//  Created by Konstantinos Mitropoulos on 8/1/2017.
//  Copyright © 2016 NTUA. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <time.h>

//~~~~~~~~~ GRAPH STRUCTS ~~~~~~~~~//
// A structure to represent a node in adjacency list
struct List_Node {
    int neighbour;
    int weight;
    struct List_Node* next;
};
typedef struct List_Node List_Node;

// A structure to represent an adjacency list
struct List {
    struct List_Node *head;         // pointer to head node
};
typedef struct List List;

// A structure to represent a graph. A graph is an array of adjacency lists.
// Size of array will be V (number of vertices in graph)
struct Graph {
    int n;
    struct List* array;
};
typedef struct Graph Graph;



//~~~~~~~~~ BASIC GRAPH FUNCTIONS ~~~~~~~~~//
// A utility function to create a new adjacency list node
List_Node* NewAdjListNode(int neighbour, int weight) {
    List_Node* newNode =
    (List_Node*) malloc(sizeof(struct List_Node));
    newNode->neighbour = neighbour;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}

// A utility function that creates a graph of V vertices
Graph* CreateGraph(int n) {
    Graph* graph = (Graph*) malloc(sizeof(Graph));
    graph->n = n;
    
    // Create an array of adjacency lists
    graph->array = (List*) malloc(n * sizeof(List));
    
    // Initialize each adjacency list as empty
    for (int i = 0; i < n; ++i)
        graph->array[i].head = NULL;
    
    return graph;
}

//Adds an edge to an undirected graph
void AddEdgeToGraph(Graph* graph, int current, int neighbour, int weight) {

    List_Node* newNode = NewAdjListNode(neighbour, weight);             //adds neighbour to current
    newNode->next = graph->array[current].head;
    graph->array[current].head = newNode;
    
    newNode = NewAdjListNode(current, weight);                          //adds current (as a neighbour) to the neighbour
    newNode->next = graph->array[neighbour].head;
    graph->array[neighbour].head = newNode;
}


void Input (FILE *fd, int *n, int *k, int *m, int tel[10001][2], int edges[500000][3])    {
    fscanf(fd, "%d%d%d", n, k, m);
    for (int i = 0; i < *k; i++) {
        fscanf(fd, "%d%d", &tel[i][0], &tel[i][1]);
    }
    
    for (int i = 0; i < *m; i++) {
        fscanf(fd, "%d%d%d", &edges[i][0], &edges[i][1], &edges[i][2]);
    }
    return;
}


void CreateFirstGraph (Graph *graph, int k, int m, int tel[10000][2], int edges[500000][3])  {
    for (int i = 0; i < k; i++) {
        AddEdgeToGraph(graph, 0, tel[i][0], tel[i][1]);
    }
    for (int i = 0; i < m; i++) {
        AddEdgeToGraph(graph, edges[i][0], edges[i][1], edges[i][2]);
    }
    return;
}


void CreateSecondGraph (Graph *graph, int m, int edges[500000][3])  {
    for (int i = 0; i < m; i++) {
        AddEdgeToGraph(graph, edges[i][0]-1, edges[i][1]-1, edges[i][2]);
    }
    return;
}



//~~~~~~~~~ HEAP STRUCTS ~~~~~~~~~//
// Structure to represent a min heap node
struct MinHeapNode {
    int  n;
    int key;
};
typedef struct MinHeapNode MinHeapNode;

// Structure to represent a min heap
struct MinHeap {
    int size;
    int capacity;
    int *pos;
    MinHeapNode **array;
};
typedef struct MinHeap MinHeap;



//~~~~~~~~~ BASIC GRAPH FUNCTIONS ~~~~~~~~~//
// A utility function to create a new Min Heap Node
MinHeapNode* newMinHeapNode(int n, int key) {
    MinHeapNode* minHeapNode = (MinHeapNode*) malloc(sizeof(MinHeapNode));
    minHeapNode->n = n;
    minHeapNode->key = key;
    return minHeapNode;
}
typedef struct MinHeapNode MinHeapNode;

// A utility function to create a Min Heap
MinHeap* createMinHeap(int capacity) {
    MinHeap* minHeap =
    (MinHeap*) malloc(sizeof(MinHeap));
    minHeap->pos = (int *)malloc(capacity * sizeof(int));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array =
    (MinHeapNode**) malloc(capacity * sizeof(MinHeapNode*));
    return minHeap;
}

// A utility function to swap two nodes of Min Heap. Needed for min heapify
void swapMinHeapNode(MinHeapNode** a, MinHeapNode** b) {
    MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

// A standard function to heapify at given idx
// This function also updates position of nodes when they are swapped
// Position is needed for decreaseey() ---INSERT----
void minHeapify(MinHeap* minHeap, int idx) {
    int smallest, left, right;
    smallest = idx;
    left = 2 * idx + 1;
    right = 2 * idx + 2;
    
    if (left < minHeap->size &&
        minHeap->array[left]->key < minHeap->array[smallest]->key )
        smallest = left;
    
    if (right < minHeap->size &&
        minHeap->array[right]->key < minHeap->array[smallest]->key )
        smallest = right;
    
    if (smallest != idx)
    {
        // The nodes to be swapped in min heap
        MinHeapNode *smallestNode = minHeap->array[smallest];
        MinHeapNode *idxNode = minHeap->array[idx];
        
        // Swap positions
        minHeap->pos[smallestNode->n] = idx;
        minHeap->pos[idxNode->n] = smallest;
        
        // Swap nodes
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);
        
        minHeapify(minHeap, smallest);
    }
}

// A utility function to check if the given minHeap is empty
int isEmpty(MinHeap* minHeap) {
    return minHeap->size == 0;
}

// Standard function to extract minimum node from heap
MinHeapNode* extractMin(MinHeap* minHeap) {
    if (isEmpty(minHeap))
        return NULL;
    
    // Store the root node
    MinHeapNode* root = minHeap->array[0];
    
    // Replace root node with last node
    MinHeapNode* lastNode = minHeap->array[minHeap->size - 1];
    minHeap->array[0] = lastNode;
    
    // Update position of last node
    minHeap->pos[root->n] = minHeap->size-1;
    minHeap->pos[lastNode->n] = 0;
    
    // Reduce heap size and heapify root
    --minHeap->size;
    minHeapify(minHeap, 0);
    
    return root;
}

// Function to decrease key value of a given vertex n. This func.
// uses pos[] of min heap to get the current index of node in min heap
void decreaseKey(MinHeap* minHeap, int n, int key) {
    int i = minHeap->pos[n];
    
    minHeap->array[i]->key = key;
    
    while (i && minHeap->array[i]->key < minHeap->array[(i - 1)/2]->key) {
        minHeap->pos[minHeap->array[i]->n] = (i-1)/2;
        minHeap->pos[minHeap->array[(i-1)/2]->n] = i;
        swapMinHeapNode(&minHeap->array[i],  &minHeap->array[(i - 1) / 2]);
        
        i = (i - 1) / 2;
    }
}

// A utility function to check if a given vertex
// 'v' is in min heap or not
bool isInMinHeap(MinHeap *minHeap, int n) {
    if (minHeap->pos[n] < minHeap->size)
        return true;
    return false;
}

// MAIN program that constructs MST using Prim's algorithm
int Prim_Algorithm(Graph *graph) {
    
    int N = graph->n;           // Get the number of vertices in graph
    int parent[N];              // Array to store constructed MST
    int key[N];                 // Key values used to pick minimum weight edge in cut
    int TotalWeight = 0;        // Total needed weight (RESULT returned)

    MinHeap* minHeap = createMinHeap(N);

    //Initialization of min heap with all vertices.
    for (int v = 1; v < N; ++v) {
        parent[v] = -1;
        key[v] = INT_MAX;
        minHeap->array[v] = newMinHeapNode(v, key[v]);
        minHeap->pos[v] = v;
    }
    
    // Make key value of 0th vertex as 0 so that is extracted first
    key[0] = 0;
    minHeap->array[0] = newMinHeapNode(0, key[0]);
    minHeap->pos[0] = 0;
    
    minHeap->size = N;
    
    // In the following loop, min heap contains all nodes not yet added to MST
    while (!isEmpty(minHeap)) {
        
        // Extract the vertex with minimum key value
        MinHeapNode* minHeapNode = extractMin(minHeap);
        int u = minHeapNode->n;
        
        // Traverse through all adjacent vertices of u and update their key values
        List_Node* pCrawl = graph->array[u].head;
        while (pCrawl != NULL) {
            int v = pCrawl->neighbour;
            
            
            // If v is not yet included in MST and weight of u-v is
            // less than key value of v, then update key value and
            // parent of v
            if (isInMinHeap(minHeap, v) && pCrawl->weight < key[v])
            {
                if (key[v] < 200000) TotalWeight -= key[v];
                TotalWeight += pCrawl->weight;
                key[v] = pCrawl->weight;
                parent[v] = u;
                decreaseKey(minHeap, v, key[v]);
            }
            pCrawl = pCrawl->next;
        }
    }

    return TotalWeight;
}

//~~~~~~~~~ MAIN FUNCTION ~~~~~~~~~//

int main()
{
    clock_t begin = clock();
    FILE *fd;
    fd = fopen("/Users/konstantinos/Documents/InProgress/lab03/spacemst/input1.txt", "r");
    
    int N, K, M;
    int Teleporters[10001][2];
    int Edges[500000][3];
    int res = -1, res2 = -1;

    Input(fd, &N, &K, &M, Teleporters, Edges);              //GET INPUT FROM STDIN
    
    struct Graph* graph = CreateGraph(N+1);                 //CREATE FIRST GRAPH
    CreateFirstGraph(graph, K, M, Teleporters, Edges);      //ADD ALL EDGES TO FIRST GRAPH (WITH TELEPORTERS)
    res = Prim_Algorithm(graph);                            //EXECUTE PRIM'S ALGORITHM
    printf("%d\n", graph->array[1].head->next->neighbour);


    struct Graph* graph2 = CreateGraph(N);                  //CREATE SECOND ALGORITHM
    CreateSecondGraph(graph2, M, Edges);                    //ADD ALL EDGES TO SECOND GRAPH (WITHOUT TELEPORTERS)
    res2 = Prim_Algorithm(graph2);                          //EXECUTE PRIM'S ALGORITHM
    
    if (res2 < res) res = res2;
    
    printf("%d\n", res);
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    fprintf(stdout, "Execution Time: %f\n", time_spent);
    return 0;
}
