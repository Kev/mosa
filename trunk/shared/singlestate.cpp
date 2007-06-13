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

#include "singlestate.h"
#include "random.h"
#include <math.h>
#include <qfile.h>
#include <qtextstream.h>

void SingleState::constructor(uint params, uint objectives, Solver* problem)
{
  newSolution = NULL;
  this->params=params;
  this->objectives=objectives;
  this->problem=problem;
  //setPerturbationType ( BasicPerturbation );
  used=false;
}

SingleState::SingleState(uint params, uint objectives, Solver* problem)
{
  constructor(params, objectives, problem);
  //create the initial set
  solution = new Solution(params, objectives);
  solution->randomise();
  problem->evaluate(solution);
}

SingleState::SingleState(SingleState& original)
{
  params = original.getNumParams();
  objectives = original.getNumObjectives();
  problem = original.getProblem();
	
  constructor(params,objectives,problem);
	setPerturbationType(original.getPerturbationType());
  solution = new Solution(*original.getSolution());
}


SingleState::~SingleState()
{
  delete solution->dereference();
}

uint SingleState::getNumParams()
{
  return params;
}

uint SingleState::getNumObjectives()
{
  return objectives;
}

Solver* SingleState::getProblem()
{
  return problem;
}

SingleState::PerturbationType SingleState::getPerturbationType()
{
  return perturbType;
}

void SingleState::setPerturbationType(SingleState::PerturbationType t)
{
  perturbType=t;
}

Solution* SingleState::getNewSolution()
{
	if (used)
	{
		used=false;
		return NULL;
	}
	used=true;
	return solution;
}


void SingleState::perturb()
{
	switch (perturbType) {
		case BasicPerturbation: basicPerturbation(); break;
		case VectorPerturbation: vectorPerturbation(); break;
		default: fprintf(stderr, "Illegal perturbation method in use\n");
	}
	
	used=false;
}

void SingleState::basicPerturbation()
{
	int param = Random::integer( 0,solution->getNumParameters() );
	bool done = false;
	while( !done ) {
		double perturbation = log( Random::uniform() );
		perturbation *= 0.1;
		if ( Random::uniform() < 0.5 ) {
			perturbation *= -1.0;
		}
		double newVal = solution->getParameter( param ) + perturbation;
		if ( 0.0 <= newVal && newVal <= 1.0 ) {
			solution->setParameter( param, newVal );
			problem->evaluate( solution );
			done=true;
		}
	}
}

void SingleState::vectorPerturbation()
{
	/*
	 L = 0.0
	for j = 0:N
	    x[j] = randn()   # single Gaussian distributed number
	    L += x[j]*x[j]   # accumulate the length
	end

	L = sqrt(L)
	for j = 0:N
	    x[j] /= L        # normalise
	end
	*/
	double perturbations[solution->getNumParameters()];
	
	bool done = false;
	while( !done ) {
		double vectorlength = 0;
		for (uint i = 0; i < solution->getNumParameters(); i++) {
			perturbations[i] = Random::gaussian();
			vectorlength += perturbations[i] * perturbations[i];
		}
		vectorlength = sqrt(vectorlength);
		done = true;
		double perturbationlength = log( Random::uniform() );
		perturbationlength *= 0.5;
		if ( Random::uniform() < 0.5 ) {
			perturbationlength *= -1;
		}
		for (uint i = 0; i < solution->getNumParameters(); i++) {
			perturbations[i] *= perturbationlength;
			perturbations[i] /= vectorlength;
			double newVal = solution->getParameter(i) + perturbations[i];
			if ( 0.0 > newVal || newVal > 1.0 ) {
				done = false;
			}
		}		
	}
	for (uint i = 0; i < solution->getNumParameters(); i++) {
		solution->setParameter(i, solution->getParameter(i) + perturbations[i]);
	}
	problem->evaluate( solution );
}

/*bool SingleState::dominates(SingleState* other)
{
	return ( (*solution) < ( *(other->getSolution()) ) );
}

bool SingleState::isDominatedBy(Solution other)
{
	return ( (*solution) < other );
}*/

void SingleState::saveState(QString& fileNameStub)
{
	/*  QString paramFileName, objFileName;
  paramFileName = fileNameStub+"parameters";
  objFileName = fileNameStub+"objectives";
  

  QFile paramFile(paramFileName);
  QFile objFile(objFileName);
  if ( !paramFile.open( IO_WriteOnly ) )
    qFatal(QString("Could not open file %1 for writing").arg(paramFileName));
  if ( !objFile.open( IO_WriteOnly ) )
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
	*/
}
