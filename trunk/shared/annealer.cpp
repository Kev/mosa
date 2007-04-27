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

#include "annealer.h"
#include "random.h"
#include "math.h"

Annealer::Annealer(double startingTemperature, double coolingFactor, uint coolingFrequency, Archive* arch,  QObject *parent) : QObject::QObject(parent)
{
  currentState = NULL;
  if (arch == NULL) {
    archive = new Archive(this);
  } else {
    archive=arch;
  }
  iteration = 0;
  temperature = startingTemperature;
  this->coolingFactor = coolingFactor;
  this->coolingFrequency = coolingFrequency;
  acceptances = 0;
}

Annealer::~Annealer()
{
  delete currentState;
}

bool Annealer::doIteration()
{
  //ASSERT( currentState!=NULL );
  ++iteration;
  //generate new state
  State* proposal = perturb(currentState);
  updateArchive(proposal);
  //get the standard (single-objective) measure of the energy
  double currentEnergy = energy(currentState);
  double proposedEnergy = energy(proposal);

  double acceptanceProbability = exp( -(proposedEnergy - currentEnergy) / temperature);

	//qDebug(QString("proposedEnergy %1 currentEnergy %2").arg(proposedEnergy).arg(currentEnergy));
  if (temperature == 0) {
    if (proposedEnergy > currentEnergy) {
		//	qDebug("Dominated check - setting probability to 0");
			acceptanceProbability = 0;
    } else {
			//qDebug("Dominated check - setting probability to 2");
      acceptanceProbability = 2;
    }
  }

  lastProb = acceptanceProbability;
  lastCurEnergy = currentEnergy;
  lastPropEnergy = proposedEnergy;

  double sample = Random::uniform();

  if (iteration % coolingFrequency == 0) {
    temperature *= coolingFactor;
  }
	//qDebug(QString("sample %1, probability %2").arg(sample).arg(acceptanceProbability));
  if (sample <= acceptanceProbability) {
		//qDebug("Accepting");
		delete currentState;
    currentState = proposal;
    lastAccepted = true;
    acceptances++;
    return true;
  }
	//qDebug("Rejecting");
  lastAccepted=false;
  delete proposal;
  return false;
}

int Annealer::getArchiveSize()
{
  return archive->size();
}

void Annealer::updateArchive(State* newState)
{
  Solution* newSolution;
  //archive any solutions introduced which are currently optimal
  
  int count=0;

  while ( (newSolution=newState->getNewSolution()) )
  {
    //ASSERT( newSolution!=NULL );
    archive->addSolution(*newSolution);
    count++;
  }
}

void Annealer::saveState(QString& fileNameStub)
{
  qWarning("Not implemented yet; Annealer::saveState state saving");
  QString stateFileName, paramFileName, objFileName;
  stateFileName = fileNameStub+"currentstate";
  paramFileName = fileNameStub+"archiveparameters";
  objFileName = fileNameStub+"archiveobjectives";
  archive->save(paramFileName,objFileName);
  currentState->saveState(stateFileName);
  qWarning(QString("Temp is currently %1").arg(temperature));
}

void Annealer::outputDebug()
{
  printf("Current Energy %g\tProposed Energy %g\t",lastCurEnergy,lastPropEnergy);
  printf("Acceptance Probability %g\t",lastProb);
  if (lastAccepted)
    printf("Accepted\n");
  else
    printf("Rejected\n");
  
}

uint Annealer::getAcceptanceCount()
{
  uint t=acceptances;
  acceptances=0;
  return t;
}
