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


#ifndef ANNEALER_H
#define ANNEALER_H

#include "state.h"
#include "solution.h"
#include "archive.h"
#include <qobject.h>

#ifndef ASSERT
#include "assert.h"
#define ASSERT assert
#endif

/*!
 * Annealer class provides an annealer frame which can be inherited
 * to provide problems/solutions/mutations
 */
class Annealer : public QObject
{
  Q_OBJECT

public:
        /*!
         * Abstract Annealer class. Decendants should define perturbation 
         * and evaluation functions for their chosen State.
	 * \param startingTemperature The starting temperature
	 * \param coolingFrequency The temperature is reduced every coolingFrequency iterations
	 * \param coolingFactor Every coolingFrequency iterations, temperature*=coolingFactor
        */
  Annealer(double startingTemperature, double coolingFactor, uint coolingFrequency, Archive* arch = 0,  QObject *parent = 0);

	/*!
	 * Destroy
	 */
	~Annealer();
	
	/*!
	 * Performs one iteration of the annealer.
	 * returns true if the current State is changed, false if it remains
	 * unchanged.
	 */
	virtual bool doIteration();

	/*!
	 * This uses the newSolutions method of States to update the 
	 * non-dominated Archive of Solutions from newState.
	 */
	virtual void updateArchive(State* newState);

	/*!
	 * Returns the number of solutions in the current archive
	 */
	virtual int getArchiveSize();

	/*!
	 * Saves the current state of the annealer into three files. 
	 * A representation of the current solution, a copy of the archive contents' parameters, and a copy of the archive contents' objectives.
	 * \param fileNameStub this is the base to be prepended to the saved filenames.
	 */
	void saveState(QString& fileNameStub);
	
	/*!
	 * This method outputs information useful for debugging the annealer
	 */
	virtual void outputDebug();

	/*!
	 * Returns the number of acceptances *since the last call*
	 */
	uint getAcceptanceCount();

protected:
	/*!
	 * This must calculate the single-objective representation of the
	 * energy of the passed solution. In anything but a trivial example
	 * there will probably also need to be multi-objective evaluations
	 * performed internally. You probably want to cache these, but it's 
	 * not enforced
	 */
	virtual double energy(State*) = 0;
	
	/*!
	 * Takes a State as a parameter, creates a copy of it, perturbs it,
	 * and returns the new State. The original State is untouched.
	 */
	virtual State* perturb(State* old) = 0;

	State* currentState;
	//State* proposedSolution; //moved this out to the iteration
	Archive* archive;
	uint iteration;
	double temperature;
	double coolingFactor;
	uint coolingFrequency;
	uint acceptances;

	//extra output vars
	double lastCurEnergy, lastPropEnergy;
	double lastProb;
	bool   lastAccepted;
};


#endif 
