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

#include "dominanceannealer.h"
#include "setstate.h"

DominanceAnnealer::DominanceAnnealer( uint params, uint objectives, uint setSize, Solver* theProblem, double startingTemperature, double coolingFactor, uint coolingFrequency, bool pqrs, Archive* archive, QObject* parent) : Annealer::Annealer(startingTemperature, coolingFactor, coolingFrequency, archive, parent)
{
  numParams=params;
  numObjectives=objectives;
  problem=theProblem;
  currentState = new SetState(params, objectives, setSize, problem, pqrs);
  static_cast<SetState*>(currentState)->setPerturbationType( SetState::DominatedRemovalBasic );
}

double DominanceAnnealer::energy(State* state)
{
  /*
  //this is the original, hideously broken version
  uint dominatedCount=0;
  uint totalCount=archive->size();
  ASSERT ( totalCount > 0 );
  Solution *solution;
  archive->resetGetSolutionList();
  while ( ( solution=(archive)->getNextSolution() ) )
  {
    if ( ((SetState*)state)->isDominatedBy( *solution ) )
    {
      ++dominatedCount;
    }
  }
  return dominatedCount / totalCount;
  */

  //this is the new version
  return ((SetState*)state)->getEnergy();
}

State* DominanceAnnealer::perturb(State* old)
{
  SetState* newState = new SetState(*(SetState*)old);
  newState->perturb();
  return newState;
}

int DominanceAnnealer::getStateSize()
{
  return ((SetState*)currentState)->getSize();
}
