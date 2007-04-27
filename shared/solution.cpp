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

#include "solution.h"
#include "random.h"

Solution::Solution() : QObject()
{
  numParameters=2;
  numObjectives=2;
  parameters=NULL;
  objectives=NULL;
  makeArrays();
  references=1;
}

Solution::Solution(Solution &original) : QObject()
{
  parameters=NULL;
  objectives=NULL;
  makeEqualTo(original);
  references=1;
}

Solution::Solution(uint numParameters, uint numObjectives) : QObject()
{
  //parameters.resize(numParameters, 0);
  //objectives.resize(numObjectives, 0);
  this->numParameters=numParameters;
  this->numObjectives=numObjectives;
  makeArrays();
  references=1;
}

Solution::~Solution()
{
  deleteArrays();
}

Solution* Solution::reference()
{
  references++;
  return this;
}

Solution* Solution::dereference()
{
  references--;
  if ( references>0 )
    return NULL;
  return this;
}

void Solution::makeArrays()
{
  parameters=new double[numParameters];
  objectives=new double[numObjectives];
}

void Solution::deleteArrays()
{
  delete[] parameters;
  parameters=NULL;
  delete[] objectives;
  objectives=NULL;
}

void Solution::randomise()
{
  for ( uint i=0; i<numParameters; ++i )
  {
    parameters[i]=Random::uniform();
  }
  for ( uint i=0; i<numObjectives; ++i )
  {
    objectives[i]=0;
  }
}


void Solution::operator=(Solution& other)
{
  makeEqualTo(other);
}

void Solution::makeEqualTo(Solution & other)
{
  numParameters=other.getNumParameters();
  numObjectives=other.getNumObjectives();
  if (parameters!=NULL)
    deleteArrays();
  makeArrays();
  //parameters.resize(numParameters, 0);
  //objectives.resize(numObjectives, 0);
  for ( uint i=0; i<getNumParameters(); ++i )
    setParameter( i, other.getParameter(i) );
  for ( uint i=0; i<getNumObjectives(); ++i )
    setObjective( i, other.getObjective(i) );
}

bool Solution::operator<(Solution &other)
{
  //ASSERT( getNumParameters() == other.getNumParameters() );
  //ASSERT( getNumObjectives() == other.getNumObjectives() );
  bool lessThan = false;
  for (uint i = 0; i < numObjectives; i++) {
    if (getObjective(i) < other.getObjective(i)) {
      lessThan = true;
    }
    if (getObjective(i) > other.getObjective(i)) {
      return false;
    }
  }
  return lessThan;
}

bool Solution::operator<=(Solution &other)
{
  //ASSERT( getNumParameters() == other.getNumParameters() );
  //ASSERT( getNumObjectives() == other.getNumObjectives() );
  for (uint i = 0; i < numObjectives; i++) {
    if (getObjective(i) > other.getObjective(i)) {
      return false;
    }
  }
  return true;
}
