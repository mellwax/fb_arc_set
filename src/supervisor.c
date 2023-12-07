/**
 * @file supervisor.c
 * @author Kristijan Todorovic <e11806442@student.tuwien.ac.at>
 * @date 28.11.2023
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <errno.h>

char *program_name;

void error_exit(char *message) {
    fprintf(stderr, "%s: \n", message);
    exit(EXIT_FAILURE);
}

void usage(void) {
    fprintf(stderr, "usage: %s [-n limit] [-w delay] [-p]\n", program_name);
    exit(EXIT_FAILURE);
}

void parse_args(int argc, char *argv[], int *limit, int *delay) {

    int option_index;

    while ((option_index = getopt(argc, argv, "n:w:")) != -1) {

        switch (option_index) {
            case 'n': {
                char *endptr;
                long lim = strtol(optarg, &endptr, 10);

                if (endptr[0] != '\0')
                    usage();


                *limit = (int) lim;

                break;
            }
            case 'w': {
                char *endptr;
                long del = strtol(optarg, &endptr, 10);

                if (endptr[0] != '\0')
                    usage();

                *delay = (int) del;

                break;
            }
            case '?':
                usage();
                break;
            default:
                error_exit("Unknown error while parsing arguments");
                break;
        }
    }
}

int main(int argc, char *argv[]) {
    program_name = argv[0];
    int limit;
    int delay;

    parse_args(argc, argv, &limit, &delay);

    fprintf(stdout, "n: %d\n", limit);
    fprintf(stdout, "delay: %d\n", delay);

    return EXIT_SUCCESS;
}
