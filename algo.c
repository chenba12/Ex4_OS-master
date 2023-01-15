#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include "graph.h"

pgraph graph = NULL;

int countN(char *p);

void deleteOutwardEdges(int data);

pnode createNode(int data);

pnode findNode(int data);

void addEdge(pnode src, pnode dest, int weight);

int countBetween(const char *p, int start);

int getNum(char *userInput, int start, char *num) {
    int size = 0;
    int maxSize = 9;
    for (int i = start; i < strlen(userInput); ++i) {
//        printf("userinout %c\n", userInput[start]);
        if (userInput[i] == ' ' || userInput[i] == '\n' || userInput[i] == 'B' || userInput[i] == 'D' ||
            userInput[i] == 'T' || userInput[i] == 'S' ||
            userInput[i] == 'A') {
            if (size == maxSize) {
                num = (char *) realloc(num, size + 1 * sizeof(char));
                maxSize++;
            }
            num[size] = '\0';
            return size;
        } else {
            if (size == maxSize) {
                num = (char *) realloc(num, size + 1 * sizeof(char));
                maxSize++;
            }
            num[size] = userInput[i];
            size++;
        }
    }
    return -1;
}

void build_graph_cmd(char *userInput, int end) {
    if (graph != NULL) {
        deleteGraph_cmd();
    }
    graph = malloc(sizeof(struct Graph_));
    graph->amountOfNodes = 0;
    graph->amountOfEdges = 0;
    graph->first_node = NULL;
    graph->last_node = NULL;
    int ns = countN(userInput);
    int pointer = 4;
    int counter = 0;
    char *num = (char *) calloc(10, sizeof(char));
    int number = 0;
    pnode src = NULL;
    pnode dest = NULL;
    for (int i = 0; i < ns - 1; ++i) {
        int j = countBetween(userInput, pointer);
        for (int k = 0; k < j; k++) {
            if (counter == 0) {
                int index = getNum(userInput, pointer, num);
                sscanf(num, "%d", &number);
                src = createNode(number);
                number = 0;
                k += index;
                pointer += index;
            } else if (counter % 2 == 1) {
                int index = getNum(userInput, pointer, num);
                sscanf(num, "%d", &number);
                dest = createNode(number);
                number = 0;
                k += index;
                pointer += index;


            } else if (counter % 2 == 0) {
                int index = getNum(userInput, pointer, num);
                sscanf(num, "%d", &number);
                k += index;
                pointer += index;
                addEdge(src, dest, number);
                number = 0;
            }
            pointer++;
            if (userInput[pointer] == 'B' || userInput[pointer] == 'D' || userInput[pointer] == 'T' || userInput[pointer] == 'S' ||
                userInput[pointer] == 'A') {
                break;
            }
            if (userInput[pointer] == ' ') pointer++;
            counter++;
        }
        if (userInput[pointer] == 'B' || userInput[pointer] == 'D' || userInput[pointer] == 'T' || userInput[pointer] == 'S' ||
            userInput[pointer] == 'A') {
            break;
        }
        counter = 0;
        pointer++;

        if (userInput[pointer] == ' ') pointer++;
    }
    free(num);
}

void insert_node_cmd(char *userInput, int end) {
    int i = 0;
    pnode src = NULL;
    pnode dest = NULL;
    char *num = (char *) calloc(10, sizeof(char));
    int number = 0;
    int index = getNum(userInput, i, num);
    sscanf(num, "%d", &number);
    pnode node = findNode(number);
    int pointer = i + index;
    i = 1;
    if (node != NULL) {
        deleteOutwardEdges(number);
        src = node;
    } else {
        src = createNode(number);
    }
    number = 0;
    if (userInput[pointer] == ' ') {
        pointer++;
    }
    while (pointer < end - 1) {
        if (i % 2 == 1) {
            index = getNum(userInput, pointer, num);
            sscanf(num, "%d", &number);
            dest = createNode(number);
            pointer += index;
            number = 0;
        } else if (i % 2 == 0) {
            index = getNum(userInput, pointer, num);
            sscanf(num, "%d", &number);
            addEdge(src, dest, number);
            pointer += index;
        }
        i++;
        pointer++;
        if (userInput[pointer] == ' ') {
            pointer++;
        }
    }
    free(num);
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

//debug
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
            printf("endpoint -> %d (weight = %d)\n", edge->endpoint->data, edge->weight);
            edge = edge->next;
        }
        node = node->next;
    }
}


void addEdge(pnode src, pnode dest, int weight) {
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
        return node;
    }
    node = (pnode) malloc(sizeof(Node));
    node->data = data;
    node->first_edge = NULL;
    node->last_edge = NULL;
    node->next = NULL;
    if (graph->last_node == NULL) {
        graph->last_node = node;
        graph->first_node = node;
    } else {
        graph->last_node->next = node;
        graph->last_node = node;
    }
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
    return NULL;
}


void deleteOutwardEdges(int data) {
    pnode node = findNode(data);
    pedge edge = node->first_edge;
    while (edge != NULL) {
        pedge temp = edge;
        edge = edge->next;
        graph->amountOfEdges--;
        free(temp);
    }
    node->first_edge = NULL;
    node->last_edge = NULL;
}

void deleteInComingEdges(int data) {
    pnode node = graph->first_node;
    while (node != NULL) {
        pedge edge = node->first_edge;
        pedge prev = NULL;
        while (edge != NULL) {
            if (edge->endpoint->data == data) {
                pedge temp = edge;
                if (prev == NULL) {
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

int shortsPath_cmd(int src, int dest) {
    pnode pointer = graph->first_node;
    while (pointer != NULL) {
        if (pointer->data == src) {
            pointer->distance = 0;
        } else pointer->distance = INT_MAX / 2;
        pointer = pointer->next;
    }
    for (int i = 0; i < graph->amountOfNodes - 1; i++) {
        pnode pointer2 = graph->first_node;
        for (int j = 0; j < graph->amountOfNodes; j++) {
            for (pedge edge = pointer2->first_edge; edge != NULL; edge = edge->next) {
                pnode neighbor = edge->endpoint;
                int new_distance = pointer2->distance + edge->weight;
                if (neighbor->distance >= new_distance) {
                    neighbor->distance = new_distance;
                }
            }
            pointer2 = pointer2->next;
        }
    }
    pnode node = findNode(dest);
    if (node->distance == INT_MAX) {
        return -1;
    } else return node->distance;
}

void permute(int *arr, int start, int end, int **result, int *count) {
    if (start == end) {
        for (int i = 0; i <= end; i++)
            result[*count][i] = arr[i];
        (*count)++;
    } else {
        for (int i = start; i <= end; i++) {
            int temp = arr[start];
            arr[start] = arr[i];
            arr[i] = temp;
            permute(arr, start + 1, end, result, count);
            temp = arr[start];
            arr[start] = arr[i];
            arr[i] = temp;
        }
    }
}

int find(int **distArr, int num, int k) {

    for (int i = 0; i < k + 1; ++i) {
        if (distArr[0][i] == num) {
            return i;
        }
    }
    return -1;
}

int factorial(int n) {
    if (n == 0)
        return 1;
    else
        return n * factorial(n - 1);
}

void TSP_cmd(int nodes[], int k) {
    int **distArr = (int **) calloc((k + 1), sizeof(int *));
    for (int i = 0; i < k + 1; i++) {
        distArr[i] = (int *) calloc((k + 1), sizeof(int));
    }
    for (int i = 1; i < k + 1; ++i) {
        distArr[0][i] = nodes[i - 1];
        distArr[i][0] = nodes[i - 1];
    }

    for (int i = 1; i < k + 1; i++) {
        shortsPath_cmd(distArr[0][i], distArr[0][i]);
        for (int j = 1; j < k + 1; j++) {
            if (i == j) {
                distArr[i][j] = 0;
            } else distArr[i][j] = findNode(nodes[j - 1])->distance;
        }
    }

    int count = 0;
    int factRes = factorial(k);
    int **result = (int **) malloc(sizeof(int *) * factRes);
    for (int i = 0; i < factRes; i++) {
        result[i] = (int *) malloc(sizeof(int) * k);
    }
    permute(nodes, 0, k - 1, result, &count);
    bool isPath = false;
    int cur_path_length;
    int min_path_length = INT_MAX;
    int i = 0;
    while (i < factRes) {
        cur_path_length = 0;
        for (int j = 0; j < k - 1; ++j) {
            int index1 = find(distArr, result[i][j], k);
            int index2 = find(distArr, result[i][j + 1], k);
            cur_path_length += distArr[index1][index2];
            if (cur_path_length >= INT_MAX / 2 || cur_path_length <= 0) {
                break;
            }
        }
        if (cur_path_length < INT_MAX / 2 && cur_path_length > 0) {
            isPath = true;
            if (cur_path_length < min_path_length) {
                min_path_length = cur_path_length;
            }
        }
        i++;
    }
    if (isPath) {
        printf("TSP shortest path: %d \n", min_path_length);
    } else {
        printf("TSP shortest path: %d \n", -1);
    }
    i = 0;
    for (i = 0; i < factRes; i++) {
        free(result[i]);
    }
    i = 0;
    for (i = 0; i < k + 1; i++) {
        free(distArr[i]);
    }
    free(distArr);
    free(result);
}

void deleteEmptyNode(int data) {
    pnode temp = graph->first_node;
    pnode prev;
    if (temp != NULL && temp->data == data) {
        temp = temp->next;
        free(temp);
        return;
    }
    while (temp != NULL && temp->data != data) {
        prev = temp;
        temp = temp->next;
    }
    if (temp == NULL)
        return;
    prev->next = temp->next;
    free(temp); // Free memory
    graph->amountOfNodes--;
}

void delete_node_cmd(int data) {
    if (!findNode(data)) {
        return;
    }
    deleteOutwardEdges(data);
    deleteInComingEdges(data);
    deleteEmptyNode(data);
}

void deleteGraph_cmd() {
    if (graph != NULL) {
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
        node = graph->first_node;
        while (node != NULL) {
            pnode temp = node;
            node = node->next;
            free(temp);
        }
        graph->first_node = NULL;
        graph->last_node = NULL;
        graph->amountOfNodes = 0;
        graph->amountOfEdges = 0;
        free(graph);
    }
    graph = NULL;
}