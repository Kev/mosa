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

#include "mosaannealer.h"
#include "singlestate.h"
#include "random.h"
#include <math.h>

MosaAnnealer::MosaAnnealer( uint params, uint objectives, Solver* theProblem, double startingTemperature, double coolingFactor, uint coolingFrequency, uint attainmentSamples, bool vectorPerturbation, Archive* archive, QObject* parent) : Annealer::Annealer(startingTemperature, coolingFactor, coolingFrequency, archive, parent)
{
  numParams=params;
  numObjectives=objectives;
  problem=theProblem;
  currentState = new SingleState(params, objectives, problem);
  numSamples = attainmentSamples;
  static_cast<SingleState*>(currentState)->setPerturbationType( SingleState::BasicPerturbation );
	if (vectorPerturbation) {
		static_cast<SingleState*>(currentState)->setPerturbationType( SingleState::VectorPerturbation );
	}
}

double MosaAnnealer::energy(State* state)
{
	QString debugEncoding;
	//qDebug(QString("Calculating energy of state:%1").arg((int)state));
  //this is the original, hideously broken version
  double dominatedCount = 0;
  double totalCount = archive->size() + 2 + numSamples;
	//qDebug(QString("MosaAnnealer::energy - totalCount %1").arg(totalCount));
  ASSERT ( totalCount > 0 );
  Solution *solution;
  double mins[numObjectives];
  double maxs[numObjectives];
  //Archive
  archive->resetGetSolutionList();
  bool first = true;
	Solution* stateSolution = ((SingleState*)state)->getSolution();
	Solution* currentSolution = ((SingleState*)currentState)->getSolution();
	Solution* proposedSolution = ((SingleState*)proposedState)->getSolution();
	//for (uint i = 0; i < numObjectives; i++) {
	//	debugEncoding = QString("%1%2").arg(debugEncoding).arg(stateSolution->getObjective(i));
	//}
	//qDebug(QString("Encoding %1").arg(debugEncoding));
  while ((solution = (archive)->getNextSolution())) {
	  for (uint d = 0; d < numObjectives; d++) {
  	  if (first) {
		    mins[d] = solution->getObjective(d);
		    maxs[d] = solution->getObjective(d);
	    } else {
		    if (solution->getObjective(d) < mins[d]) {
			    mins[d] = solution->getObjective(d);
		    }
		    if (solution->getObjective(d) > maxs[d]) {
			    maxs[d] = solution->getObjective(d);
		    }
	    }
	  }
    if (*solution < *stateSolution) {
      dominatedCount++;
    }
		first = false;
  }
	// + current and proposed
  if (*currentSolution < *stateSolution) {
    dominatedCount++;
  }
  if (*proposedSolution < *stateSolution) {
    dominatedCount++;
  }
	//qDebug(QString("After secondary test, dominatedCount = %1").arg(dominatedCount));
  // + attainment sampling
  // I hate using single-character variables, but I'm sticking with the 
  // notation used in my thesis.
  uint chosenObjective = Random::integer(0,numObjectives);
  double smallestValue;
  for (uint i = 0; i < numSamples; i++) {
	  Solution v(0,numObjectives);
	  for (uint d = 0; d < numObjectives; d++) {
		  v.setObjective(d,Random::uniform()*(maxs[d]-mins[d]) + mins[d]);
	  }
	  archive->resetGetSolutionList();
	  bool set = false;
	  Solution* u;
	  while ( ( u=(archive)->getNextSolution() ) ) {
	  	if (!set || u->getObjective(chosenObjective) < smallestValue) {
		    v.setObjective(chosenObjective, u->getObjective(chosenObjective));
		    if ((*u) < v) {
			    set = true;
			    smallestValue = u->getObjective(chosenObjective);
		    }
	    }
	  }
	  if (v < *stateSolution) {
		  dominatedCount++;
	  }
  }
	//qDebug(QString("%1 / %2 = %3").arg(dominatedCount).arg(totalCount).arg(dominatedCount / totalCount));
  return dominatedCount / totalCount;
}

State* MosaAnnealer::perturb(State* old)
{
  SingleState* newState = new SingleState(*(SingleState*)old);
  newState->perturb();
  proposedState = newState;
  return newState;
}

int MosaAnnealer::getStateSize()
{
  return 1;
}

uint MosaAnnealer::doBurninIteration(uint minIterations)
{
	double energyChange = 0;
	double energyChangeSum = 0;
	int energyChangeCount = 0;
	int iterationsUsed = 0;
	temperature = 10020;
	while (temperature >= 10000){
		for (uint i = 0; i < minIterations; i++){
			iterationsUsed++;
			//fprintf(stderr,"in burnin %d %d %f\n",i,num_E,delta_E);
			doIteration();
			energyChange = 	lastPropEnergy - lastCurEnergy;
			if (energyChange < 0){
				energyChangeSum += energyChange;
				energyChangeCount++;
			}
		}
		if (energyChangeCount > 9){
			temperature = -2 * 0.5 * (energyChangeSum / energyChangeCount) / log(2.0);
		}
		//fprintf(stderr,"in while \n");
		if (temperature >= 10000){
			--temperature;
		}
	}
	qDebug(QString("After %1 burnin iterations, temperature is %1.").arg(iterationsUsed).arg(temperature));
	return iterationsUsed;
}
