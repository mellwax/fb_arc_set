/**
 * @file graph_util.c
 * @author Kristijan Todorovic, [e11806442(at)student.tuwien.ac.at]
 * @date 03.12.2023
 */

#include "graph_util.h"
#include <stdlib.h>

#define INITIAL_CAPACITY (4)

static void error_and_exit(char *message) {
    fprintf(stderr, "%s: \n", message);
    exit(EXIT_FAILURE);
}

edge edge_constr(int u, int v) {
    edge e = {.u = u, .v = v};

    return e;
}

void edge_print(edge *e, FILE *output) {
    fprintf(output, "%d-%d", e->u, e->v);
}

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

void graph_destr(graph *g) {
    free(g->edges);
    free(g->vertices);
}

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

int graph_contains_edge(graph *g, edge *e) {
    for (int i = 0; i < g->size_e; i++) {
        edge g_e = g->edges[i];
        if (g_e.u == e->u && g_e.v == e->v) {
            return 1;
        }
    }
    return 0;
}

static int graph_contains_vertex(graph *g, int vertex) {
    for (int i = 0; i < g->capacity_v; i++) {
        if (g->vertices[i] == vertex) {
            return 1;
        }
    }
    return 0;
}

void graph_add(graph *g, edge *e) {
    if (g->size_e + 1 == g->capacity_e) {
        if (graph_resize(g, 'e') == -1) {
            graph_destr(g);
            error_and_exit("error resizing array");
        }
    }
    if (g->size_v + 2 == g->capacity_v) {
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

void graph_print(graph *g, FILE *output) {
    for (int i = 0; i < g->size_e; i++) {
        edge_print(&g->edges[i], output);
        if (i != g->size_e - 1) {
            fprintf(output, " ");
        }
    }
    fprintf(output, "\n");
}

