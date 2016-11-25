#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int n;
    if ( argc != 2 ) {
        n = 8;
    } else {
        n = atoi(argv[1]);
    }

    int *pic   = ( int* ) malloc ( sizeof ( int ) * ( n       ) );
    int *row   = ( int* ) malloc ( sizeof ( int ) * ( n       ) );
    int *col   = ( int* ) malloc ( sizeof ( int ) * ( n       ) );

    int *diag1 = ( int* ) malloc ( sizeof ( int ) * ( 2*n - 1 ) );
    int *diag2 = ( int* ) malloc ( sizeof ( int ) * ( 2*n - 1 ) );

    printf("%d %d %d\n", n * 3 + 2*(n*2-1), n*n*n, n);

    for (int piece = 0; piece < n; ++piece) {
        for (int x = 0; x < n; ++x) {
            for (int y = 0; y < n; ++y) {
                for (int i = 0; i < 2*n-1; ++i) {
                    if ( i < n ) {
                        row[i]   = 0;
                        col[i]   = 0;
                    }
                    diag1[i] = 0;
                    diag2[i] = 0;
                }

                row  [x        ] = 1;
                col  [y        ] = 1;
                diag1[x-y+(n-1)] = 1;
                diag2[x+y      ] = 1;

                // Prints the piece set
                for (int k = 0; k < n; ++k) {
                    if ( k == piece ) {
                        printf("1 ");
                    } else {
                        printf("0 ");
                    }
                }

#ifdef _space
                printf("  -   ");
#endif

                // Prints the row set
                for (int k = 0; k < n; ++k)
                    printf("%d ", row[k]);

#ifdef _space
                printf("  -   ");
#endif

                // Prints the column set
                for (int k = 0; k < n; ++k)
                    printf("%d ", col[k]);

#ifdef _space
                printf("  -   ");
#endif

                // Prints the main diag set
                for (int k = 0; k < n*2 - 1; ++k)
                    printf("%d ", diag1[k]);

#ifdef _space
                printf("  -   ");
#endif

                // Prints the other diag set
                for (int k = 0; k < n*2 - 1; ++k)
                    printf("%d ", diag2[k]);

                printf("\n");
            }
        }
    }

    return 0;
}
