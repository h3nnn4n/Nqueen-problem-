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
 *    in a product, an acknowledgment in the product documentation would be   * *    appreciated but is not required.                                        * * 2. Altered source versions must be plainly marked as such, and must not be * *    misrepresented as being the original software.                          *
 * 3. This notice may not be removed or altered from any source distribution. *
 ******************************************************************************/

#ifndef __DANCING_LINKS
#define __DANCING_LINKS

// If __USE_HEURISTIC is set an heristic will be used to select which column to explore first.
// Otherwise it will use the leftmost uncovered column.
// The heuristic consists of exploring first the node that lead to the fewest branchs.
// This process is explained on Knuth's paper at page 6.
//#define __USE_HEURISTIC
//#define __print_set

typedef struct __links {
    struct __links *L;
    struct __links *R;
    struct __links *U;
    struct __links *D;
    struct __links *C;

    int  n;
    int  size;
    char *name;
} _links;

typedef struct __ans {
    struct __ans   *next;
    struct __links *O;
} _ans;

// Initializes and empty structure
_links *init_torus();

// Inserts a column header into the Doubly linked circular list of columns
void    insert_col_header(_links *);

// Reads an matrix m and builds the structure that Knuth's described in his paper (Dancing Links)
void    build_links_for_dancing(_links *h, int **m, int x, int y);

// Knuth's dancing links(DLX) implementation. See Knuth's paper on page 5
void    dancing_links(_links *, int, _ans *, int n);

// Covers a column. This process is described on Knuth's paper page 6.
void    cover(_links *c);

// Uncovers a column. This process is described on Knuth's paper page 6.
void    uncover(_links *c);

#endif
