/**
 * @file graph_util.h
 * @author Kristijan Todorovic, [e11806442(at)student.tuwien.ac.at]
 * @date 03.12.2023
 */

#ifndef FB_ARC_SET_GRAPH_UTIL_H
#define FB_ARC_SET_GRAPH_UTIL_H
#include <stdio.h>

/**
 * @brief a struct for a directed edge in a graph. u -> v
 */
typedef struct edge {
    int u;
    int v;
} edge;

/**
 * @brief a struct to represent a graph, using an array for the edges.
 * Also has an array containing the vertices, as well as the current size of
 * the edges and the vertices, and their capacities.
 */
typedef struct graph {
    int size_e;
    int size_v;
    int capacity_e;
    int capacity_v;
    edge *edges;
    int *vertices;
} graph;

/**
 * @brief This function returns an edge from vertex u to vertex v.
 *
 * @param u vertex u
 * @param v vertex v
 * @return an edge struct, where u -> v
 */
edge edge_constr(int u, int v);

/**
 * @brief This function returns an empty graph struct.
 *
 * @details sizes are initialized with 0, start capacities are 10 for each array,
 * which is allocated by malloc. If an error occurs during memory allocation, the program
 * terminates with EXIT_FAILURE.
 *
 * @return an empty graph
 */
graph graph_constr(void);

/**
 * @brief This function frees any memory that was allocated to the Graph g.
 *
 * @param g Graph to be destructed
 */
void graph_destr(graph *g);

/**
 * @brief This function adds an edge e to a graph g.
 *
 * @param g Graph, to which edge e shall be added
 * @param e Edge, to be added to graph g
 */
void graph_add(graph *g, edge *e);

/**
 * @brief This function checks if a given edge is contained in a given Graph.
 * If the edge is contained in the graph 1 is returned, otherwise 0.
 *
 * @param g Graph, to be checked, if edge e is contained
 * @param e the specified edge
 * @return 1, if graph g contains edge e, 0 otherwise
 */
int graph_contains_edge(graph *g, edge *e);

/**
 * @brief This function prints the edges of a Graph g to the specified output.
 *
 * @param g Graph, to be printed
 * @param output the output file
 */
void graph_print(graph *g, FILE *output);

#endif //FB_ARC_SET_GRAPH_UTIL_H
