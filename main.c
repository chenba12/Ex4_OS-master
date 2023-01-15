#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "graph.h"

int main() {
    int size = 0;
    char *userInput = calloc(size, sizeof(char));
    char c;
    char *num = (char *) calloc(10, sizeof(char));
    int number = 0;
    int index;
    while ((c = fgetc(stdin)) != EOF) {
        userInput = (char *) realloc(userInput, (size + 10) * sizeof(char));
        if (userInput == NULL) {
            printf("?!");
        }
        userInput[size++] = c;
    }
    userInput = (char *) realloc(userInput, (size + 1) * sizeof(char));
    userInput[size++] = '\0';
    for (int i = 0; i < size; i++) {
        if (userInput[i] == 'A') {
            i += 2;
            int end = nextLetter(userInput + i);
            build_graph_cmd(userInput + i, end);
        } else if (userInput[i] == 'B') {
            int end = nextLetter(userInput + i + 1);
            insert_node_cmd(userInput + i + 2, end);
        } else if (userInput[i] == 'D') {
            i += 2;
            index = getNum(userInput, i, num);
            sscanf(num, "%d", &number);
            delete_node_cmd(number);
            i += index;
        } else if (userInput[i] == 'S') {
            i += 2;
            index = getNum(userInput, i, num);
            sscanf(num, "%d", &number);
            int firstNode = number;
            i += index + 1;
            index = getNum(userInput, i, num);
            sscanf(num, "%d", &number);
            int secondNode = number;
            int path = shortsPath_cmd(firstNode, secondNode);
            printf("Dijsktra shortest path: %d \n", path);
            i += index;
        } else if (userInput[i] == 'T') {
//            printf("in T\n");
            i += 2;
            int k = userInput[i] - '0';
//            printf("k is %d\n", k);
            i += 2;
            int s = 0;
            int *nodes = (int *) malloc(k * sizeof(int));
            while (s < k) {

                if (userInput[i] == ' ') {
                    i++;
                    continue;
                } else {
                    index = getNum(userInput, i, num);
                    sscanf(num, "%d", &number);
                    nodes[s] = number;
//                    printf("%d\n", number);
                    s++;
                    i += index;
                }
            }
            TSP_cmd(nodes, k);
            free(nodes);
        }
    }
    free(num);
    free(userInput);
    deleteGraph_cmd();
    return 0;
}

