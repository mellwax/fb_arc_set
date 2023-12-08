/**
 * @file circular_buffer.h
 * @author Kristijan Todorovic, [e11806442(at)student.tuwien.ac.at]
 * @date 28.11.2023
 */

#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <semaphore.h>
#include "graph_util.h"

#define SHM_NAME "/11806442_shm"
#define MAX_DATA (32)

#define SEM_FREE "/11806442_sem_free"
#define SEM_USED "/11806442_sem_used"
#define SEM_MUTEX "/11806442_sem_mutex"

typedef struct circular_buffer {
    int wr_pos;
    int rd_pos;
    graph data[MAX_DATA];
    sem_t *sem_free;
    sem_t *sem_used;
    sem_t *sem_mutex;
} circular_buffer;

circular_buffer circular_buffer_constr(void);

void circular_buffer_write(circular_buffer *cb, graph *g);

graph *circular_buffer_read(circular_buffer *cb);

#endif  //CIRCULAR_BUFFER_H

