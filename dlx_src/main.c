/************************************************************************************
 *                                                                                  *
 *     Copyright (C) 2015  Renan S. Silva                                           *
 *                                                                                  *
 *     This program is free software: you can redistribute it and/or modify         *
 *     it under the terms of the GNU General Public License as published by         *
 *     the Free Software Foundation, either version 3 of the License, or            *
 *     (at your option) any later version.                                          *
 *                                                                                  *
 *     This program is distributed in the hope that it will be useful,              *
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of               *
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                *
 *     GNU General Public License for more details.                                 *
 *                                                                                  *
 *     You should have received a copy of the GNU General Public License            *
 *     along with this program.  If not, see <http://www.gnu.org/licenses/>.        *
 *                                                                                  *
 ***********************************************************************************/

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

    set = gen_set(n, &x, &y);

    m = init_torus();

    for ( i = 0 ; i < y ; i++){
        insert_col_header(m);
    }

    puts("--------------------");

    build_links_for_dancing(m, set, x, y);

    puts("--------------------");
    _ans *O = (_ans*) malloc ( sizeof(_ans) );
    dancing_links(m, 0, O, n);
    puts("--------------------");

    return EXIT_SUCCESS;
}
