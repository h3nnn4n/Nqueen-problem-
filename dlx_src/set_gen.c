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

int** gen_set(int n, int* xx, int* yy) {
    int **set;

    *yy = n * 2 + 2*(n*2-1);
    *xx = n*n;

    set = (int**) malloc ( sizeof(int*) * (*yy) );

    for ( int i = 0 ; i < *yy ; i++)
        set[i] = (int*) malloc ( sizeof(int) * (*xx) );

    for (int x = 0; x < n; ++x) {
        for (int y = 0; y < n; ++y) {
            for (int k = 0; k < *yy; ++k)
                set[k][x*n + y] = 0;

            set[      x                    ][x*n + y] = 1;
            set[n   + y                    ][x*n + y] = 1;
            set[2*n + x-y+(n-1)            ][x*n + y] = 1;
            set[2*n + 2*(n - 1) + x + y +1 ][x*n + y] = 1;
        }
    }
    return set;
}
