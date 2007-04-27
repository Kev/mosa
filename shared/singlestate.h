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

#ifndef SINGLESTATE_H
#define SINGLESTATE_H

#include "state.h"
#include "archive.h"
#include "solution.h"
#include "solver.h"

#include <qobject.h>

class SingleState : public State
{
  
 public:
	enum PerturbationType { BasicPerturbation, VectorPerturbation };
  /*!
   * Copy constructor.
   */
  SingleState(SingleState&);

  /*!
   * Constructor which creates a solution
   * \param params Number of parameters for  solution
   * \param objectives Number of objectives for solution
   */
  SingleState(uint params, uint objectives, Solver* problem);

  /*!
   * Destructor
   */
  ~SingleState();

  /*!
   * Returns the number of parameters set
   */
  uint getNumParams();

  /*!
   * Returns the number of objectives set
   */
  uint getNumObjectives();

  /*!
   * Returns the problem Solver in use
   */
  Solver* getProblem();

  /*!
   * Returns the solution, then returns NULL. Is reset by a perturbation
   */
  Solution* getNewSolution();

   /*!
   * Performs a perturbation on the set, of the type set by the other.
   */
  void perturb();

   /*!
    * Checks if this->solution dominates other->solution
    */
  //bool dominates(SingleState*);

   /*!
    * Checks if other dominates this
    */
  //bool isDominatedBy(Solution);
   /*!
    * returns this->solution
    */
  Solution* getSolution() {return solution;}
  /*!
   * Sets the perturbation method for this state.
   */
  void setPerturbationType(PerturbationType t);

  /*!
   * Returns the enum of the PerturbationType in use. Necessary for the
   * copy constructor, but do not use yourself
   */
  PerturbationType getPerturbationType();
  
  void saveState(QString&);

 private:
  uint params;
  uint objectives;
  Solution* solution;
  Solution* newSolution;
  Solver* problem;
  PerturbationType perturbType;
  void basicPerturbation();
	void vectorPerturbation();
  void constructor(uint params, uint objectives, Solver* problem);
  bool used;
};

#endif
