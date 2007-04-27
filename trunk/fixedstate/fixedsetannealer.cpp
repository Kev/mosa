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

#include "fixedsetannealer.h"
#include "setstate.h"

FixedSetAnnealer::FixedSetAnnealer( uint params, uint objectives, uint setSize, Solver* theProblem, double startingTemperature, double coolingFactor, uint coolingFrequency, QObject* parent) : Annealer::Annealer(startingTemperature, coolingFactor, coolingFrequency, parent)
{
  numParams=params;
  numObjectives=objectives;
  problem=theProblem;
  currentState = new SetState(params, objectives, setSize, problem);

}

double FixedSetAnnealer::energy(State* state)
{
  uint dominatedCount=0;
  uint totalCount=((SetState*)state)->getSize();
  ASSERT ( totalCount > 0 );
  Solution *solution;
  ((SetState*)state)->resetGetSolutionList();
  while ( ( solution=((SetState*)state)->getNextSolution() ) )
  {
    if ( archive->dominates( *solution ) )
    {
      ++dominatedCount;
    }
  }
  return dominatedCount / totalCount;
}

State* FixedSetAnnealer::perturb(State* old)
{
  SetState* newState = new SetState(*(SetState*)old);
  newState->perturb();
  return newState;
}

