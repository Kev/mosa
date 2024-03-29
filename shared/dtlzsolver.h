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

#ifndef DTLZSOLVER_H
#define DTLZSOLVER_H

#include "solver.h"
#include <qobject.h>

class DTLZSolver : public Solver
{
 Q_OBJECT
 public:
  void evaluate(Solution* solution);
  void setProblem(uint problemNumber);
  /*!
   * Returns the suggested number of parameters for a problem formulation
   * with the provided number of objectives
   */
  uint getSuggestedNumParams(uint numObjectives);
 private:
  uint problemNumber;
};

#endif
