/**
 * @file supervisor.c
 * @author Kristijan Todorovic, [e11806442(at)student.tuwien.ac.at]
 * @date 28.11.2023
 */

#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include "graph_util.h"
#include "circular_buffer.h"

/** the program name */
const char *program_name;

/**
 * @brief Prints a usage message to stderr, and terminates the program
 * with EXIT_FAILURE.
 */
static void usage(void) {
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
static void swap(int **arr, int i, int j) {
    int tmp = (*arr)[i];
    (*arr)[i] = (*arr)[j];
    (*arr)[j] = tmp;
}

/**
 * @brief This function creates a random permutation of the vertices array in a graph g.
 *
 * @param g the specified graph
 */
static void random_vertices_permutation(graph *g) {
    for (int i = g->size_v - 1; i >= 0; --i) {
        int j = rand() % (i + 1);
        swap(&g->vertices, i, j);
    }
}

/**
 * @brief Calculates a solution for the feedback arc set problem and adds it to a feedback arc graph
 *
 * @details This function iterates through the vertex list of graph g, which is a random
 * permutation created before calling this function.
 *
 * @param fb feedback arc graph, where edges are added to
 * @param g graph from which vertices and edges are read
 */
static void add_fb_set_to_graph(graph *fb, graph *g) {
    for (int i = g->size_v - 1; i > 0; i--) {
        for (int j = i - 1; j >= 0; j--) {
            edge e = edge_constr(g->vertices[i], g->vertices[j]);
            if (graph_contains_edge(g,&e) == 1) {
                graph_add(fb, &e);
            }
        }
    }
}

/**
 * @brief The main logic of this generator program, receiving a graphs edges as input,
 * calculating possible solutions for the minimum feedback arc set problem, and writing
 * these solutions to the circular buffer.
 *
 * @param argc argument counter
 * @param argv argument values
 * @return EXIT_SUCCESS on successful termination, otherwise EXIT_FAILURE
 */
int main(int argc, char **argv) {

    if (argc < 2) {
        usage();
    }

    program_name = argv[0];

    srand((unsigned) time (NULL));

    graph g = graph_constr();

    for (int i = 1; i < argc; i++) {
        int u, v;
        if (sscanf(argv[i], "%d-%d", &u, &v) != 2) {
            usage();
        }
        edge e = edge_constr(u, v);
        graph_add(&g, &e);
    }

    circular_buffer *cb = circular_buffer_setup(0);

    while (cb->terminate == 0) {
        random_vertices_permutation(&g);
        graph fb_arc_set = graph_constr();
        add_fb_set_to_graph(&fb_arc_set, &g);
        if (fb_arc_set.size_e <= MAX_EDGES) {
            circular_buffer_write(cb, &fb_arc_set);
        }
    }


    circular_buffer_destr(cb, 0);

    graph_destr(&g);
    return EXIT_SUCCESS;
}

