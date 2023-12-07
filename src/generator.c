/**
 * @file supervisor.c
 * @author Kristijan Todorovic <e11806442@student.tuwien.ac.at>
 * @date 28.11.2023
 */

#include <stdio.h>
#include <stdlib.h>
#include "graph_util.h"

char *program_name;

void error_exit(char *message) {
    fprintf(stderr, "%s: \n", message);
    exit(EXIT_FAILURE);
}

void usage(void) {
    fprintf(stderr, "usage: %s EDGE1 EDGE2 ...\n", program_name);
    exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]) {

    program_name = argv[0];

    graph g = graph_constr();

    for (int i = 1; i < argc; i++) {
        int u, v;
        if (sscanf(argv[i], "%d-%d", &u, &v) != 2) {
            usage();
        }
        edge e = edge_constr(u, v);
        graph_add(&g, &e);
    }


    return EXIT_SUCCESS;
}