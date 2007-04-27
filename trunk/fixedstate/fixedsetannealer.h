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


#ifndef FIXEDSETANNEALER_H
#define FIXEDSETANNEALER_H

#include "annealer.h"
#include "archive.h"
#include "solver.h"

/*!
 * FixedSetAnnealer class provides an annealer set up for set-based 
 * solutions with energy measured using the Dominance Measure
 */
class FixedSetAnnealer : public Annealer
{
  Q_OBJECT

public:
  /*!
   *Constructor, creates a dominance annealer with specified number of params and objectives, and a parent QObject as optionally specified. 
   * \param theProblem This is the Solver used to evaluate the multiple objectives of each solution.
   * \param setSize The number of solutions that should be generated for the initial set.
   * \param startingTemperature The starting temperature
   * \param coolingFrequency The temperature is reduced every coolingFrequency iterations
   * \param coolingFactor Every coolingFrequency iterations, temperature*=coolingFactor
   */
  FixedSetAnnealer( uint params, uint objectives, uint setSize, Solver* theProblem, double startingTemperature, double coolingFactor, uint coolingFrequency, QObject *parent = 0 );

protected:
  /*!
   * Calculates the energy of the State. In this case, it's the dominance
   * measure of the set
   */
  double energy(State*);

  /*!
   * Creates a new state that's a copy of the old state, and then perturbs
   * and returns it.
   */
  State* perturb(State* old);

private:
  Solver* problem;
  uint numParams;
  uint numObjectives;
};


#endif 
