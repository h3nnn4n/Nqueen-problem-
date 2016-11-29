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
    int **set;
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

    if ( rank == 0 ) {
        int counter = size;
        start_time = MPI_Wtime();
        get_size_reduced(n, &x, &y, n_fixed_rows);
        int data_size = x * y * sizeof( int );
        int *outbuf = (int*) malloc ( data_size );
        for (int k = 0; k < pow(n, n_fixed_rows); ++k) {
            int pos = 0;
            int control = 666;
            do {
                set = get_first_rows(n, &x, &y, n_fixed_rows, &control);
                if ( set == NULL ) {
                    if ( control == -1 ) {
                        printf("Nothing else to do, finishing...\n");
                        goto skip;
                    }
                }
            } while ( set == NULL && control > -1 );

            for (int i = 0; i < x; ++i) {
                for (int j = 0; j < y; ++j) {
                    outbuf[pos++] = set[j][i];
                }
            }

            if ( --counter > 0 ) {
                MPI_Send(outbuf, x * y, MPI_INT, counter, 0, MPI_COMM_WORLD );
            } else {
                unsigned long int tmp = 0;
                MPI_Recv(&tmp, 1, MPI_UNSIGNED_LONG, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
                solutions_found += tmp;

                MPI_Send(outbuf, x * y, MPI_INT, status.MPI_SOURCE, 0, MPI_COMM_WORLD );
            }

            fprintf(stderr, " -- %2.2f\r", (k + 1)/pow(n, n_fixed_rows) * 100.0);
            fflush(stderr);
            fflush(stdout);
            // TODO: Clean up the malloc mess
        }
skip:

        // KILL
        outbuf[0] = -1;
        outbuf[1] = -1;
        outbuf[2] = -1;

        for (int i = 1; i < size; ++i) {
            unsigned long int tmp = 0;
            MPI_Send(outbuf, x * y, MPI_INT, i, 0, MPI_COMM_WORLD );
            MPI_Recv(&tmp, 1, MPI_UNSIGNED_LONG, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
            solutions_found += tmp;
        }

        end_time = MPI_Wtime();
        printf("%d %f %lu\n", size, end_time - start_time, solutions_found);
    } else {
        get_size_reduced(n, &x, &y, n_fixed_rows);
        int data_size = x * y * sizeof( int );
        int *inbuf = (int*) malloc ( data_size );
        int **data = (int**) malloc ( sizeof(int*) * y );
        for ( int i = 0 ; i < y ; i++)
            data[i] = (int*) malloc ( sizeof(int) * x );

        while ( 1 ) {
            MPI_Recv(inbuf, x * y, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);

            if ( inbuf[0] == -1 && inbuf[1] == -1 && inbuf[2] == -1 ) break;

            int pos = 0;
            for (int i = 0; i < x; ++i) {
                for (int j = 0; j < y; ++j) {
                    data[j][i] = inbuf[pos++];
                }
            }

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
