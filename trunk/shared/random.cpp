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

#include "random.h"
#include <math.h>

double Random::uniform()
{
  //some silly code to get a random number, using QT
  static bool first_random = TRUE;
  if ( first_random ) {
    first_random = FALSE;
    QTime midnight( 0, 0, 0 );
    srand( midnight.secsTo(QTime::currentTime()) );
  }
  double sample=(1.0*rand())/RAND_MAX;
  //qWarning(QString("uniform random generated:%1").arg(sample));
  return sample;
}

double Random::gaussian()
{
  //some silly code to get a random number, using QT
  static bool first_random = TRUE;
  if ( first_random ) {
    first_random = FALSE;
    QTime midnight( 0, 0, 0 );
    srand( midnight.secsTo(QTime::currentTime()) );
  }
	double sample = sqrt(-2*log(uniform()))*2*cos(2*M_PI*uniform());

  return sample;
}

int Random::integer(int min, int max)
{
  //again with the silliness that allegedly sorts out the seed
/*  static bool first_random = TRUE;
  if ( first_random ) {
    first_random = FALSE;
    QTime midnight( 0, 0, 0 );
    srand( midnight.secsTo(QTime::currentTime()) );
  }*/
  double rand = uniform();
  int val = min+floor( rand*(max-min) );
  //qWarning(QString("integer random generated (%1<<%2):%3").arg(min).arg(max).arg(val));
  return val+min;

}
