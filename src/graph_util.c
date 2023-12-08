/**
 * @file graph_util.c
 * @author Kristijan Todorovic, [e11806442(at)student.tuwien.ac.at]
 * @date 03.12.2023
 */

#include "graph_util.h"
#include <stdlib.h>

#define INITIAL_CAPACITY (8)

/**
 * @brief Prints an error message to stderr, and terminates the program
 * with EXIT_FAILURE.
 *
 * @param message to be printed
 */
static void error_and_exit(char *message) {
    fprintf(stderr, "%s: \n", message);
    exit(EXIT_FAILURE);
}

/**
 * @brief This function returns an edge from vertex u to vertex v.
 *
 * @param u vertex u
 * @param v vertex v
 * @return an edge struct, where u -> v
 */
edge edge_constr(int u, int v) {
    edge e = {.u = u, .v = v};

    return e;
}

/**
 * @brief prints an edge to the specified output e.g. 1-2, representing a
 * directed edge from vertex u to vertex v.
 *
 * @param e edge to be printed
 * @param output output file to be printed to
 */
static void edge_print(edge *e, FILE *output) {
    fprintf(output, "%d-%d", e->u, e->v);
}

/**
 * @brief This function returns an empty graph struct.
 *
 * @details sizes are initialized with 0, start capacities are 8 for each array,
 * which is allocated by malloc. If an error occurs during memory allocation, the program
 * terminates with EXIT_FAILURE.
 *
 * @return an empty graph
 */
graph graph_constr(void) {
    graph g;
    g.size_e = 0;
    g.size_v = 0;
    g.capacity_e = INITIAL_CAPACITY;
    g.capacity_v = INITIAL_CAPACITY;
    g.edges = malloc(sizeof(edge) * g.capacity_e);
    g.vertices = malloc(sizeof(int) * g.capacity_v);

    if (g.edges == NULL || g.vertices == NULL) {
        error_and_exit("error allocating memory");
    }

    for (int i = 0; i < g.capacity_v; i++) {
        g.vertices[i] = -1;
    }

    return g;
}

/**
 * @brief This function frees any memory that was allocated to the Graph g.
 *
 * @param g Graph to be destructed
 */
void graph_destr(graph *g) {
    free(g->edges);
    free(g->vertices);
}

/**
 * @brief This function resizes either the edges array or the vertices array of a graph g.
 * The character c specifies whether the edges 'e' or the vertices 'v' shall be resized.
 *
 * @param g pointer to the graph to be resized
 * @param c specifying, which array to resize
 * @return
 */
static int graph_resize(graph *g, char c) {

    switch (c) {
        case 'e': {
            edge *new_e = realloc(g->edges, sizeof(edge) * g->capacity_e * 2);

            if (new_e == NULL) {
                return -1;
            }

            g->edges = new_e;
            g->capacity_e *= 2;

        }
            break;
        case 'v': {
            int *new_v = realloc(g->vertices, sizeof(int) * g->capacity_v * 2);

            if (new_v == NULL) {
                return -1;
            }

            g->vertices = new_v;
            g->capacity_v *= 2;
        }
            break;
        default:
            return -1;
    }

    return 0;
}

/**
 * @brief This function checks if a given edge is contained in a given Graph.
 * If the edge is contained in the graph 1 is returned, otherwise 0.
 *
 * @param g Graph, to be checked, if edge e is contained
 * @param e the specified edge
 * @return 1, if graph g contains edge e, 0 otherwise
 */
int graph_contains_edge(graph *g, edge *e) {
    for (int i = 0; i < g->size_e; i++) {
        edge g_e = g->edges[i];
        if (g_e.u == e->u && g_e.v == e->v) {
            return 1;
        }
    }
    return 0;
}

/**
 * @brief This function checks if a given vertex is contained in a given Graph.
 * If the vertex is contained in the graph 1 is returned, otherwise 0.
 *
 * @param g to be checked, if vertex is contained
 * @param vertex the specified vertex
 * @return 1, if graph g contains vertex, 0 otherwise
 */
static int graph_contains_vertex(graph *g, int vertex) {
    for (int i = 0; i < g->capacity_v; i++) {
        if (g->vertices[i] == vertex) {
            return 1;
        }
    }
    return 0;
}

/**
 * @brief This function adds an edge e to a graph g.
 *
 * @details memory is dynamically allocated by malloc(3) and if needed reallocated
 * more memory by realloc(3).
 *
 * @param g Graph, to which edge e shall be added
 * @param e Edge, to be added to graph g
 */
void graph_add(graph *g, edge *e) {
    if (g->size_e + 1 == g->capacity_e) {
        if (graph_resize(g, 'e') == -1) {
            graph_destr(g);
            error_and_exit("error resizing array");
        }
    }
    if (g->size_v + 2 >= g->capacity_v) {
        if (graph_resize(g, 'v') == -1) {
            graph_destr(g);
            error_and_exit("error resizing array");
        }
    }

    g->edges[g->size_e++] = *e;

    if (graph_contains_vertex(g, e->u) == 0) {
        g->vertices[g->size_v++] = e->u;
    }
    if (graph_contains_vertex(g, e->v) == 0) {
        g->vertices[g->size_v++] = e->v;
    }
}

/**
 * @brief This function prints the edges of a Graph g to the specified output.
 * e.g. 0-1 1-2 0-2
 *
 * @param g Graph, to be printed
 * @param output the output file
 */
void graph_print(graph *g, FILE *output) {
    for (int i = 0; i < g->size_e; i++) {
        edge_print(&g->edges[i], output);
        if (i != g->size_e - 1) {
            fprintf(output, " ");
        }
    }
    fprintf(output, "\n");
}

