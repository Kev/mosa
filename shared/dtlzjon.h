/*
 * dtlzjon - Implementation of the DTLZ problems.
 * Copyright (C) 2003-2007  Jonathan Fieldsend
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef DTLZ_TEST_FUNCTIONS_H
#define DTLZ_TEST_FUNCTIONS_H

#include "jefsupport.h"

void DTLZ1(decision_vector *pop,int p_l,int l_f, double *fitness);
void DTLZ2(decision_vector *pop,int p_l,int l_f, double *fitness);
void DTLZ3(decision_vector *pop,int p_l,int l_f, double *fitness);
void DTLZ4(decision_vector *pop,int p_l,int l_f,double alpha, double *fitness);
void DTLZ4a(decision_vector *pop,int p_l,int l_f, double *fitness);
void DTLZ5(decision_vector *pop,int p_l,int l_f, double *fitness);
void DTLZ6(decision_vector *pop,int p_l,int l_f, double *fitness);
void DTLZ7(decision_vector *pop,int p_l,int l_f, double *fitness);
void DTLZ8(decision_vector *pop,int p_l,int l_f, double *fitness);
void DTLZ81xtra(decision_vector *pop,int p_l,int l_f, double *fitness);
void DTLZ8vlv(decision_vector *pop,int p_l,int l_f, double *fitness);
void DTLZ9(decision_vector *pop,int p_l,int l_f, double *fitness);


#endif
