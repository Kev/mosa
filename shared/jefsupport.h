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

#ifndef JEFSUPPORT_H
#define JEFSUPPORT_H

class decision_vector{
  public :
    double *chromosome;
  double *objectives;
  int chrom_len;
  int obj_len;
  decision_vector();
  decision_vector(double *chrom,double *obj,int c_l,int o_l);
  ~decision_vector();
  void operator=(decision_vector & b);
  int operator<(decision_vector & b);
  int operator>(decision_vector & b);
  int operator<=(decision_vector & b);
  int operator>=(decision_vector & b);
  void refill_chromosome(double *refill);
  //void es_evolve(double m_p,double m_m,int z_o);
};

#endif
