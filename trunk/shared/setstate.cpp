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

#include "setstate.h"
#include "random.h"
#include <math.h>
#include <qfile.h>
#include <qtextstream.h>

void SetState::constructor(uint params, uint objectives, Solver* problem, bool pqrs)
{
  //set.setAutoDelete(true);
  newSolution = NULL;
  this->params=params;
  this->objectives=objectives;
  this->problem=problem;
	this->pqrs=pqrs;
  setPerturbationType ( BasicPerturbation );
  //setIterator = new QPtrListIterator<Solution>(set);
  resetGetSolutionList();
  //allocSet(0);
  size=0;
  iterator=0;
  lastEnergy=0;
  min = new double[objectives];
  max = new double[objectives];
  for (uint i=0; i<objectives; ++i)
    {
      max[i]=0-RAND_MAX;
      min[i]=RAND_MAX;
    }
}

SetState::SetState(uint params, uint objectives, uint setSize, Solver* problem, bool pqrs)
{
  constructor(params, objectives, problem, pqrs);
  //create the initial set
  allocSet(setSize);
  size=setSize;
  for (uint i=0; i< setSize; ++i)
  {
    Solution* sol = new Solution(params, objectives);
    sol->randomise();
    problem->evaluate(sol);
    checkMinMax(sol);
    set[i]=sol;
    /*newSolutions[newSize]=sol;
      newSize++;*/
  }
  
}

void SetState::checkMinMax(Solution* sol)
{
  for (uint i=0; i<objectives; ++i)
    {
      if ( sol->getObjective(i) < min[i] )
	min[i] = sol->getObjective(i);
      if ( sol->getObjective(i) > max[i] )
	max[i] = sol->getObjective(i);
    }
}

void SetState::allocSet(uint size)
{
  //this should always be created larger than needed, so that an extra
  // (perturbed) solution can be added
  set = new Solution*[size+10];
  maxSize=size+10;
}

void SetState::growSet()
{
  Solution** old=set;
  allocSet(size);
  for (uint i=0; i<size; ++i)
    set[i]=old[i];
  delete[] old;
}

SetState::SetState(SetState& original) : State()
{
  params = original.getNumParams();
  objectives = original.getNumObjectives();
  problem = original.getProblem();
  bool pqrs = original.pqrsEnabled();
	
  constructor(params,objectives,problem, pqrs);

  setPerturbationType( original.getPerturbationType() );

  original.resetGetSolutionList();
	
  size=0;
  allocSet(original.getSize());

  Solution* solution;
  while ( ( solution = original.getNextSolution() ) )
  {
    Solution* newSolution = solution->reference();
    set[size]=newSolution;
    size++;
  }
  
  for (uint i=0;i<objectives;++i)
    {
      min[i]=original.getMin(i);
      max[i]=original.getMax(i);
    }

}

bool SetState::pqrsEnabled()
{
	return pqrs;
}

double SetState::getMin(int obj)
{
  return min[obj];
}

double SetState::getMax(int obj)
{
  return max[obj];
}

SetState::~SetState()
{
  for ( uint i=0; i<size; ++i )
    {
      delete set[i]->dereference();
    }
  delete[] set;
  delete[] min;
  delete[] max;
}

uint SetState::getNumParams()
{
  return params;
}

uint SetState::getNumObjectives()
{
  return objectives;
}

Solver* SetState::getProblem()
{
  return problem;
}

SetState::PerturbationType SetState::getPerturbationType()
{
  return perturbType;
}

void SetState::setPerturbationType(SetState::PerturbationType t)
{
  perturbType=t;
}

Solution* SetState::getNewSolution()
{
  Solution* item = newSolution;
  newSolution=NULL;
  return item;
}

uint SetState::getSize()
{
  return size;
}

void SetState::resetGetSolutionList()
{
  iterator=0;
}

Solution* SetState::getNextSolution()
{
  if ( iterator>=size )
    return 0;
  Solution* solution=set[iterator];
  iterator++;
  return solution;
}

void SetState::perturb()
{
  if ( perturbType == BasicPerturbation )
    basicPerturbation();
  else if ( perturbType == DominatedRemovalBasic )
    basicDominatedPerturbation();
  else
    qFatal("Invalid Perturbation Type Set");
}

void SetState::basicPerturbation()
{
  fprintf(stderr,"Warning: doesn't set min/max\n");
  float pertProbability=0.2;
  uint it=0;
  while ( it < size )
  {
    if ( Random::uniform() < pertProbability )
    {
      int param = Random::integer( 0,set[it]->getNumParameters() );
      bool done = false;
      while( !done )
      {
	double pert = log( Random::uniform() );
	pert*=0.1;
	if ( Random::uniform() < 0.5 )
	  pert*=-1.0;
	double newVal = set[it]->getParameter( param );
	newVal += pert;
	if ( 0.0 <= newVal && newVal <= 1.0 )
	  {
	    set[it]->setParameter( param, newVal );
	    problem->evaluate( set[it] );
	    done=true;
	  }
      }
      newSolution=set[it];
    }
    ++it;
  }
}

void SetState::basicDominatedPerturbation()
{
  //We select a single solution, and perturb it.
  //We then remove any solutions from the set that it dominates,
  //or that dominate it.
  //We then add it.

  //as we could potentially grow larger than the array, check for that.
  if (maxSize<=size)
    growSet();

  Solution* solution;
  

  
  uint toPerturb = Random::integer( 0, size );
  int param = Random::integer( 0,params ); 
	if (pqrs)
	{
  	int obj = Random::integer( 0,objectives);

	  double val = min[obj]+ (Random::uniform() * (max[obj]-min[obj]));

	  toPerturb=0;
  	double closest=set[0]->getObjective(obj);
  	double mindiff=closest-val;
	  if ( mindiff < 0 )
	    mindiff=0-mindiff;

	  for (uint i=0; i<getSize(); ++i)
	    {
	      double diff=(set[i]->getObjective(obj)-val);
	      if (diff < 0)
				diff = 0-diff;
	      if (diff < mindiff)
				{
				  mindiff=diff;
				  closest=set[i]->getObjective(obj);
				  toPerturb=i;
				}
		}
	}
  
  //printf("Looking for value %g in range %g %g, closest is %g\n",val,min[obj],max[obj],closest);

  uint it;
  it=0;
  it += toPerturb;
  solution=new Solution(*set[it]);

  //first make the perturbations

  bool done = false;
  while( !done )
  {
	  double pert = log( Random::uniform() );
	  pert*=0.1;
	  if ( Random::uniform() < 0.5 )
		  pert*=-1.0;
	  double newVal = solution->getParameter( param );
	  newVal += pert;
	  if ( 0.0 <= newVal && newVal <= 1.0 )
	  {
		  solution->setParameter( param, newVal );
		  problem->evaluate( solution );
		  done=true;
	  }
  }

  //now remove any dominated or dominating solutions
  it=0;
  int count=0;
  uint total=getSize();
  
  //hack for minmax
  for (uint i=0; i<objectives; ++i)
    {
      double old=min[i];
      min[i]=max[i];
      max[i]=old;
    }
  
  while ( it < getSize() )
  {
	  bool del=false;
	  if ( count<=0 && *solution < *set[it] )
	    {
	      --count;
	      del=true;
	    }
	  if ( count>=0 && *set[it] < *solution )
	    {
	      ++count;
	      del=true;
	    }
	  if ( del )
	    {
	      delete set[it]->dereference();
	      set[it]=NULL;
	      ++it;
	      --size;
	    }
	  else
	    {
	      checkMinMax(set[it]);
	      ++it;
	    }
	  
  }
  
  compactSet();

  lastEnergy=((double)count)/total;

  //now add the perturbation to the set
  set[size]=solution;
  size++;
  newSolution=solution;
  
}

void SetState::compactSet()
{
  uint offset=0;
  for (uint i=0; i<size; ++i)
    {
      while ( set[i+offset]==NULL )
	{
	  ++offset;
	  
	}
      if ( offset>0 )
	{
	  set[i]=set[i+offset];
	}
    }
}

bool SetState::isDominatedBy(Solution& solution)
{
  uint it=0;
  Solution* sol;
  while ( it < size )
  {
    sol=set[it];
    ++it;
    if ( solution < (*sol) )
    {
      return true;
    }
  }
  return false;
}

double SetState::getEnergy()
{
  double e=lastEnergy;
  lastEnergy=0;
  return e;
}

void SetState::saveState(QString& fileNameStub)
{
  QString paramFileName, objFileName;
  paramFileName = fileNameStub+"parameters";
  objFileName = fileNameStub+"objectives";
  

  QFile paramFile(paramFileName);
  QFile objFile(objFileName);
  if ( !paramFile.open( QIODevice::WriteOnly ) )
    qFatal(QString("Could not open file %1 for writing").arg(paramFileName));
  if ( !objFile.open( QIODevice::WriteOnly ) )
    qFatal(QString("Could not open file %1 for writing").arg(objFileName));
  QTextStream params(&paramFile);
  QTextStream obj(&objFile);

  uint it=0;
  Solution* sol;
  while ( it<size )
  {
    sol=set[it];
    ++it;
    for ( uint i=0; i<sol->getNumParameters(); ++i)
    {
      params << sol->getParameter(i) << "\t";
    }
    params << "\n";

    for ( uint i=0; i<sol->getNumObjectives(); ++i)
    {
      obj << sol->getObjective(i) << "\t";
    }
    obj << "\n";

  }

  paramFile.close();
  objFile.close();
}
