/*
 * test.cpp - Assorted diagnostic outputs.
 * Copyright (C) 2007  Kevin Smith
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "solution.h"
#include "setstate.h"
#include <qobject.h>
#include "dtlzsolver.h"
#include "random.h"

int main(int argc, char *argv[])
{
  Solution *num1, *num2;
  SetState *set1, *set2;

  num1=new Solution(1,2);
  
  printf("Testing solution copying/deleting\n");

  for (uint i=0;i<10;++i)
    {
      printf("%d\n",i);
      num2 = new Solution(*num1);
      delete num1;
      num1=num2;
    }
  
  DTLZSolver* problem = new DTLZSolver();
  problem->setProblem(1);
  uint p = problem->getSuggestedNumParams(3);

  printf("Testing set copying/deleting\n");
  set1 = new SetState(p,3,10, problem);
  for (uint i=0;i<10;++i)
    {
      printf("%d\n",i);
      set2 = new SetState(*set1);
      delete set1;
      set1=set2;
    }
  

  printf("Testing set mutation\n");
  SetState *set3;
  for (uint i=0;i<10;++i)
    {
      printf("%d\n",i);
      set2 = new SetState(*set1);
      set3 = new SetState(*set1);
      set2->perturb();
      delete set1;
      delete set3;
      set1=set2;
    }

  printf("Testing state casts\n");
  State *state1, *state2;
  state1=new SetState(p,3,10,problem);
  for (uint i=0;i<10;++i)
    {
      printf("%d\n",i);
      state2 = new SetState(*(SetState*)state1);
      state2->perturb();
      delete state1;
      state1=state2;
    }

  printf("Testing state getnews\n");
  Solution *newSolution;
  Archive *archive=new Archive(0);
  for (uint i=0;i<10;++i)
    {
      printf("%d\n",i);
      state2 = new SetState(*(SetState*)state1);
      state2->perturb();
      delete state1;
      state1=state2;

      while ( (newSolution=state1->getNewSolution()) )
	{
	  archive->addSolution(*newSolution);
	}
    }
  
  printf("Random int\n");
  for (uint i=0;i<10;++i)
    {
      printf("1 : %d\n",Random::integer(0,10));
    }

  printf("Random float\n");
  for (uint i=0;i<10;++i)
    {
      printf("1 : %g\n",Random::uniform());
    }
  
    printf("Random int\n");
  for (uint i=0;i<10;++i)
    {
      printf("1 : %d\n",Random::integer(0,10));
    }


}
