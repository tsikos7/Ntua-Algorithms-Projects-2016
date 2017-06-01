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
struct AdjList {
    struct List_Node *head;         // pointer to head node
};
typedef struct AdjList AdjList;

// A structure to represent a graph. A graph is an array of adjacency lists.
// Size of array will be V (number of vertices in graph)
struct Graph {
    int n;
    struct AdjList* array;
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
    graph->array = (AdjList*) malloc(n * sizeof(AdjList));
    
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

void printArr(Graph *mst, int arr[], int weig[], int n)
{
    for (int i = 1; i < n; ++i)
        AddEdgeToGraph(mst, arr[i], i, weig[i]);
    return;
}


// MAIN program that constructs MST using Prim's algorithm
int Prim_Algorithm(Graph *graph, Graph *mst) {
    
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
    printArr(mst, parent, key, N);
    return TotalWeight;
}




void Input (FILE *fd, int *n, int *q, int *m, int edges[500000][3], int path[500000][2])    {
    fscanf(fd, "%d%d", n, m);
    
    for (int i = 0; i < *m; i++) {
        fscanf(fd, "%d%d%d", &edges[i][0], &edges[i][1], &edges[i][2]);
    }
    
    fscanf(fd, "%d", q);
    
    for (int i = 0; i < *q; i++) {
        int f, g;
        fscanf(fd, "%d%d", &f, &g);
        path[i][0] = f-1;
        path[i][1] = g-1;
    }
    
    return;
}


void Create_Graph (Graph *graph, int m, int edges[500000][3])  {
    for (int i = 0; i < m; i++) {
        AddEdgeToGraph(graph, edges[i][0]-1, edges[i][1]-1, edges[i][2]);
    }
    return;
}


struct List {
    int number;
    int maxWeight;
    struct List *next;         // pointer to head node
};
typedef struct List List;

int Space[3000][3000];

int BFS (Graph *graph, int n, int src)  {
    
    //fprintf(stdout, "Path from %d to %d is being checked...\n\n", src+1, dest+1);
    
    bool visited[3000];
    for(int i = 0; i < n; i++)
        visited[i] = false;
    
    List *queue_head  = (List*) malloc(sizeof(List));
    List *queue_end = queue_head;
    
    visited[src] = true;
    queue_head->number = src;
    
    queue_head->next = NULL;
    int s = src;
    int Weight[3000] = {0};
    int i = 0, k = 0;
    while (queue_head != NULL)    {
        
        /*if (queue_head->number == dest) {
            //fprintf(stdout, "Destination %d Found!!\n\n", dest+1);
            fprintf(stdout, "%d\n", queue_head->maxWeight);
            //fprintf(stdout, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
            return 1;
        }*/
        s = queue_head->number;
        
        //fprintf(stdout, "Checking current vertex's %d neighbours...\n", s+1);
        queue_head = queue_head->next;
        List_Node *head = graph->array[s].head;
        
        while (head != NULL)   {
            
            if(!visited[head->neighbour])
            {
                //fprintf(stdout, "%d is one\n", head->neighbour+1);
                visited[head->neighbour] = true;
                
                List *new = (List*) malloc(sizeof(List));
                new->number = head->neighbour;
                new->next = NULL;
                if (queue_head == NULL) {
                    if (Weight[k] > head->weight) new->maxWeight = Weight[k];
                    else new->maxWeight = head->weight;
                    queue_head = new;
                    queue_end = new;
                    
                    //fprintf(stdout, "CHECK THIS: %d\n", new->maxWeight);
                    
                }
                else {
                    if (Weight[k] > head->weight) new->maxWeight = Weight[k];
                    else new->maxWeight = head->weight;
                    queue_end->next = new;
                    queue_end = queue_end->next;
                

                    
                    //fprintf(stdout, "CHECK THIS: %d\n", new->maxWeight);
                }
                Weight[i+1] = queue_end->maxWeight;
                Space[src][head->neighbour] = queue_end->maxWeight;
                
                i++;
            }
            
            head = head->next;
            
        }
        k++;
        
        //fprintf(stdout, "OK!\n");
    }
    
    return -1;
}



void BFS_main (Graph *graph, int n, int q, int path[50000][2])  {
    int src = -1, dest = -1;
    for (int i = 0; i < n; i++) {
        BFS(graph, n, i);
    }
    
    for (int i = 0; i < q; i++) {
        src = path[i][0];
        dest = path[i][1];
        fprintf(stdout, "%d\n", Space[src][dest]);
    }
    
    
    return;
}

//~~~~~~~~~ MAIN FUNCTION ~~~~~~~~~//

int main()
{
    clock_t begin = clock();
    FILE *fd;
    fd = fopen("/Users/konstantinos/Documents/InProgress/lab03/roadtrip/input6.txt", "r");
    
    int N, M, Q;
    int Edges[100000][3];
    int Path[100000][2];
    int res = -1;
    
    
    Input(fd, &N, &Q, &M, Edges, Path);              //GET INPUT FROM STDIN
 
    struct Graph* graph = CreateGraph(N);                  //CREATE SECOND ALGORITHM
    Create_Graph(graph, M, Edges);                    //ADD ALL EDGES TO SECOND GRAPH (WITHOUT TELEPORTERS)
    struct Graph* MST = CreateGraph(N);
    res = Prim_Algorithm(graph, MST);                          //EXECUTE PRIM'S ALGORITHM
    
    BFS_main(MST, N, Q, Path);
    
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    fprintf(stdout, "Execution Time: %f\n", time_spent);
    return 0;
}
