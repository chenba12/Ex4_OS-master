#ifndef GRAPH_
#define GRAPH_

typedef struct GRAPH_NODE_ *pnode;
typedef struct GRAPH_EDGE_ *pedge;

typedef struct GRAPH_EDGE_ {
    int endpoint;
    int weight;
    pedge next;
} Edge, *pedge;

typedef struct GRAPH_NODE_ {
    int data;
    pedge first_edge;
    pedge last_edge;
    pnode next;
} Node, *pnode;

typedef struct Graph_ {
    pnode first_node;
    pnode last_node;
    int amountOfNodes;
    int amountOfEdges;
} Graph, *pgraph;

void build_graph_cmd(char *userInput, int end);

void insert_node_cmd(char *userInput, int end);

void delete_node_cmd(int data);

void printGraph_cmd(); //for self debug

void deleteGraph_cmd();

void shortsPath_cmd(int src, int dest);

void TSP_cmd();

int nextLetter(char *userInput);

#endif
