#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include "graph.h"

pgraph graph = NULL;
int weight = INT_MAX;
int size;

int countN(char *p);

void deleteOutwardEdges(int data);

pnode createNode(int data);

pnode findNode(int data);

void addEdge(pnode src, int dest, int weight);

int countBetween(const char *p, int start);

//TODO dont ignore the final n
void build_graph_cmd(char *userInput, int end) {
    if (graph != NULL) {
        deleteGraph_cmd();
    }
    printf("%s\n", userInput);
    graph = malloc(sizeof(struct Graph_));
    graph->amountOfNodes = 0;
    graph->amountOfEdges = 0;
    int ns = countN(userInput);
    int pointer = 2;
    int counter = 0;
    pnode src = NULL;
    pnode dest = NULL;
    for (int i = 0; i < ns - 1; ++i) {
        int j = countBetween(userInput, pointer);
        for (int k = 0; k < j; k++) {
            if (counter == 0) {
                src = createNode(userInput[pointer] - '0');
            } else if (counter % 2 == 1) {
                dest = createNode(userInput[pointer] - '0');
            } else if (counter % 2 == 0) {
                int weight = userInput[pointer] - '0';
                printf("src-> data %d\n", src->data);
                printf("dest-> data %d\n", dest->data);
                printf("weight = %d\n", weight);
                addEdge(src, dest->data, weight);
            }
            //A 4 n 0 2 5 3 3 n 2 0 4 1 1 n 1 3 7 0 2 n 3 T 3 2 1 3 S 2 0
            pointer++;
            counter++;
        }
        counter = 0;
        pointer++;
    }
    printGraph_cmd();
}

void insert_node_cmd(char *userInput, int end) {
    int i = 0;
    pnode src = NULL;
    pnode dest = NULL;
    int weight;
    while (i != end) {
        pnode node = findNode(userInput[i] - '0');
        if (i == 0) {
            if (node != NULL) {
                deleteOutwardEdges(node->data);
                src = node;
            } else {
                src = createNode(userInput[i] - '0');
                //doesn't exist
            }
        } else if (i % 2 == 1) {
            dest = createNode(userInput[i] - '0');
        } else if (i % 2 == 0) {
            weight = userInput[i] - '0';
            addEdge(src, dest->data, weight);
        }
        i++;
    }
}


int nextLetter(char *userInput) {
    for (int i = 0; i < strlen(userInput); i++) {
        if (userInput[i] == 'B' || userInput[i] == 'D' || userInput[i] == 'T' || userInput[i] == 'S' ||
            userInput[i] == 'A') {
            return i;
        }
    }
    return -1;
}


void printGraph_cmd() {
    pedge edge = NULL;
    pnode node = graph->first_node;
    printf("Number of first_node: %d\n", graph->amountOfNodes);
    printf("Number of first_edge: %d\n", graph->amountOfEdges);
    while (node != NULL) {
        printf("Node: %d\n", node->data);
        printf("  Edges:\n");
        edge = node->first_edge;
        while (edge != NULL) {
            printf("endpoint -> %d (weight = %d)\n", edge->endpoint, edge->weight);
            edge = edge->next;
        }
        node = node->next;
    }
}


void addEdge(pnode src, int dest, int weight) {
    pedge edge = (pedge) malloc(sizeof(Edge));
    edge->endpoint = dest;
    edge->weight = weight;
    edge->next = NULL;
    if (src->last_edge == NULL) {
        src->first_edge = edge;
        src->last_edge = edge;
    } else {
        src->last_edge->next = edge;
        src->last_edge = edge;
    }
    graph->amountOfEdges++;
}

pnode createNode(int data) {
    pnode node = NULL;
    node = findNode(data);
    if (node != NULL) {
        //node exist
        return node;
    }
    //node doesn't exist
    // Allocate memory for the new Node
    node = (pnode) malloc(sizeof(Node));
    node->data = data;
    node->first_edge = NULL;
    node->last_edge = NULL;
    // Add the Node to the array of first_node
    if (graph->last_node == NULL) {
        graph->last_node = node;
        graph->first_node = node;
    } else {
        graph->last_node->next = node;
        graph->last_node = node;
    }
    // Increment the number of first_node in the Graph
    graph->amountOfNodes++;
    return node;
}

int countN(char *p) {
    int counter = 0;
    for (int i = 0; i < strlen(p); ++i) {
        if (p[i] == 'n') counter++;
    }
    return counter;
}

int countBetween(const char *p, int start) {
    int counter = 0;
    int i = start;
    char c = p[i];
    while (c != 'n' && c != 'T' && c != 'B' && c != 'D' && c != 'S') {
        counter++;
        i++;
        c = p[i];
    }
    return counter;
}


pnode findNode(int data) {
    pnode node = graph->first_node;
    while (node != NULL) {
        if (node->data == data) {
            return node;
        }
        node = node->next;
    }
    // No node with the given name was found
    return NULL;
}


void deleteOutwardEdges(int data) {
    pnode node = findNode(data);
    // Free the memory for the first_edge
    pedge edge = node->first_edge;
    while (edge != NULL) {
        pedge temp = edge;
        edge = edge->next;
        graph->amountOfEdges--;
        free(temp);
    }
    // Reset the first_edge field for the node
    node->first_edge = NULL;
    node->last_edge = NULL;
}

void deleteInComingEdges(int data) {
    pnode node = graph->first_node;
    // Iterate through the first_node and first_edge of the graph
    while (node != NULL) {
        pedge edge = node->first_edge;
        pedge prev = NULL;
        while (edge != NULL) {
            if (edge->endpoint == data) {
                // This edge has the given endpoint node as the destination
                pedge temp = edge;
                if (prev == NULL) {
                    // This is the first edge in the linked list
                    node->first_edge = edge->next;
                } else {
                    prev->next = edge->next;
                }
                edge = edge->next;
                free(temp);
                graph->amountOfEdges--;
            } else {
                prev = edge;
                edge = edge->next;
            }
        }
        node = node->next;
    }
}

void deleteEmptyNode(int data) {
    pnode temp = graph->first_node;
    pnode prev;
    // If head node itself holds the key to be deleted
    if (temp != NULL && temp->data == data) {
        temp = temp->next; // Changed head
        free(temp); // free old head
        return;
    }
    // Search for the key to be deleted, keep track of the
    // previous node as we need to change 'prev->next'
    while (temp != NULL && temp->data != data) {
        prev = temp;
        temp = temp->next;
    }
    // If key was not present in linked list
    if (temp == NULL)
        return;
    // Unlink the node from linked list
    prev->next = temp->next;
    free(temp); // Free memory
    // Decrement the number of first_node in the graph
    graph->amountOfNodes--;
}

void delete_node_cmd(int data) {
    if (!findNode(data)) {
        return;
    }
    printf("data is %d\n", data);
    deleteOutwardEdges(data);
    deleteInComingEdges(data);
    deleteEmptyNode(data);
    printf("*************************\n");
    printGraph_cmd();
}

int min(int a, int b) {
    if (a > b) {
        return b;
    } else return a;
}

void shortsPath_cmd(int start, int end) {
    start = 0;
    end = 2;
    int dist[graph->amountOfNodes][graph->amountOfNodes];

    // Initialize distance matrix
    for (int i = 0; i < graph->amountOfNodes; i++) {
        for (int j = 0; j < graph->amountOfNodes; j++) {
            if (i == j) {
                dist[i][j] = 0;
            } else {
                dist[i][j] = INT_MAX;
            }
        }
    }

    // Fill distance matrix with edge weights
    pnode curr = graph->first_node;
    while (curr != NULL) {
        pedge edge = curr->first_edge;
        while (edge != NULL) {
            dist[edge->endpoint][curr->data] = edge->weight;
            edge = edge->next;
        }
        curr = curr->next;
    }

    // Floyd-Warshall algorithm
    for (int k = 0; k < graph->amountOfNodes; k++) {
        for (int i = 0; i < graph->amountOfNodes; i++) {
            for (int j = 0; j < graph->amountOfNodes; j++) {
                if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX) {
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    }
    // Print shortest distance between start and end
    printf("Dijsktra shortest path: %d\n", dist[start][end]);
}


void deleteGraph_cmd() {
    if (graph != NULL) {
        // Free the memory for the first_edge
        pnode node = graph->first_node;
        while (node != NULL) {
            pedge edge = node->first_edge;
            while (edge != NULL) {
                pedge temp = edge;
                edge = edge->next;
                free(temp);
            }
            node = node->next;
        }
        // Free the memory for the first_node
        node = graph->first_node;
        while (node != NULL) {
            pnode temp = node;
            free(temp);
            node = node->next;
        }
        // Reset the fields of the graph struct
        graph->first_node = NULL;
        graph->amountOfNodes = 0;
        graph->amountOfEdges = 0;
        free(graph);
    }
    graph = NULL;
}