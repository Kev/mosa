/*
 * Copyright (C) 2005-2007 Kevin Smith
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307
USA
 *
 */

#ifndef SOLUTION_H
#define SOLUTION_H

#include <qobject.h>
#include <q3valuevector.h>

class Solution : public QObject
{
 Q_OBJECT
 public:
  
  /*!
   * Default Constructor. You probably don't want this one
   */
  Solution();
  
  /*!
   * Copy Constructor
   */
  Solution(Solution &original);

  /*!
   * Sets the number of parameters and objectives accordingly
   */
  Solution(uint numParameters, uint numObjectives);

  /*!
   * Now has to worry about the arrays
   */
  ~Solution();

  /*!
   * this enables us to do reference counting. Please call this if you're
   * referencing an *already existing* object. For new objects please use the
   * constructors as usual
   */
  Solution* reference();

  /*!
   * This enables us to safely delete the object. When you want to delete 
   * the object pointed to by Solution *point, do the following.
   *   delete point->dereference();
   * the method will return NULL if there are still references, making the 
   * delete do nothing.
   */
  Solution* dereference();

  /*!
   * Sets all the parameters to random values in the range [0:1]
   */
  void randomise();

  /*!
   * Returns the number of Parameters
   */
  inline uint getNumParameters() { return numParameters; }

  /*!
   * Returns the number of objectives
   */
  inline uint getNumObjectives() { return numObjectives; }
    
  /*!
   * Returns the index-th parameter (0-based)
   */
  inline double getParameter(uint index) { return parameters[index]; }

  /*!
   * Returns the index-th objective (0-based)
   */
  inline double getObjective(uint index) { return objectives[index]; }

  /*!
   * Sets the index-th parameter (0-based) to value
   */
  inline void setParameter(uint index, double value) { parameters[index]=value; }

  /*!
   * Sets the index-th objective (0-based) to value
   */
  inline void setObjective(uint index, double value) { objectives[index]=value; }

  /*!
   * Copies the other Solution
   */
  void operator=(Solution &other);

  /*!
   * Usual meaning of dominance. a<b iff a[i]<=b[i] for all i 
   * and a[i]<b[i] for at least one.
   */
  bool operator<(Solution &other);

  /*!
   * Weakly dominates. a<=b iff a[i]<=b[i] for all i
   */
  bool operator<=(Solution &other);

 private:
  uint numParameters, numObjectives;
  double *parameters;
  double *objectives;
  //QValueVector<double> parameters;
  //QValueVector<double> objectives;
  uint references;

  void makeEqualTo(Solution& other);
  void makeArrays();
  void deleteArrays();
};

#endif
