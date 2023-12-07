/**
 * @file graph_util.c
 * @author Kristijan Todorovic <e11806442@student.tuwien.ac.at>
 * @date 03.12.2023
 */

#include "graph_util.h"
#include <stdio.h>
#include <stdlib.h>

edge edge_constr(int u, int v) {
    edge e = {.u = u, .v = v};

    return e;
}

void edge_print(edge *e, FILE *output) {
    fprintf(output, "%d-%d", e->u, e->v);
}

edge_list edge_list_constr(void) {
    edge_list el;
    el.size_e = 0;
    el.size_v = 0;
    el.capacity_e = 10;
    el.capacity_v = 10;
    el.edges = malloc(sizeof(edge) * el.capacity_e);
    el.vertices = malloc(sizeof(int) * el.capacity_v);

    if (el.edges == NULL || el.vertices == NULL) {
        error_exit("error allocating memory");
    }

    for (int i = 0; i < el.capacity_v; i++) {
        el.vertices[i] = -1;
    }

    return el;
}

void edge_list_destr(edge_list *el) {
    free(el->edges);
    free(el->vertices);
}

int edge_list_resize(edge_list *el, char c) {

    switch (c) {
        case 'e': {
            edge *new_e = realloc(el->edges, sizeof(edge) * el->capacity_e * 2);

            if (new_e == NULL) {
                return -1;
            }

            el->edges = new_e;
            el->capacity_e *= 2;

        }
            break;
        case 'v': {
            int *new_v = realloc(el->vertices, sizeof(int) * el->capacity_v * 2);

            if (new_v == NULL) {
                return -1;
            }

            el->vertices = new_v;
            el->capacity_v *= 2;
        }
            break;
        default:
            return -1;
    }

    return 0;
}

int edge_list_contains_vertex(edge_list *el, int vertex) {
    for (size_t i = 0; i < el->capacity_v; i++) {
        if (el->vertices[i] == vertex) {
            return 1;
        }
    }
    return 0;
}

void edge_list_add(edge_list *el, edge *e) {
    if (el->size_e + 1 == el->capacity_e) {
        if (edge_list_resize(el, 'e') == -1) {
            edge_list_destr(el);
            error_exit("error resizing array");
        }
    }
    if (el->size_v + 2 == el->capacity_v) {
        if (edge_list_resize(el, 'v') == -1) {
            edge_list_destr(el);
            error_exit("error resizing array");
        }
    }

    el->edges[el->size_e++] = *e;

    if (edge_list_contains_vertex(el, e->u) == 0) {
        el->vertices[el->size_v++] = e->u;
    }
    if (edge_list_contains_vertex(el, e->v) == 0) {
        el->vertices[el->size_v++] = e->v;
    }
}

void edge_list_print(edge_list *el, FILE *output) {
    for (int i = 0; i < el->size_e; i++) {
        edge_print(&el->edges[i], output);
        if (i != el->size_e - 1) {
            fprintf(output, " ");
        }
    }
    fprintf(output, "\n");
}

void error_exit(char *message) {
    fprintf(stderr, "%s: \n", message);
    exit(EXIT_FAILURE);
}

int main() {

    edge_list el = edge_list_constr();

    for (int i = 0; i < 18; i++) {
        edge e = edge_constr(i, i + 1);
        edge_list_add(&el, &e);
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




    edge_list_destr(&el);
    return EXIT_SUCCESS;
}