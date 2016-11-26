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
