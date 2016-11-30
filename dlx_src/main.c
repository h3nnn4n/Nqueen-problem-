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

int main(int argc, char *argv[]) {
    _links *m;
    _ans *O;
    /*int **set;*/
    int *counter_control;
    int x, y, n;
    int n_fixed_rows;
    int rank, size;
    double start_time, end_time;

    if ( argc != 3 ) {
        n = 8;
        n_fixed_rows = 4;
    } else {
        n = atoi(argv[1]);
        n_fixed_rows = atoi(argv[2]);
    }

    assert ( n >= n_fixed_rows );

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    assert ( size <= n );

    extern unsigned long int branchs;
    extern unsigned long int solutions_found;

    branchs = 0;
    solutions_found = 0;

    counter_control = (int*) malloc ( sizeof (int*) * n_fixed_rows );
    for (int i = 0; i < n_fixed_rows; ++i)
        counter_control[i] = 0;

    if ( rank == 0 ) {
        int counter = size;
        start_time = MPI_Wtime();
        get_size_reduced(n, &x, &y, n_fixed_rows);
        /*int data_size = x * y * sizeof( int );*/
        /*int *outbuf = (int*) malloc ( data_size );*/
        for (int k = 0; k < pow(n, n_fixed_rows); ++k) {
            /*int pos = 0;*/
            int control = 666;
            /*do {*/
                /*set = get_first_rows(n, &x, &y, n_fixed_rows, &control, counter_control);*/
                /*if ( set == NULL ) {*/
                    /*if ( control == -1 ) {*/
                        /*printf("Nothing else to do, finishing...\n");*/
                        /*goto skip;*/
                    /*}*/
                /*}*/
            /*} while ( set == NULL && control > -1 );*/

            /*for (int i = 0; i < x; ++i) {*/
                /*for (int j = 0; j < y; ++j) {*/
                    /*outbuf[pos++] = set[j][i];*/
                /*}*/
            /*}*/

            if ( --counter > 0 ) {
                /*printf(" Sending first job to %d\n", counter);*/
                MPI_Send(counter_control, n_fixed_rows, MPI_INT, counter, 0, MPI_COMM_WORLD );
            } else {
                unsigned long int tmp = 0;
                /*printf("  Waiting on Recv\n");*/
                MPI_Recv(&tmp, 1, MPI_UNSIGNED_LONG, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
                solutions_found += tmp;

                /*printf("   Got ans from %d = %lu\n", status.MPI_SOURCE, tmp);*/

                MPI_Send(counter_control, n_fixed_rows, MPI_INT, status.MPI_SOURCE, 0, MPI_COMM_WORLD );
                /*printf("Sent more work to %d\n", status.MPI_SOURCE);*/
            }

            update_counter( n, n_fixed_rows, counter_control, &control );

            fprintf(stderr, " -- %2.2f\r", (k + 1)/pow(n, n_fixed_rows) * 100.0);
            fflush(stderr);
            fflush(stdout);
            // TODO: Clean up the malloc mess
        }
/*skip:*/

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
        printf("%d %f %lu\n", size, end_time - start_time, solutions_found);
    } else {
        get_size_reduced(n, &x, &y, n_fixed_rows);
        /*int data_size = x * y * sizeof( int );*/
        /*int *inbuf = (int*) malloc ( data_size );*/
        int **data = NULL; //(int**) malloc ( sizeof(int*) * y );
        /*for ( int i = 0 ; i < y ; i++)*/
            /*data[i] = (int*) malloc ( sizeof(int) * x );*/

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

            m = init_torus();

            for ( int i = 0 ; i < y ; i++){
                insert_col_header(m);
            }

            build_links_for_dancing(m, data, x, y);

            O = (_ans*) malloc ( sizeof(_ans) );
            O->next = NULL;
            O->O = NULL;
            dancing_links(m, 0, O, n);

            // TODO: Clean up the malloc mess
            MPI_Send(&solutions_found, 1, MPI_UNSIGNED_LONG, 0, 0, MPI_COMM_WORLD );
            solutions_found = 0;
        }
    }

    MPI_Finalize();

    return EXIT_SUCCESS;
}
