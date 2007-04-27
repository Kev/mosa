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


#ifndef ARCHIVE_H
#define ARCHIVE_H

#include "solution.h"

#include <qobject.h>
#include <q3ptrlist.h>
/*!
 * The Archive class provides a container object for Solutions. Currently it
 * uses slow data structures. If one wished, they could extend the class to 
 * use Fieldsend/Everson's dominated trees instead.
 */
class Archive : public QObject
{
	Q_OBJECT
	public:
		/*!
		 *
		 * \param parent The parent object is said to 'own' this object. If the parent is deleted, so will this object be.
		 */
		Archive(QObject * parent = 0);

		~Archive();
		
		/*!
		 * Sets whether the archive should remove solutions once they are 
		 * dominated or not
		 */
		void setIsPareto(bool pareto);

		/*!
		 * Add the Solution to the archive. Note that the solution is copied first.
		 * Note that this will fail if the pareto 
		 * flag is true, and the Solution is dominated by the archive
		 */
		bool addSolution(Solution& solution);

		/*!
		 * Tests to see if any element of the archive dominates the solution
		 */
		bool dominates(Solution& solution);

		/*!
		 * This saves out the parameters and objectives for each 
		 * solution in the archive to the specified filenames.
		 */
		void save(QString& paramFileName, QString& objFileName);
		
		/*!
		 * Returns the size of the archive
		 */
		int size();

		 /*!
		  * Sets the internal iterator setIterator back to the start of the
		  * set list. Effectively resets the getNextsolution() method
		  */
		void resetGetSolutionList();
		
		/*!
		 * Returns solutions from the set, in order, returning NULL once the 
		 * iterator has reached the end. The iterator is reset to the start
		 * using resetGetSolutionList()
		 */
		Solution* getNextSolution();

	private:
		Q3PtrList<Solution> archive;
		/*!
		 * Flag to say if the archive must contain only mutually non-dominating 
		 * points. (Default: true)
		 */
		bool pareto;
		Q3PtrListIterator<Solution> *setIterator;
};

#endif
