/**
 * @file circular_buffer.h
 * @author Kristijan Todorovic, [e11806442(at)student.tuwien.ac.at]
 * @date 28.11.2023
 */

#include "circular_buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

static void err_exit(char *message) {
    fprintf(stderr, "%s\n", message);
    exit(EXIT_FAILURE);
}

circular_buffer circular_buffer_constr(void) {
    circular_buffer cb;

    cb.wr_pos = 0;
    cb.rd_pos = 0;

    cb.sem_free = sem_open(SEM_FREE, O_CREAT | O_EXCL, 0600, MAX_DATA);
    if (cb.sem_free == SEM_FAILED) {
        err_exit("error opening SEM_FREE");
    }
    cb.sem_used = sem_open(SEM_USED, O_CREAT | O_EXCL, 0600, 0);
    if (cb.sem_used == SEM_FAILED) {
        err_exit("error opening SEM_USED");
    }
    cb.sem_mutex = sem_open(SEM_MUTEX, O_CREAT | O_EXCL, 0600, 1);
    if (cb.sem_mutex == SEM_FAILED) {
        err_exit("error opening semaphore");
    }

    return cb;
}

void circular_buffer_write(circular_buffer *cb, graph *g) {
    sem_wait(cb->sem_free);
    sem_wait(cb->sem_mutex);
    cb->data[cb->wr_pos] = *g;
    sem_post(cb->sem_mutex);
    sem_post(cb->sem_used);
    cb->wr_pos++;
    cb->wr_pos %= MAX_DATA;
}

graph *circular_buffer_read(circular_buffer *cb) {
    sem_wait(cb->sem_used);
    graph *g = &cb->data[cb->rd_pos];
    sem_post(cb->sem_free);
    cb->rd_pos++;
    cb->rd_pos %= MAX_DATA;

    return g;
}

