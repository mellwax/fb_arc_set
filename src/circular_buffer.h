/**
 * @file circular_buffer.h
 * @author Kristijan Todorovic, [e11806442(at)student.tuwien.ac.at]
 * @date 28.11.2023
 *
 * @brief The header file for the circular buffer, implemented through @file circular_buffer.c
 *
 * @details These functions are used to set up the circular buffer using shared memory and named semaphores.
 * Data, edges of a graph, are written to this buffer by a generator program.
 * This written data is then read by a supervisor program.
 */

#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <semaphore.h>
#include "graph_util.h"

/**
 * shared memory name and its max capacity
 * maximum 60 sets with a maximum of 8 edges
 */
#define SHM_NAME "/11806442_shm"
#define MAX_DATA (60)
#define MAX_EDGES (8)

/**
 * named semaphores to synchronize reading and mutually exclusive writing.
 */
#define SEM_FREE "/11806442_sem_free"
#define SEM_USED "/11806442_sem_used"
#define SEM_MUTEX "/11806442_sem_mutex"


/**
 * struct implementing the circular buffer, memory size is 4092 bytes
 */
typedef struct circular_buffer {
    int terminate;
    int wr_pos;
    int rd_pos;
    int edge_s[MAX_DATA];
    edge edge[MAX_DATA][MAX_EDGES];
} circular_buffer;

/**
 * @brief Sets up the circular buffer and returns the pointer to the shared memory.
 *
 * @details This function opens up the shared memory, semaphores and sets up the circular buffer.
 * Errors, which may occur during these operations, are handled by this function and will terminate
 * the program with EXIT_FAILURE and print an error message to stderr.
 *
 * @param server specifies, whether the caller (1) is the server or a client (0)
 * @return returns a pointer to the shared memory
 */
circular_buffer *circular_buffer_setup(int server);

/**
 * @brief This function writes a feedback arc set to the shared memory circular buffer.
 *
 * @details Synchronization is ensured by the named semaphores, the caller of this function
 * has to make sure, that not more than MAX_EDGES are contained within the feedback arc graph.
 *
 * @param cb the circular buffer
 * @param fb_g feedback arc set to be written
 */
void circular_buffer_write(circular_buffer *cb, graph *fb_g);

/**
 * @brief This function reads the edges, of the next feedback arc set, and adds them to the graph g.
 *
 * @param cb the circular buffer
 * @param g the graph, where edges from the buffer are added to
 */
void circular_buffer_read(circular_buffer *cb, graph *g);

/**
 * @brief This function destructs the circular buffer, closing and unlinking the shared memory and semaphores.
 *
 * @details Errors, which may occur during these operations, are handled by this function and will terminate
 * the program with EXIT_FAILURE and print an error message to stderr.
 *
 * @param cb the circular buffer
 * @param server specifies, whether the caller (1) is the server or a client (0)
 */
void circular_buffer_destr(circular_buffer *cb, int server);

#endif  //CIRCULAR_BUFFER_H

