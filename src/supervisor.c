/**
 * @file supervisor.c
 * @author Kristijan Todorovic, [e11806442(at)student.tuwien.ac.at]
 * @date 28.11.2023
 */

#include <stdlib.h>
#include <getopt.h>
#include <errno.h>
#include <limits.h>
#include <sys/mman.h>
#include <signal.h>
#include <unistd.h>
#include "graph_util.h"
#include "circular_buffer.h"

/** the program name */
static const char *program_name;

volatile sig_atomic_t quit = 0;

/**
 * @brief signal handler to receive a SIGINT or SIGTERM signal
 *
 * @param signal
 */
static void handle_signal(int signal) {
    quit = 1;
}

/**
 * @brief Prints an error message to stderr, and terminates the program
 * with EXIT_FAILURE.
 *
 * @param message to be printed
 */
static void error_and_exit(char *message) {
    fprintf(stderr, "%s\n", message);
    exit(EXIT_FAILURE);
}

/**
 * @brief Prints a usage message to stderr, and terminates the program
 * with EXIT_FAILURE.
 */
static void usage(void) {
    fprintf(stderr, "usage: %s [-n limit] [-w delay] [-p]\n", program_name);
    exit(EXIT_FAILURE);
}

/**
 * @brief parses the arguments passed to this program.
 * [-n limit] specifies the limit for generated solutions
 * [-w delay] specifies the delay before this program starts to read from the circular buffer.
 *
 * @param argc argument counter
 * @param argv argument values
 * @param limit pointer to variable, which stores the specified limit for generated solutions
 * @param delay pointer to variable, which stores the specified delay in seconds
 */
static void parse_args(int argc, char **argv, int *limit, int *delay) {

    int option_index;

    while ((option_index = getopt(argc, argv, "n:w:")) != -1) {

        switch (option_index) {
            case 'n': {
                char *endptr;
                long lim = strtol(optarg, &endptr, 10);

                if (endptr[0] != '\0') {
                    usage();
                }
                if (lim < 0) {
                    usage();
                }

                *limit = (int) lim;

                break;
            }
            case 'w': {
                char *endptr;
                long del = strtol(optarg, &endptr, 10);

                if (endptr[0] != '\0') {
                    usage();
                }
                if (del < 0) {
                    usage();
                }

                *delay = (int) del;

                break;
            }
            case '?':
                usage();
                break;
            default:
                error_and_exit("Unknown error while parsing arguments");
                break;
        }
    }
}

/**
 * @brief the main logic of this supervisor program, reading solutions, which were written to
 * the circular buffer and printing out the best solution.
 *
 * @param argc argument counter
 * @param argv argument values
 * @return EXIT_SUCCESS on successful termination, otherwise EXIT_FAILURE
 */
int main(int argc, char **argv) {
    program_name = argv[0];
    int limit = -1;
    int delay = 0;

    parse_args(argc, argv, &limit, &delay);

    struct sigaction sa = {.sa_handler = handle_signal};
    sigaction(SIGINT, &sa, NULL);

    circular_buffer *cb = circular_buffer_setup(1);


    graph best = graph_constr();
    best.size_e = INT_MAX;

    sleep(delay);


    for (int i = 0; !quit && i != limit; ++i) {

        graph g = graph_constr();
        circular_buffer_read(cb, &g);

        if (g.size_e == 0) {
            graph_destr(&best);
            best = graph_constr();
            for (int j = 0; j < g.size_e; j++) {
                graph_add(&best, &g.edges[j]);
            }
            break;
        }

        if (g.size_e < best.size_e) {
            graph_destr(&best);
            best = graph_constr();
            for (int j = 0; j < g.size_e; j++) {
                graph_add(&best, &g.edges[j]);
            }
            fprintf(stderr, "Solution with %d edges: ", best.size_e);
            graph_print(&best, stderr);
        }

        graph_destr(&g);
    }

    cb->terminate = 1;

    if (best.size_e == 0) {
        fprintf(stdout, "The graph is acyclic!\n");
    } else {
        fprintf(stdout, "The graph might not be acyclic, best solution removes %d edges.\n", best.size_e);
    }

    circular_buffer_destr(cb, 1);
    //graph_destr(&best);

    return EXIT_SUCCESS;
}
