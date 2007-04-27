/*
 * jefsupport - Support for Jon Fieldsend's problem class.
 * Copyright (C) 2005-2007  Kevin Smith
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

#include "jefsupport.h"

//----------------------------------------------------------------------
decision_vector::decision_vector()
  : chrom_len(2)
    ,obj_len(2)
{
  chromosome = new double[2];
  objectives = new double[2];
}
//----------------------------------------------------------------------
decision_vector::decision_vector(double *chrom,double *obj,int c_l, int o_l)
  : chrom_len(c_l)
    , obj_len(o_l)
{
  chromosome=chrom;
  objectives=obj;
}
//--------------------------------------------
decision_vector::~decision_vector()
{
  delete[] chromosome;
  delete[] objectives;
}
//--------------------------------------------
void decision_vector::operator=(decision_vector & b)
{
  int i;
  if (chrom_len!=b.chrom_len){
    delete[] chromosome;
    chrom_len=b.chrom_len;
    chromosome = new double[chrom_len];
  }
  if (obj_len!=b.obj_len){
    delete[] objectives;
    obj_len=b.obj_len;
    objectives = new double[obj_len];
  }
  for (i=0; i<chrom_len; i++)
    chromosome[i]=b.chromosome[i];
  for (i=0; i<obj_len; i++)
    objectives[i]=b.objectives[i];
}
//------------------------------------------------------------------------
int decision_vector::operator>(decision_vector & b)
{
  int i,dom=0;
  for (i=0; i<obj_len; i++){
    if (objectives[i]<b.objectives[i])
      return 0;
    else if (objectives[i]<b.objectives[i])
      dom++;
  }
  if (dom>0) return 1;
  else return 0;
}
//-----------------------------------------------------------------------------
int decision_vector::operator<(decision_vector & b)
{
  int i,dom=0;
  for (i=0; i<obj_len; i++){
    if (objectives[i]>b.objectives[i])
      return 0;
    else if (objectives[i]<b.objectives[i])
      dom++;
  }
  if (dom>0) return 1;
  else return 0;
}

//-----------------------------------------------------------------------------
int decision_vector::operator<=(decision_vector & b)
{
  int i;
  for (i=0; i<obj_len; i++)
    if (objectives[i]>b.objectives[i])
      return 0;
  return 1;
}
//-----------------------------------------------------------------------------
int decision_vector::operator>=(decision_vector & b)
{
  int i;
  for (i=0; i<obj_len; i++)
    if (objectives[i]<b.objectives[i])
      return 0;
  return 1;
}
//--------------------------------------------
void decision_vector::refill_chromosome(double *refill)
{
  for (int i=0; i<chrom_len; i++)
    chromosome[i]=refill[i];
}
//--------------------------------------------
/*void decision_vector::es_evolve(double mutatation_prob, double mutation_multiplier,int zero_one)
{
  int i,j;
  double r,v;
  if (zero_one){
    for (i=0; i<chrom_len; i++){
      r=UL();
      if (r<mutatation_prob){
        j=1;
        while(j){
          v=chromosome[i]+ mutation_multiplier*NL();
          if ((v>=0)&&(v<=1)){
            chromosome[i]=v;
            j=0;
          }
        }
      }
    }
  }
  else {
    for (i=0; i<chrom_len; i++){
      r=UL();
      if (r<mutatation_prob){
        j=1;
        chromosome[i]+=mutation_multiplier*NL();
      }
    }
  }
  }*/
//--------------------------------------------
