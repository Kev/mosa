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

#include "archive.h"
#include <qfile.h>
#include <qtextstream.h>


Archive::Archive(QObject *parent) : QObject::QObject(parent)
{
  setIsPareto(true);
  archive.setAutoDelete(true); //no more worries about pointers ;)
  setIterator = new Q3PtrListIterator<Solution>(archive);
}

Archive::~Archive()
{
  delete setIterator;
}

void Archive::setIsPareto(bool pareto)
{
  this->pareto=pareto;
}

bool Archive::addSolution(Solution& solution)
{
  Solution* newSolution = new Solution(solution);
  if (pareto)
  {
    Q3PtrListIterator<Solution> it(archive);
    Solution* sol;
    while ( (sol = it.current()) != 0 )
    {
      if ( (*sol) < (*newSolution) )
      {
	return false;
      }

      if ( (*newSolution) < (*sol) )
      {
	archive.remove(sol);
      }
      else
	++it;
    }
  }

  archive.append(newSolution);
  return true;
}

bool Archive::dominates(Solution& solution)
{
  Q3PtrListIterator<Solution> it(archive);
  Solution* sol;
  while ( (sol = it.current()) != 0 )
  {
    ++it;
    if ( (*sol) < solution )
    {
      return true;
    }
  }
  return false;
}

void Archive::save(QString& paramFileName, QString& objFileName)
{
  QFile paramFile(paramFileName);
  QFile objFile(objFileName);
  if ( !paramFile.open( QIODevice::WriteOnly ) )
    qFatal(QString("Could not open file %1 for writing").arg(paramFileName));
  if ( !objFile.open( QIODevice::WriteOnly ) )
    qFatal(QString("Could not open file %1 for writing").arg(objFileName));
  QTextStream params(&paramFile);
  QTextStream obj(&objFile);

  Q3PtrListIterator<Solution> it(archive);
  Solution* sol;
  while ( (sol = it.current()) != 0 )
  {
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

int Archive::size()
{
  return archive.count();
}

void Archive::resetGetSolutionList()
{
  setIterator->toFirst();
}

Solution* Archive::getNextSolution()
{
  Solution* solution=setIterator->current();
  setIterator->operator++();
  return solution;
}
