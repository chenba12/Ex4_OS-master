#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "graph.h"

int main() {
    char *userInput = NULL;
    int size = 0;
    int c;
    while ((c = fgetc(stdin)) != EOF) {
        if ((char) c != ' ' && (char) c != '\t' && (char) c != '\n') {
            userInput = (char *) realloc(userInput, (size + 1) * sizeof(char));
            userInput[size++] = (char) c;
        }
    }
    printf("%s\n", userInput);
    for (size_t i = 0; i < strlen(userInput); i++) {
        if (userInput[i] == 'A') {
            int end = nextLetter(userInput + i + 1);
            build_graph_cmd(userInput + i + 1, end);
        } else if (userInput[i] == 'B') {
            int end = nextLetter(userInput + i + 1);
            insert_node_cmd(userInput + i + 1, end);
        } else if (userInput[i] == 'D') {
            delete_node_cmd(userInput[i + 1] - '0');
        } else if (userInput[i] == 'S') {
            shortsPath_cmd(0, 2);
        } else if (userInput[i] == 'T') {
            int nodes[3] = {2, 1, 3};
            TSP_cmd();
        }
    }

//    graphToMatrix();
    free(userInput);
    deleteGraph_cmd();
    return 0;
}
