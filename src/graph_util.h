/**
 * @file graph_util.h
 * @author Kristijan Todorovic <e11806442@student.tuwien.ac.at>
 * @date 03.12.2023
 */

#ifndef FB_ARC_SET_GRAPH_UTIL_H
#define FB_ARC_SET_GRAPH_UTIL_H
#include <stdio.h>

// u -> v
typedef struct edge {
    int u;
    int v;
} edge;

typedef struct graph {
    size_t size_e;
    size_t size_v;
    int capacity_e;
    int capacity_v;
    edge *edges;
    int *vertices;
} graph;

edge edge_constr(int u, int v);

graph graph_constr(void);

void graph_destr(graph *g);

void graph_add(graph *g, edge *e);

int graph_contains_vertex(graph *g, int vertex);

#endif //FB_ARC_SET_GRAPH_UTIL_H
