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

#include <qobject.h>

#include "annealer.h"
#include "mosaannealer.h"
#include "dtlzsolver.h"
#include "math.h"
//#include "assert.h"
//#define ASSERT assert
#ifdef Q_WS_WIN
#include "XGetopt.h"
#endif 

/*! \mainpage SAMOSA
 *
 * \section intro_sec Introduction
 *
 * This is the set based multiobjective simulated annealer.
 *
 * \section install_sec Installation
 *
 * \subsection step1 Step 1: get code
 * \subsection step2 Step 2: compile
 *  
 * etc...
 */

/*!
 * Output the available command-line options
 */
void printOptions(char* name)
{
        fprintf(stderr,"Usage: %s [OPTION]\n",name);
        fprintf(stderr,"Run the set based multi objective simulated annealer.\n\n");
        fprintf(stderr,"        -d      --              dimensionality of objective space\n");
        fprintf(stderr,"        -i      --              number of evaluations\n");
        fprintf(stderr,"        -p      --              problem number\n");
        fprintf(stderr,"        -b      --              Minimum number of burn-in iterations (0). If non-zero, more may be used.\n");

        fprintf(stderr,"        -s      --              output text file label\n");
        fprintf(stderr,"        -g      --              Time to greedy\n");
        fprintf(stderr,"        -S      --              Set number of attainment surface samples\n");
        fprintf(stderr,"        -v      --              Debug output\n");
				fprintf(stderr,"        -P      --              Vector perturbation mode\n");
        fprintf(stderr,"        -O      --              full output mode, every iteration\n");
        fprintf(stderr,"        -h      --help                  display this help and exit\n");
        fprintf(stderr,"        -V      --version               output version information and exit\n");
        exit(0);
}

/*!
 * Outputs the program version and quits
 */
void printVersion()
{
  qDebug(" MOSA Version 0.1");
}

/*!
 * This is a complement to Annealer::saveState. This saves out an xml 
 * file with all the configuration details of this run, including the date
 * and program version.
 */
void saveConfig(QString& fileNameStub)
{
  qWarning("saveConfig not yet implemented");
}

/*!
 * You should know what this method does!
 */

int main(int argc, char *argv[])
{
  //Ok, let's define our variables
  uint numObjectives = 0;
  uint numParameters = 0;
  uint problemNumber = 0;
  uint iterations = 0;
	uint burninLength = 0;
  bool iterationOutput = false;
  QString outputLabel = "output";
  double startingTemperature = 4;
  double coolingFactor = 0;
  uint coolingFrequency = 100;
  double lowTemperature = 0.00001;
  uint lowTemperatureAfter = 10000;
  bool increasedOutput = false;
  uint attainmentSamples = 100;
  bool vectorPerturbation = false;
  //First off, let's parse the command-line arguments
  char* short_options="d:i:p:s:x:b:g:S:GOPVvh";
  int option_char;
  while ((option_char = getopt (argc, argv, short_options)) != -1)
  {
    switch (option_char)
      {
      case 'd': numObjectives = atoi(optarg);break;
      case 'i': iterations = atoi(optarg);break;
      case 'p': problemNumber = atoi(optarg);break;
      case 'O': iterationOutput = true;break;
      case 's': outputLabel = optarg;break;
      case 'g': lowTemperatureAfter = atoi(optarg);break;
      case 'b': burninLength = atoi(optarg);break;
      case 'S': attainmentSamples = atoi(optarg);break;
      case 'P': vectorPerturbation = true;break;
      case 'v': increasedOutput = true;break;
      case 'V': printVersion();exit(0);
      case 'h': printOptions(argv[0]);
      case '?': printOptions(argv[0]);
      }
  }
  
  if (coolingFactor == 0)
  {
    qDebug("Setting coolingFactor automagically");
    coolingFactor=exp( ( log(lowTemperature) - log(startingTemperature) ) 
            / 
            ( lowTemperatureAfter / coolingFrequency ) );
  }
  qDebug(QString("coolingFactor is %1").arg(coolingFactor));
  

  if (numObjectives == 0 || iterations == 0 || problemNumber == 0) {
    printOptions(argv[0]);
  }

  DTLZSolver* problem = new DTLZSolver();
  problem->setProblem(problemNumber);
  numParameters = problem->getSuggestedNumParams(numObjectives);

  //let's test everything's set correctly
  ASSERT( numObjectives > 0);
  ASSERT( numParameters > 0);

  MosaAnnealer *sa = new MosaAnnealer(numParameters, numObjectives, problem, startingTemperature, coolingFactor, coolingFrequency, attainmentSamples, vectorPerturbation);
  if (burninLength > 0) {
    iterations -= sa->doBurninIteration(burninLength);
  }
  for (uint i = 0; i < iterations; ++i) {
    sa->doIteration();
    if (increasedOutput || i % 1000 == 0) {
      printf("Iteration %d, archive size %d, state size %d, acceptances %d\n",i,sa->getArchiveSize(),sa->getStateSize(), sa->getAcceptanceCount());
    }
    if (i % 1000 == 0){
      QString genLabel=QString("%1-generation-%2").arg(outputLabel).arg(i);
      sa->saveState(genLabel);
    }
    if (increasedOutput) {
      sa->outputDebug();
    }
  }
  
  saveConfig(outputLabel);
  sa->saveState(outputLabel);
  delete sa;
  delete problem;
}
