/**
 * @file circular_buffer.h
 * @author Kristijan Todorovic, [e11806442(at)student.tuwien.ac.at]
 * @date 28.11.2023
 *
 * @brief The circular buffer for generated solutions.
 *
 * @details These functions are used to set up the circular buffer using shared memory and named semaphores.
 * Data, edges of a graph, are written to this buffer by a generator program.
 * This written data is then read by a supervisor program.
 */

#include "circular_buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

/**
 * semaphores
 */
sem_t *sem_free;
sem_t *sem_used;
sem_t *sem_mutex;

/**
 * shared memory file descriptor
 */
int shmfd;

/**
 * @brief Prints an error message to stderr, and terminates the program
 * with EXIT_FAILURE.
 *
 * @param message to be printed
 */
static void err_exit(char *message) {
    fprintf(stderr, "%s\n", message);
    exit(EXIT_FAILURE);
}

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
circular_buffer *circular_buffer_setup(int server) {

    if (server == 1) {
        shmfd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0600);
    } else {
        shmfd = shm_open(SHM_NAME, O_RDWR, 0);
    }

    if (shmfd == -1) {
        err_exit("error opening shm");
    }

    if (server == 1) {
        if (ftruncate(shmfd, sizeof(circular_buffer)) < 0) {
            if (close(shmfd) == -1) {
                err_exit("error closing shm fd");
            }
            if (shm_unlink(SHM_NAME) == -1) {
                err_exit("error unlinking shm");
            }
            err_exit("error during ftruncate");
        }
    }

    circular_buffer *cb = mmap(NULL, sizeof(circular_buffer), PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);

    if (cb == MAP_FAILED) {
        if (close(shmfd) == -1) {
            err_exit("failed to close shm file descriptor");
        }
        if (server == 1) {
            if (shm_unlink(SHM_NAME) == -1) {
                err_exit("error unlinking shm");
            }
        }
        err_exit("mapping failed");
    }

    if (server == 1) {
        sem_free = sem_open(SEM_FREE, O_CREAT | O_EXCL, 0600, MAX_DATA);
        sem_used = sem_open(SEM_USED, O_CREAT | O_EXCL, 0600, 0);
        sem_mutex = sem_open(SEM_MUTEX, O_CREAT | O_EXCL, 0600, 1);
    } else {
        sem_free = sem_open(SEM_FREE, 0);
        sem_used = sem_open(SEM_USED, 0);
        sem_mutex = sem_open(SEM_MUTEX, 0);
    }

    if (sem_free == SEM_FAILED) {
        if (munmap(cb, sizeof(*cb)) == -1) {
            err_exit("unmapping failed");
        }
        if (close(shmfd) == -1) {
            err_exit("failed to close shm file descriptor");
        }
        if (server == 1) {
            if (shm_unlink(SHM_NAME) == -1) {
                err_exit("error unlinking shm");
            }
            if (sem_unlink(SEM_FREE) == -1) {
                err_exit("error unlinking sem_free");
            }
        }
        err_exit("failed to open sem_free");
    }

    if (sem_used == SEM_FAILED) {
        if (munmap(cb, sizeof(*cb)) == -1) {
            err_exit("error unmapping shm");
        }
        if (close(shmfd) == -1) {
            err_exit("failed to close shm file descriptor");
        }
        if (server == 1) {
            if (shm_unlink(SHM_NAME) == -1) {
                err_exit("error unlinking shm");
            }
        }
        if (sem_close(sem_free) == -1) {
            err_exit("error closing sem_free");
        }
        if (server == 1) {
            if (sem_unlink(SEM_FREE) == -1) {
                err_exit("error unlinking sem_free");
            }
            if (sem_unlink(SEM_USED) == -1) {
                err_exit("error unlinking sem_used");
            }
        }
        err_exit("failed to open sem_used");
    }

    if (sem_mutex == SEM_FAILED) {
        if (munmap(cb, sizeof(*cb)) == -1) {
            err_exit("error unmapping shm");
        }
        if (close(shmfd) == -1) {
            err_exit("failed to close shm file descriptor");
        }
        if (server == 1) {
            if (shm_unlink(SHM_NAME) == -1) {
                err_exit("error unlinking shm");
            }
        }
        if (sem_close(sem_free) == -1) {
            err_exit("error closing sem_free");
        }
        if (sem_close(sem_used) == -1) {
            err_exit("error closing sem_used");
        }
        if (server == 1) {
            if (sem_unlink(SEM_FREE) == -1) {
                err_exit("error unlinking sem_free");
            }
            if (sem_unlink(SEM_USED) == -1) {
                err_exit("error unlinking sem_used");
            }
            if (sem_unlink(SEM_MUTEX) == -1) {
                err_exit("error unlinking sem_mutex");
            }
        }
        err_exit("failed to open sem_mutex");
    }

    cb->rd_pos = 0;
    cb->wr_pos = 0;
    cb->terminate = 0;

    return cb;
}

/**
 * @brief This function writes a feedback arc set to the shared memory circular buffer.
 *
 * @details Synchronization is ensured by the named semaphores, the caller of this function
 * has to make sure, that not more than MAX_EDGES are contained within the feedback arc graph.
 *
 * @param cb the circular buffer
 * @param fb_g feedback arc set to be written
 */
void circular_buffer_write(circular_buffer *cb, graph *fb_g) {
    sem_wait(sem_free);
    sem_wait(sem_mutex);
    for (int i = 0; i < fb_g->size_e; i++) {
        cb->edge[cb->wr_pos][i] = fb_g->edges[i];
    }
    cb->edge_s[cb->wr_pos] = fb_g->size_e;
    cb->wr_pos += 1;
    cb->wr_pos %= MAX_DATA;
    sem_post(sem_mutex);
    sem_post(sem_used);
}

/**
 * @brief This function reads the edges, of the next feedback arc set, and adds them to the graph g.
 *
 * @param cb the circular buffer
 * @param g the graph, where edges from the buffer are added to
 */
void circular_buffer_read(circular_buffer *cb, graph *g) {
    sem_wait(sem_used);
    for (int i = 0; i < cb->edge_s[cb->rd_pos]; i++) {
        graph_add(g, &cb->edge[cb->rd_pos][i]);
    }
    cb->rd_pos += 1;
    cb->rd_pos %= MAX_DATA;
    sem_post(sem_free);
}

/**
 * @brief This function destructs the circular buffer, closing and unlinking the shared memory and semaphores.
 *
 * @details Errors, which may occur during these operations, are handled by this function and will terminate
 * the program with EXIT_FAILURE and print an error message to stderr.
 *
 * @param cb the circular buffer
 * @param server specifies, whether the caller (1) is the server or a client (0)
 */
void circular_buffer_destr(circular_buffer *cb, int server) {
    munmap(cb, sizeof(*cb));

    close(shmfd);

    if (server == 1) {
        shm_unlink(SHM_NAME);
    }

    sem_close(sem_free);
    sem_close(sem_used);
    sem_close(sem_mutex);

    if (server == 1) {
        sem_unlink(SEM_FREE);
        sem_unlink(SEM_USED);
        sem_unlink(SEM_MUTEX);
    }
}
