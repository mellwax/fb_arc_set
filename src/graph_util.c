/**
 * @file graph_util.c
 * @author Kristijan Todorovic <e11806442@student.tuwien.ac.at>
 * @date 03.12.2023
 */

#include "graph_util.h"
#include <stdio.h>
#include <stdlib.h>

void error_exit(char *message);

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
    g.capacity_e = 10;
    g.capacity_v = 10;
    g.edges = malloc(sizeof(edge) * g.capacity_e);
    g.vertices = malloc(sizeof(int) * g.capacity_v);

    if (g.edges == NULL || g.vertices == NULL) {
        error_exit("error allocating memory");
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

int edge_list_resize(graph *g, char c) {

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

int graph_contains_vertex(graph *g, int vertex) {
    for (size_t i = 0; i < g->capacity_v; i++) {
        if (g->vertices[i] == vertex) {
            return 1;
        }
    }
    return 0;
}

void graph_add(graph *g, edge *e) {
    if (g->size_e + 1 == g->capacity_e) {
        if (edge_list_resize(g, 'e') == -1) {
            graph_destr(g);
            error_exit("error resizing array");
        }
    }
    if (g->size_v + 2 == g->capacity_v) {
        if (edge_list_resize(g, 'v') == -1) {
            graph_destr(g);
            error_exit("error resizing array");
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

void edge_list_print(graph *g, FILE *output) {
    for (int i = 0; i < g->size_e; i++) {
        edge_print(&g->edges[i], output);
        if (i != g->size_e - 1) {
            fprintf(output, " ");
        }
    }
    fprintf(output, "\n");
}

void error_exit(char *message) {
    fprintf(stderr, "%s: \n", message);
    exit(EXIT_FAILURE);
}

/*
int main() {

    graph el = graph_constr();

    for (int i = 0; i < 7; i++) {
        edge e = edge_constr(i, i + 1);
        graph_add(&el, &e);
    }


    edge_list_print(&el, stdout);

    for (int i = 0; i < el.size_v; i++) {
        if (i == 0) {
            fprintf(stdout, "[%d, ", el.vertices[i]);
        } else if (i == el.size_v - 1) {
            fprintf(stdout, "%d]\n", el.vertices[i]);
        } else {
            fprintf(stdout, "%d, ", el.vertices[i]);
        }
    }
    fprintf(stdout, "\n");




    graph_destr(&el);
    return EXIT_SUCCESS;
}
 */