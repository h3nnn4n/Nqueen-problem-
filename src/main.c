/******************************************************************************
 * Copyright (C) 2015-2016  Renan S. Silva                                    *
 *                                                                            *
 * This software is provided 'as-is', without any express or implied          *
 * warranty. In no event will the authors be held liable for any damages      *
 * arising from the use of this software.                                     *
 *                                                                            *
 * Permission is granted to anyone to use this software for any purpose,      *
 * including commercial applications, and to alter it and redistribute it     *
 * freely, subject to the following restrictions:                             *
 *                                                                            *
 * 1. The origin of this software must not be misrepresented; you must not    *
 *    claim that you wrote the original software. If you use this software    *
 *    in a product, an acknowledgment in the product documentation would be   *
 *    appreciated but is not required.                                        *
 * 2. Altered source versions must be plainly marked as such, and must not be *
 *    misrepresented as being the original software.                          *
 * 3. This notice may not be removed or altered from any source distribution. *
 ******************************************************************************/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>

#include <mpi.h>

#include "links.h"
#include "set_gen.h"

#define __silent

int main(int argc, char *argv[]) {
    _links *m;
    _ans *O;
    int *counter_control;
    int x, y, n;
    int n_fixed_rows;
    int rank, size;
    double start_time, end_time;

    if ( argc == 2 ) {
        n = atoi(argv[1]);
        n_fixed_rows = 2;
        printf("Assuming n_fixed_rows = %d\n", n_fixed_rows);
    } else if ( argc == 3 ) {
        n = atoi(argv[1]);
        n_fixed_rows = atoi(argv[2]);
    } else {
        n = 8;
        n_fixed_rows = 2;
        printf("Assuming n = %d\n", n);
        printf("Assuming n_fixed_rows = %d\n", n_fixed_rows);
    }

    assert ( n >= n_fixed_rows && "The number of fixed lines is bigger than the total lines" );

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    assert ( size <= pow(n, n_fixed_rows) && "Not enough work to do, incread n_fixed_rows (second argument)" );

    extern unsigned long int branchs;
    extern unsigned long int solutions_found;

    branchs = 0;
    solutions_found = 0;

    counter_control = (int*) malloc ( sizeof (int*) * n_fixed_rows );
    for (int i = 0; i < n_fixed_rows; ++i)
        counter_control[i] = 0;

    if ( rank == 0 ) {
#ifndef __silent
                    printf("Starting\nCores:\t%8d\nJobs:\t%8d\n", size, (int)pow(n, n_fixed_rows));
#endif
        int counter = size;
        start_time = MPI_Wtime();
        get_size_reduced(n, &x, &y, n_fixed_rows);
        int **set;
        for (int k = 0; k < pow(n, n_fixed_rows); ++k) {
            int control = 666;
            do {
                set = get_first_rows(n, &x, &y, n_fixed_rows, counter_control);
                if ( set == NULL && control == -1 ) {
#ifndef __silent
                    printf("Nothing else to do, finishing...\n");
#endif
                    goto skip;
                } else if ( control == -1 ) {
                    assert( 0 && "This should be unreachable" );
                } else if ( set == NULL ) {
                    update_counter( n, n_fixed_rows, counter_control, &control );
                }
            } while ( set == NULL );

            if ( --counter > 0 ) {
                MPI_Send(counter_control, n_fixed_rows, MPI_INT, counter, 0, MPI_COMM_WORLD );
            } else {
                unsigned long int tmp = 0;
                MPI_Recv(&tmp, 1, MPI_UNSIGNED_LONG, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
                solutions_found += tmp;

                MPI_Send(counter_control, n_fixed_rows, MPI_INT, status.MPI_SOURCE, 0, MPI_COMM_WORLD );
            }

            update_counter( n, n_fixed_rows, counter_control, &control );

#ifndef __silent
            fprintf(stderr, " -- %2.2f\r", (k + 1)/pow(n, n_fixed_rows) * 100.0);
#endif
            fflush(stderr);
            fflush(stdout);
            // TODO: Clean up the malloc mess
        }

        free(set);
skip:

        // KILL
        for (int i = 0; i < n_fixed_rows; ++i) {
            counter_control[i] = -1;
        }

        for (int i = 1; i < size; ++i) {
            unsigned long int tmp = 0;
            MPI_Send(counter_control, n_fixed_rows, MPI_INT, i, 0, MPI_COMM_WORLD );
            MPI_Recv(&tmp, 1, MPI_UNSIGNED_LONG, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
            solutions_found += tmp;
        }

        end_time = MPI_Wtime();
        printf("%8d %8d %6.6f %14lu\n", size, (int)pow(n, n_fixed_rows), end_time - start_time, solutions_found);

        free(counter_control);
    } else {
        get_size_reduced(n, &x, &y, n_fixed_rows);
        int **data = NULL; //(int**) malloc ( sizeof(int*) * y );

        while ( 1 ) {
            MPI_Recv(counter_control, n_fixed_rows, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);

            // New kill switch
            int flag = 1;
            for (int i = 0; i < n_fixed_rows; ++i) {
                if ( counter_control[i] == -1 ) {

                } else {
                    flag = 0;
                    break;
                }
            }

            if ( flag ) break;

            data = get_first_rows(n, &x, &y, n_fixed_rows, counter_control);
            if ( data == NULL ) {
#ifndef __silent
                printf("Got bad counter from master\n");
#endif
            } else {
                m = init_torus();
                assert ( m != NULL && "Null pointer after init_torus" );

                for ( int i = 0 ; i < y ; i++){
                    insert_col_header(m);
                }

                build_links_for_dancing(m, data, x, y);

                O = (_ans*) malloc ( sizeof(_ans) );
                O->next = NULL;
                O->O = NULL;
                dancing_links(m, 0, O, n);

                free_set(data, y);
                free_ans(O);
                free_links(m);
            }

            // TODO: Clean up the malloc mess
            MPI_Send(&solutions_found, 1, MPI_UNSIGNED_LONG, 0, 0, MPI_COMM_WORLD );
            solutions_found = 0;
        }
        free(counter_control);
    }

    get_first_rows(0, NULL, NULL, 0, NULL); // This frees all static data inside the function

    MPI_Finalize();

    return EXIT_SUCCESS;
}
