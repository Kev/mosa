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

#ifndef STATE_H
#define STATE_H

#include "solution.h"
#include <qobject.h>

class State
{

public:
  
  //State();
  
  virtual ~State(){ };

  /*!
   * Returns Solutions in the new solution list. Once they've been
   * returned by this method, they're removed from the list.
   * Will return NULL if there are no remaining Solutions to return.
   */
  virtual Solution* getNewSolution()=0;

  /*!
   * Performs a perturbation of the state. The exact manner of 
   * perturbation should be set using other methods.
   */
  virtual void perturb()=0;

  /*!
   * Saves the current state (both objectives and parameters) to files
   * \param fileNameStub is the stub part of the filename
   */
  virtual void saveState(QString& fileNameStub)=0;
};

#endif
