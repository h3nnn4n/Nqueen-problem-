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

#include "links.h"
#include "set_gen.h"

int main(int argc, char *argv[]) {
    _links *m;
    _ans *O;
    int **set;
    int x, y, n;
    int n_fixed_rows = atoi(argv[2]);

    /*if ( argc != 2 ) {*/
        /*n = 8;*/
    /*} else {*/
    n = atoi(argv[1]);
    /*}*/

    extern unsigned long int branchs;
    extern unsigned long int solutions_found;

    branchs = 0;
    solutions_found = 0;
    O = NULL;

    for (int k = 0; k < pow(n, n_fixed_rows); ++k) {
        set = get_first_rows(n, &x, &y, n_fixed_rows);

        /*set = gen_set(n, &x, &y);*/

        m = init_torus();

        for ( int i = 0 ; i < y ; i++){
            insert_col_header(m);
        }

        build_links_for_dancing(m, set, x, y);

        O = (_ans*) malloc ( sizeof(_ans) );
        O->next = NULL;
        O->O = NULL;
        dancing_links(m, 0, O, n);

        /*for (int j = 0; j < y; ++j) {*/
            /*free(set[j]);*/
        /*}*/
        /*free(set);*/
        /*free(m  );*/
        /*free(O  );*/
    }

    return EXIT_SUCCESS;
}
