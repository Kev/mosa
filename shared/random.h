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

#include <qdatetime.h>


class Random
{
 public:
  /*!
   * Draws a double from a uniform distribution (0<=x<1)
   */
  static double uniform();
  /*!
   * Draws a double from a gaussian distribution (mean 0, sd 1)
   */
  static double gaussian();
  /*!
   * Draws an integer randomly such that min<=x<max
   */
  static int integer(int min, int max);

};
