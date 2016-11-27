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

unsigned long int branchs;
unsigned long int solutions_found;

static inline void cover(_links *c){
    _links *i, *j;
    c->R->L = c->L;                                          // Line 15
    c->L->R = c->R;                                          // Line 15

    for ( i = c->D ; i != c ; i = i->D ){                    // Line 16
        for ( j = i->R ; j != i ; j = j->R){                 // Line 17
            j->D->U = j->U;                                  // Line 18
            j->U->D = j->D;                                  // Line 18
            j->C->size -= 1;                                 // Line 19
        }
    }

    return;
}

static inline void uncover(_links *c){
    _links *i, *j;
    for ( i = c->U ; i != c ; i = i->U ) {        // Line 20
        for ( j = i->L ; j != i ; j = j->L ){     // Line 21
            j->C->size += 1;                      // Line 22
            j->D->U = j;                          // Line 23
            j->U->D = j;                          // Line 23
        }
    }

    c->R->L = c;                                  // Line 24
    c->L->R = c;                                  // Line 24

    return;
}

void dancing_links(_links *h, int k, _ans *ans, int n){
    _links *c;
    _links *j;
    _links *r;

    /*if ( k >= n ) abort();*/

    /*printf("%d %d\n", k, n);*/
    if ( k >= n ) {
        /*abort();*/
        /*_ans* a;*/
        /*for ( a = ans->next; a != ans && a != NULL; a = a->next ) {*/
            /*printf("%d ", a->O->n);*/
        /*}*/
        /*printf("\n -> %d\n\n", k);*/

        solutions_found++;
#ifdef __print_progress
        if ( solutions_found % 100000 == 0 ) {
            printf("Solved. Took %lu steps\n", branchs); // Line 1
            printf("Found %lu solutions\n", solutions_found);
            puts("");
        }
#endif
        /*puts("--------------------");*/
    }

    /*if ( h->R == h ) {                              // Line 1*/
        /*solutions_found++;*/
        /*printf("Solved. Took %lu steps\n", branchs); // Line 1*/
        /*printf("Found %lu solutions\n", solutions_found);*/
        /*int w;*/
        /*_ans *s;*/
        /*_links *p;*/
        /*for ( s = ans->next ; s->next != NULL ; s = s->next ){*/
            /*for (p = s->O, w = 0 ; (p != s->O || w == 0) && p->C->n > n ; p = p->R, w++ );*/
            /*c = p;*/
            /*for (w = 0 ; p != c || w == 0 ; p = p->R, w++ ){*/
                /*printf("%2.d ", p->C->n > n ? p->C->n - n : p->C->n);*/
            /*}*/
            /*puts("");*/
        /*}*/

        /*if ( s->next == NULL ){*/
            /*for (p = s->O, w = 0 ; (p != s->O || w == 0) && p->C->n > n ; p = p->R, w++ );*/
            /*c = p;*/
            /*for (w = 0 ; p != c || w == 0 ; p = p->R, w++ ){*/
                /*printf("%2.d ", p->C->n > n ? p->C->n - n : p->C->n);*/
            /*}*/
            /*puts("");*/
        /*}*/

        /*puts("--------------------");*/

        /*return;                                 // Line 1*/
    /*}                                           // Line 1*/

    c = h->R; // Chose a colum object           // Line 2

#ifdef __USE_HEURISTIC
    int s;
    s = c->size;
    for ( j = h->R ; j != h ; j = j->R ){       // Line 13
        if (j->size < s){                       // Line 14
            printf("%d\t%d\t%p\n", s, j->size, j);
            c = j;                              // Line 14
            s = j->size;                        // Line 14
        }                                       // Line 14
    }                                           // Line 13
#endif

    cover(c);                                   // Line 3

    _ans *tt = (_ans*) malloc ( sizeof(_ans) ); // Storing the anserws
    _ans *aux = ans;                            //
    while (aux->next != NULL) aux = aux->next;  //
    aux->next = tt;                             //
    tt ->next = NULL;                           //
    tt ->O    = NULL;                           //

    for ( r = c->D ; r != c ; r = r->D ){       // Line 4
        tt->O = r;                              // Line 5

        for( j = r->R ; j != r ; j = j->R){     // Line 6
            cover(j->C);                        // Line 7
        }

        branchs++;
        dancing_links(h, k + 1, ans, n);        // Line 8

        r = tt->O;                              // Line 9
        c =  r->C;                              // Line 9

        for (j = r->L ; j != r ; j = j->L){     // Line 10
            uncover(j->C);                      // Line 11
        }
    }

    uncover(c);                                 // Line 12

    aux = ans;
    while (aux->next->next != NULL && aux->next != NULL){
        aux = aux->next;
    }

    aux->next = NULL;
    free(tt);

#ifdef __print_result
    if ( k == 0 ) {
        if ( n % 2 == 0 ) solutions_found *= 2;
        printf("Solved. Took %lu steps\n", branchs); // Line 1
        printf("Found %lu solutions\n", solutions_found);
    }
#elif defined(__print_result_compact)
    if ( k == 0 ) {
        if ( n % 2 == 0 ) solutions_found *= 2;
        printf("%lu %lu\n", solutions_found, branchs); // Line 1
    }
#endif

    return;
}

_links *init_torus(){
    _links *p = (_links*) malloc ( sizeof(_links) );

    p->L    = p;
    p->R    = p;
    p->U    = p;
    p->D    = p;
    p->C    = p;
    p->size = 0;
    p->n    = 0;
    p->name = NULL;

    return p;
}

void build_links_for_dancing(_links *h, int **m, int x, int y){
    int i, j;
    _links *a;
    _links *first;
    _links *t;

    for ( j = 0 ; j < x ; j++ ){
        for ( i = 0, a = h->R, first = NULL ; i < y ; i++, a = a->R){
#ifdef __print_set
            printf("%d ", m[i][j]);
#endif
            if ( m[i][j] == 1){
                a->size++;
                for ( t = a->D; t != a; t = t->D );

                _links *new = (_links*) malloc ( sizeof(_links) );

                _links *tt = t->D;
                t->D   = new;
                new->D = tt;
                tt->U  = new;
                new->U = t;
                new->R = t;
                new->C = a;
                new->n = i;

                if ( first == NULL ){
                    new->R = new;
                    new->L = new;
                    first  = new;
                } else {
                    tt       = first->L;
                    first->L = new;
                    new->L   = tt;
                    tt->R    = new;
                    new->R   = first;
                }
            }
        }
#ifdef __print_set
        printf("\n");
#endif
    }

    return;
}

void insert_col_header(_links *h){
    _links *new = (_links*) malloc ( sizeof(_links) );
    _links *a;

    a         = h->L;
    h->L      = new;
    a->R      = new;
    new->L    = a;
    new->R    = h;
    new->D    = new;
    new->U    = new;
    new->C    = h;
    new->size = 0;
    new->n    = a->n + 1;
    new->name = NULL;

    return;
}
