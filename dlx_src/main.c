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

#include "links.h"
#include "set_gen.h"

int main(int argc, char *argv[]) {
    _links *m;
    int **set;
    int x, y, i, n;

    if ( argc != 2 ) {
        n = 8;
    } else {
        n = atoi(argv[1]);
    }

    extern unsigned long int branchs;
    extern unsigned long int solutions_found;

    branchs = 0;
    solutions_found = 0;

    set = get_first_rows(n, &x, &y, 3);

    /*set = gen_set(n, &x, &y);*/

    m = init_torus();

    for ( i = 0 ; i < y ; i++){
        insert_col_header(m);
    }

    build_links_for_dancing(m, set, x, y);

    _ans *O = (_ans*) malloc ( sizeof(_ans) );
    dancing_links(m, 0, O, n);

    return EXIT_SUCCESS;
}
