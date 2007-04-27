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

#include "dtlzsolver.h"
#include "dtlzjon.h"
#include "assert.h"
#define ASSERT assert

void DTLZSolver::evaluate(Solution* solution)
{
  uint numObjectives = solution->getNumObjectives();
  uint numParameters = solution->getNumParameters();
  double *parameters, *objectives;
  parameters = new double[numParameters];
  objectives = new double[numObjectives];
  
  for ( uint i=0; i<numParameters; i++ )
    parameters[i] = solution->getParameter(i);
  for ( uint i=0; i<numObjectives; i++ )
    objectives[i] = 0;
  
  decision_vector *dv = new decision_vector(parameters, objectives, numParameters, numObjectives);

  switch (problemNumber)
    {
    case 1: 
      DTLZ1(dv, numParameters, numObjectives, objectives); 
      break;
    case 2:
      DTLZ2(dv, numParameters, numObjectives, objectives);
      break;
    case 3:
      DTLZ3(dv, numParameters, numObjectives, objectives);
      break;
    case 4:
      DTLZ4a(dv, numParameters, numObjectives, objectives);
      break;
    case 5:
      DTLZ5(dv, numParameters, numObjectives, objectives);
      break;
    case 6:
      DTLZ7(dv, numParameters, numObjectives, objectives);
      break;
    case 7:
      DTLZ8vlv(dv, numParameters, numObjectives, objectives);
      break;
    default: 
      qFatal("The problem you've selected doesn't exist yet :D");
    }
      
  for ( uint i=0; i<numObjectives; i++ )
    solution->setObjective( i, objectives[i] );
  
    
  /* //we don't need this because jon's naughty and deletes them in dv
  delete[] parameters;
  delete[] objectives;
  */
  delete dv;
}

void DTLZSolver::setProblem(uint problemNumber)
{
  ASSERT( problemNumber<9 );
  this->problemNumber=problemNumber;
}

uint DTLZSolver::getSuggestedNumParams(uint numObjectives)
{
  switch (problemNumber)
    {
    case 1:
      return 5-1+numObjectives;
    case 2:
      return 5-1+numObjectives;
    case 3:   
      return 10+numObjectives-1;
    case 4:
      return 10+numObjectives-1;
    case 5:
      return 10+numObjectives-1;
    case 6:
      return 20+numObjectives-1;
    case 7:
      return 10*numObjectives;
    default: 
      qFatal("The problem you've selected doesn't exist yet :D");
  }
  return 0;
}
