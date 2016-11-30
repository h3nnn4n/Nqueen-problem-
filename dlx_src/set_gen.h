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

#ifndef SET_GEN_H
#define SET_GEN_H

int **gen_set          (int n, int *x , int *y                                                   ) ;
int **get_first_rows   (int n, int *xx, int *yy, int  n_fixed_rows,/*int *control,*/int *counter ) ;
void  get_size_reduced (int n, int *xx, int *yy, int  n_fixed_rows                               ) ;
void  get_size_full    (int n, int *xx, int *yy                                                  ) ;
void  update_counter   (int n, int n_fixed_rows, int *counter     , int *control                 ) ;

#endif /* SET_GEN_H */
