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
#include <stdlib.h>
#include <assert.h>

#include "set_gen.h"

int** gen_set(int n, int* xx, int* yy) {
    int **set;

    *yy = n * 2 + 2*(n*2-1) - 4;
    *xx = n*n;

    set = (int**) malloc ( sizeof(int*) * (*yy) );

    if ( n % 2 == 0 && 1 ) {
        *xx -= n/2;

        for ( int i = 0 ; i < *yy ; i++)
            set[i] = (int*) malloc ( sizeof(int) * (*xx) );

        int p = 0;
        for (int x = 0; x < n; ++x) {
            for (int y = 0; y < n; ++y) {
                if ( n % 2 == 0 && x == 0 && y >= (n/2) ) continue; // Remove half the solutions, should be a 2x speedup
                for (int k = 0; k < *yy; ++k)
                    set[k][p] = 0;

                set[      x                        ][p] = 1;
                set[n   + y                        ][p] = 1;

                if ( x - y + (n - 1) == 1 || x - y + (n - 1) == 2*n - 1 ) { } else
                set[2*n + x - y + (n - 1)       - 2][p] = 1;

                if ( x + y == 0 || x + y == 2*n - 2 ) { } else
                set[2*n + (2*(n - 1)-2) + x + y    ][p] = 1;

                p++;
            }
        }
    } else {
        for ( int i = 0 ; i < *yy ; i++)
            set[i] = (int*) malloc ( sizeof(int) * (*xx) );

        for (int x = 0; x < n; ++x) {
            for (int y = 0; y < n; ++y) {
                for (int k = 0; k < *yy; ++k)
                    set[k][x*n + y] = 0;

                set[      x                        ][x*n + y] = 1;
                set[n   + y                        ][x*n + y] = 1;

                if ( x - y + (n - 1) == 0 || x - y + (n - 1) == 2*n - 2 ) { } else
                set[2*n + x - y + (n - 1)       - 1][x*n + y] = 1;

                if ( x + y == 0 || x + y == 2*n - 2 ) { } else
                set[2*n + (2*(n - 1)-2) + x + y    ][x*n + y] = 1;
            }
        }
    }
    return set;
}

int **get_first_rows(int n, int *xx, int *yy, int n_fixed_rows/*, int *control*/, int *counter) {
    static int   first_run = 1;
    static int **set       = NULL;
    int check[n * 2 + 2*(n*2-1) - 4];

    *yy = n * 2 + 2*(n*2-1) - 4;
    *xx = n*n;

    // TODO: Remove symmetry
    // This initializes the full set only once
    if ( first_run ) {
        first_run = 0;

        /*counter = (int*) malloc ( sizeof (int*) * n_fixed_rows );*/
        /*for (int i = 0; i < n_fixed_rows; ++i)*/
            /*counter[i] = 0;*/

        set = (int**) malloc ( sizeof(int*) * (*yy) );
        for ( int i = 0 ; i < *yy ; i++)
            set[i] = (int*) malloc ( sizeof(int) * (*xx) );

        for (int x = 0; x < n; ++x) {
            for (int y = 0; y < n; ++y) {
                for (int k = 0; k < *yy; ++k)
                    set[k][x*n + y] = 0;

                set[      x                        ][x*n + y] = 1;
                set[n   + y                        ][x*n + y] = 1;

                if ( x - y + (n - 1) == 0 || x - y + (n - 1) == 2*n - 2 ) { } else
                set[2*n + x - y + (n - 1)       - 1][x*n + y] = 1;

                if ( x + y == 0 || x + y == 2*n - 2 ) { } else
                set[2*n + (2*(n - 1)-2) + x + y    ][x*n + y] = 1;
            }
        }
    }

    *xx = n*n - n_fixed_rows * n + n_fixed_rows; // There will be fewer lines

    // Allocates the memory for the reduced set
    int **data = (int**) malloc ( sizeof(int*) * (*yy) );

    assert( data != NULL );
    assert( set  != NULL );

    for ( int i = 0 ; i < *yy ; i++) {
        data[i] = (int*) malloc ( sizeof(int) * (*xx) );
        assert( data[i] != NULL );
    }

    for (int i = 0; i < *yy; ++i) {
        check[i] = 0;
    }

    // Copies the fixed rows and check for feasibility
    for (int i = 0; i < n_fixed_rows; ++i) {
        for (int j = 0; j < *yy; ++j) {
            data[j][i] = set[j][i * n + counter[i]];
            /*check[j] += data[j][i];*/
            /*if ( check[j] > 1 ) { goto abort; }*/
        }
    }

    // Copies the rest of the set
    for ( int i = 0 ; i < *yy; i++) {
        for (int j = n_fixed_rows; j < *xx; ++j) {
            data[i][j] = set[i][n_fixed_rows * n + j - n_fixed_rows];
        }
    }

    /*// Updates the static counter*/
    /*update_counter( n, n_fixed_rows, counter, control );*/

    return data;

/*abort:*/
    /*for ( int i = 0 ; i < *yy ; i++)*/
        /*free(data[i]);*/
    /*free(data);*/

    /*update_counter( n, n_fixed_rows, counter, control );*/

    /*return NULL;*/
}

void update_counter( int n, int n_fixed_rows, int *counter, int *control ){
    assert ( counter != NULL );

    if ( control != NULL ) {
        int accul = 0;
        for (int j = 0; j < n_fixed_rows; ++j)
            accul += counter[j] + 1;

        if ( accul == n_fixed_rows * n )
            *control = -1;
        else
            *control = 0;
    }

    counter[0] += 1;
    for (int j = 0; j < n_fixed_rows; ++j) {
        if ( counter[j] >= n ) {
            counter[j  ] = 0;
            counter[j+1] += 1;
        }
    }
}

void get_size_reduced(int n, int *xx, int *yy, int n_fixed_rows) {
    *yy = n * 2 + 2*(n*2-1) - 4;
    *xx = n*n - n_fixed_rows * n + n_fixed_rows;
}

void get_size_full(int n, int *xx, int *yy) {
    *yy = n * 2 + 2*(n*2-1) - 4;
    *xx = n*n;
}

