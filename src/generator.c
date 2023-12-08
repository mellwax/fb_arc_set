/**
 * @file supervisor.c
 * @author Kristijan Todorovic, [e11806442(at)student.tuwien.ac.at]
 * @date 28.11.2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "graph_util.h"

/** the program name */
const char *program_name;

/**
 * @brief Prints an error message to stderr, and terminates the program
 * with EXIT_FAILURE.
 *
 * @param message to be printed
 */
void error_exit(char *message) {
    fprintf(stderr, "%s: \n", message);
    exit(EXIT_FAILURE);
}

/**
 * @brief Prints a usage message to stderr, and terminates the program
 * with EXIT_FAILURE.
 */
void usage(void) {
    fprintf(stderr, "usage: %s EDGE1 EDGE2 ...\n", program_name);
    exit(EXIT_FAILURE);
}

/**
 * @brief Swaps the values of two indices in an array.
 *
 * @param arr the array
 * @param i first index
 * @param j second index
 */
void swap(int **arr, int i, int j) {
    int tmp = (*arr)[i];
    (*arr)[i] = (*arr)[j];
    (*arr)[j] = tmp;
}

/**
 *
 * @param target
 * @param vertices_s
 */
void random_vertices_permutation(int **target, int vertices_s) {
    time_t t;
    srand((unsigned) time (&t));

    for (int i = 0; i < vertices_s - 2; i++) {
        int j = rand() % vertices_s;
        swap(target, i, j);
    }
}

/**
 *
 *
 * @param fb
 * @param g
 */
void add_fb_set_to_graph(graph *fb, graph *g) {
    for (int i = g->size_v - 1; i > 0; i--) {
        for (int j = i - 1; j >= 0; j--) {
            edge e = edge_constr(g->vertices[i], g->vertices[j]);
            if (graph_contains_edge(g,&e) == 1) {
                graph_add(fb, &e);
            }
        }
    }
}

int main(int argc, char **argv) {

    if (argc < 2) {
        usage();
    }

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

    for (int i = 0; i < 3; i++) {
        random_vertices_permutation(&g.vertices, g.size_v);

        graph fb = graph_constr();

        add_fb_set_to_graph(&fb, &g);

        fprintf(stdout, " g: ");
        graph_print(&g, stdout);
        fprintf(stdout, "fb: ");
        graph_print(&fb, stdout);

        graph_destr(&fb);
    }
    graph_destr(&g);
    return EXIT_SUCCESS;
}

