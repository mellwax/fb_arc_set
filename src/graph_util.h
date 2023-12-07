/**
 * @file graph_util.h
 * @author Kristijan Todorovic <e11806442@student.tuwien.ac.at>
 * @date 03.12.2023
 */

#ifndef FB_ARC_SET_GRAPH_UTIL_H
#define FB_ARC_SET_GRAPH_UTIL_H
#include <stdlib.h>

// u -> v
typedef struct edge {
    int u;
    int v;
} edge;

typedef struct edge_list {
    size_t size_e;
    size_t size_v;
    int capacity_e;
    int capacity_v;
    edge *edges;
    int *vertices;
} edge_list;

edge_list edge_list_constr(void);

void edge_list_destr(edge_list *el);

void edge_list_add(edge_list *el, edge *e);

void error_exit(char *message);

#endif //FB_ARC_SET_GRAPH_UTIL_H
