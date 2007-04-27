/*
 * dtlzjon - Implementation of the DTLZ problems.
 * Copyright (C) 2003-2007  Jonathan Fieldsend
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

#include <math.h>
#include "jefsupport.h"
#include "dtlzjon.h"

//---------------------------
void DTLZ1(decision_vector *pop,int p_l,int l_f, double *fitness)
{
  //Page 16 
  //chromosome parameters p_l
  //number of objectives l_f
  int i,k,j;
  double gxM,pi=3.14159265358979;
  // 
  k=p_l-l_f+1; //recommended as 5 in DTLZ paper
  //calculate gxM - contained in the last k genes of the chromosome
  gxM=k;
  for (i=0; i<k; i++){
    gxM+=pow(pop->chromosome[i+l_f-1]-0.5,2)-cos(20.0*pi*(pop->chromosome[i+l_f-1]-0.5));
  }
  gxM=1+100.0*gxM;
  //calculate fitness - contained in first l_f-1 genes of the chromosme (and gxM)
  fitness[0]=0.5*gxM;
  for (i=0; i<l_f-1; i++){
    fitness[0]*=pop->chromosome[i];
  }
  for (i=1; i<l_f; i++){
    fitness[i]=0.5*gxM;
    for (j=0; j<l_f-i-1; j++)
      fitness[i]*=pop->chromosome[j];
    fitness[i]*=(1-pop->chromosome[j]);
  }
  //for (i=0; i<p_l; i++)
  //if ((pop->genome[i]>1.0)||(pop->genome[i]<0.0))
  //printf("\n %f %d %d %d\n",pop->genome[i],i,p_l,l_f);
  //return l_f objective levels 
  for (i=0; i<l_f; i++){
    //printf("%f  \t",fitness[i]);
    pop->objectives[i]=fitness[i];
  }
  //printf("%d %d\n",p_l,l_f);
  //delete[] fitness;
}
//---------------------------
void DTLZ2(decision_vector *pop,int p_l,int l_f, double *fitness)
{
  
//Page 17
  //chromosome parameters p_l
  //number of objectives l_f
  int i,k,j;
  double gxM,pi=3.14159265358979;
  
  k=p_l-l_f+1; //recommended as 5 in DTLZ paper
  //calculate gxM - contained in the last k genes of the chromosome
  gxM=0.0;
  for (i=0; i<k; i++){
    gxM+=pow(pop->chromosome[i+l_f-1]-0.5,2.0);
    //printf(" %d ",i+l_f-1);
  }
  gxM=1.0+gxM;
  //calculate fitness - contained in first l_f-1 genes of the chromosome (and gxM)
  fitness[0]=gxM;
  for (i=0; i<l_f-1; i++){
    fitness[0]*=cos(pop->chromosome[i]*pi/2.0);
  }
  for (i=1; i<l_f; i++){
    fitness[i]=gxM*sin(pop->chromosome[l_f-i-1]*pi/2.0);
    for (j=0; j<l_f-1-i; j++)
      fitness[i]*=cos(pop->chromosome[j]*pi/2.0);
  }
  //return l_f objective levels 
  //printf("\t %d %d\t",p_l,k);
  for (i=0; i<l_f; i++)
    pop->objectives[i]=fitness[i];
 
}
//---------------------------
void DTLZ3(decision_vector *pop,int p_l,int l_f, double *fitness)
{
//Page 18 -> (3^k -1) local Pareto fronts
  //chromosome parameters p_l
  //number of objectives l_f
  int i,k,j;
  double gxM,pi=3.14159265358979;
  k=p_l-l_f+1; //recommended as 10 in DTLZ paper
  //calculate gxM - contained in the last k genes of the chromosome
  gxM=k;
  for (i=0; i<k; i++){
    gxM+=pow(pop->chromosome[i+l_f-1]-0.5,2)-cos(20.0*pi*(pop->chromosome[i+l_f-1]-0.5));
  }
  gxM=1+100.0*gxM;
  //calculate fitness - contained in first l_f-1 genes of the chromosome (and gxM)
  fitness[0]=gxM;
  for (i=0; i<l_f-1; i++){
    fitness[0]*=cos(pop->chromosome[i]*pi/2.0);
  }
  for (i=1; i<l_f; i++){
    fitness[i]=gxM*sin(pop->chromosome[l_f-i-1]*pi/2.0);
    for (j=0; j<l_f-1-i; j++)
      fitness[i]*=cos(pop->chromosome[j]*pi/2.0);
  }

  //return l_f objective levels 
  for (i=0; i<l_f; i++)
    pop->objectives[i]=fitness[i];
}
//---------------------------
void DTLZ4(decision_vector *pop,int p_l,int l_f,double alpha, double *fitness)
{
  //Page 19 alpha suggested as 100
  //genome parameters p_l
  //number of objectives l_f
  int i,k,j;
  double gxM,pi=3.14159265358979;
   k=p_l-l_f+1; //recommended as 10 in DTLZ paper
  //calculate gxM - contained in the last k genes of the chromosome
  gxM=0.0;
  for (i=0; i<k; i++){
    gxM+=pow(pop->chromosome[i+l_f-1]-0.5,2);
  }
  gxM=1+gxM;
  //calculate fitness - contained in first l_f-1 genes of the chromosome (and gxM)
  fitness[0]=gxM;
  for (i=0; i<l_f-1; i++){
    fitness[0]*=cos(pow(pop->chromosome[i],alpha)*pi/2.0);
  }
  for (i=1; i<l_f; i++){
    fitness[i]=gxM*sin(pow(pop->chromosome[l_f-i-1],alpha)*pi/2.0);
    for (j=0; j<l_f-1-i; j++)
      fitness[i]*=cos(pow(pop->chromosome[j],alpha)*pi/2.0);
  }

  //return l_f objective levels 
  for (i=0; i<l_f; i++)
    pop->objectives[i]=fitness[i];
}
//---------------------------
void DTLZ4a(decision_vector *pop,int p_l,int l_f, double *fitness)
{
//Page 19 alpha suggested as 100 - exactlt the same as DLTZ4 - except alpha set inside
  //genome parameters p_l
  //number of objectives l_f
  int i,k,j;
  double gxM,pi=3.14159265358979,alpha=100.0;
	alpha=100;
  k=p_l-l_f+1; //recommended as 10 in DTLZ paper
  //calculate gxM - contained in the last k genes of the chromosome
  gxM=0.0;
  for (i=0; i<k; i++){
    gxM+=pow(pop->chromosome[i+l_f-1]-0.5,2.0);
  }
  gxM=1.0+gxM;
  //calculate fitness - contained in first l_f-1 genes of the chromosome (and gxM)
  fitness[0]=gxM;
  for (i=0; i<l_f-1; i++){
    fitness[0]*=cos(pow(pop->chromosome[i],alpha)*pi/2.0);
  }
  for (i=1; i<l_f; i++){
    fitness[i]=gxM*sin(pow(pop->chromosome[l_f-i-1],alpha)*pi/2.0);
    for (j=0; j<l_f-1-i; j++)
      fitness[i]*=cos(pow(pop->chromosome[j],alpha)*pi/2.0);
  }
  //return l_f objective levels 
  for (i=0; i<l_f; i++){
    pop->objectives[i]=fitness[i];
  }
}
//---------------------------
void DTLZ5(decision_vector *pop,int p_l,int l_f, double *fitness)
{
//Page 20 OK theta1 is undefined in deb so making best guess - so is r, likewise
  //NOT IN THE CONFERENCE PAPER
  //genome parameters p_l
  //number of objectives l_f
  int i,k,j;
  double *theta,gxM,gr,pi=3.14159265358979;
  theta = new double[l_f-1];
  k=p_l-l_f+1; //recommended as 10 in DTLZ paper
  //calculate gxM - contained in the last k genes of the chromosome
  gxM=0.0;
  for (i=0; i<k; i++){
    gxM+=pow(pop->chromosome[i+l_f-1]-0.5,2.0);
  }
  gr=gxM;
	gxM=1+gxM;
  fitness[0]=gxM;
  
  theta[0]=pop->chromosome[0];
  for (i=1; i<l_f-1; i++)
    theta[i]=(pi/(4.0*(1+gr)))*(1+2*gr*pop->chromosome[i]);

  for (i=0; i<l_f-1; i++){
    if (i>0)
      fitness[0]*=cos(theta[i]);
    else
      fitness[0]*=cos(theta[i]*pi/2.0);
  }
  for (i=1; i<l_f; i++){
    if (l_f-i-1>0)
      fitness[i]=gxM*sin(theta[l_f-i-1]);
    else
      fitness[i]=gxM*sin(theta[l_f-i-1]*pi/2.0);
    for (j=0; j<l_f-1-i; j++){
      if (j>0)
        fitness[i]*=cos(theta[j]);
      else
        fitness[i]*=cos(theta[j]*pi/2.0);
    }
  }

  //return l_f objective levels
  for (i=0; i<l_f; i++)
    pop->objectives[i]=fitness[i];
   delete[] theta;
}


void DTLZ6(decision_vector *pop,int p_l,int l_f, double *fitness)
{
  int i,k,j;
  double *theta,gxM,gr,pi=3.14159265358979;
  theta = new double[l_f-1];
  k=p_l-l_f+1; //recommended as 10 in DTLZ paper
  //calculate gxM - contained in the last k genes of the chromosome
  gxM=0.0;
  for (i=0; i<k; i++){
	  gxM+=pow(pop->chromosome[i+l_f-1],0.1);
	}
	
	gr=gxM;
	gxM=1+gxM;
  fitness[0]=gxM;
  
  theta[0]=pop->chromosome[0];
  for (i=1; i<l_f-1; i++)
    theta[i]=(pi/(4.0*(1+gr)))*(1+2*gr*pop->chromosome[i]);

  for (i=0; i<l_f-1; i++){
    if (i>0)
      fitness[0]*=cos(theta[i]);
    else
      fitness[0]*=cos(theta[i]*pi/2.0);
  }
  for (i=1; i<l_f; i++){
    if (l_f-i-1>0)
      fitness[i]=gxM*sin(theta[l_f-i-1]);
    else
      fitness[i]=gxM*sin(theta[l_f-i-1]*pi/2.0);
    for (j=0; j<l_f-1-i; j++){
      if (j>0)
        fitness[i]*=cos(theta[j]);
      else
        fitness[i]*=cos(theta[j]*pi/2.0);
    }
  }

  //return l_f objective levels
  for (i=0; i<l_f; i++)
    pop->objectives[i]=fitness[i];
   delete[] theta;
}

//---------------------------
void DTLZ7(decision_vector *pop,int p_l,int l_f, double *fitness)
{
//Page 22, has 2^l_f-1 disconnected regions
  //DTLZ6 in the conference paper
  //genome parameters p_l
  //number of objectives l_f
  int i,k;
  double gxM,h=0.0,pi=3.14159265358979;
  k=p_l-l_f+1; //recommended as 20 in DTLZ paper
  //calculate gxM - contained in the last k genes of the chromosome
  gxM=0.0;
  for (i=0; i<k; i++){
    gxM+=pop->chromosome[i+l_f-1];
  }
  gxM=1.0+(9.0/(double) k)*gxM;
  //calculate fitness - contained in first l_f-1 genes of the chromosome (and gxM)
  for (i=0; i<l_f-1; i++){
    fitness[i]=pop->chromosome[i];
  }
  for (i=0; i<l_f-1; i++){
    h+=(fitness[i]/(1.0+gxM)*(1.0+sin(3.0*pi*fitness[i])));
  }
  h=l_f-h;
  fitness[l_f-1]=(1.0+gxM)*h;
  //return l_f objective levels 
  for (i=0; i<l_f; i++)
    pop->objectives[i]=fitness[i];
  //printf("\t %d %d %d\t",p_l,l_f,k);
}

//---------------------------

void DTLZ8(decision_vector *pop,int p_l,int l_f, double *fitness)     
{                                                                               
//Page 23, constraint surface approach                                          
  //DTLZ7 in conference paper                                                   
  //genome parameters p_l                                                       
  //number of objectives l_f                                                    
  // p_l>l_f (suggested p_l=10*l_f)                                             
  //the last objective contains the constraint violation, therefore a           
  //l_f=4 objective problem, is actually a 3 objective one plus 1               
  //binary constraint violation measure                                         
                                                                                
  int i,j;                                                                      
  int a_f=l_f-1; //the *actual* number of objectives                            
  a_f=(int)(0.5*l_f);
  double min=10.0; //initialised above constraint   violation, it doesn't matter what it really starts at as long as its >=0        
  for (j=0; j<a_f; j++){                                                        
    fitness[j]=0.0;                                                             
    for (i=(int) ceil(j*(p_l/(double) a_f)); i<(int) ceil((j+1)*(p_l/(double)   
a_f))-1; i++)                                                                   
      fitness[j]+=pop->chromosome[i];                                           
    fitness[j]/=ceil(p_l/(double) a_f);                                         
  }                                                                             
  //now calculate constraints                                                   
  for (j=0; j<a_f-1; j++){                                                      
    if (fitness[a_f-1]+4.0*fitness[j]-1.0<0.0)
      fitness[a_f+j]=1;
    else
      fitness[a_f+j]=0;
  }                                                                             
  for (i=0; i<a_f-1; i++)                                                       
    for (j=i+1; j<a_f-1; j++)                                                   
      if(min>(fitness[i]+fitness[j]))                                           
        min=fitness[i]+fitness[j];                                              

  if (2.0*fitness[a_f-1]+min-1.0<0)                                             
    fitness[l_f-1]=1;                                                             
  else                                                                          
    fitness[l_f-1]=0;                                                             
                                                                                
  //return l_f objective levels                                                 
  for (i=0; i<l_f; i++)                                                         
    pop->objectives[i]=fitness[i];                                              
}

void DTLZ81xtra(decision_vector *pop,int p_l,int l_f, double *fitness)     
{                                                                               
//Page 23, constraint surface approach                                          
  //DTLZ7 in conference paper                                                   
  //genome parameters p_l                                                       
  //number of objectives l_f                                                    
  // p_l>l_f (suggested p_l=10*l_f)                                             
  //the last objective contains the constraint violation, therefore a           
  //l_f=4 objective problem, is actually a 3 objective one plus 1               
  //binary constraint violation measure                                         
                                                                                
  int i,j;                                                                      
  int a_f=l_f-1; //the *actual* number of objectives                            
  double min=10.0; //initialised above constraint   violation, it doesn't matter what it really starts at as long as its >=0        
  bool constraint_violation =false;                                             
  for (j=0; j<a_f; j++){                                                        
    fitness[j]=0.0;                                                             
    for (i=(int) ceil(j*(p_l/(double) a_f)); i<(int) ceil((j+1)*(p_l/(double)   
a_f))-1; i++)                                                                   
      fitness[j]+=pop->chromosome[i];                                           
    fitness[j]/=ceil(p_l/(double) a_f);                                         
  }                                                                             
  //now calculate constraints                                                   
  for (j=0; j<a_f-1; j++){                                                      
    if (fitness[a_f-1]+4.0*fitness[j]-1.0<0.0)                                  
      constraint_violation=true;                                                
  }                                                                             
  for (i=0; i<a_f-1; i++)                                                       
    for (j=i+1; j<a_f-1; j++)                                                   
      if(min>(fitness[i]+fitness[j]))                                           
        min=fitness[i]+fitness[j];                                              
  if (2.0*fitness[a_f-1]+min-1.0<0)                                             
    constraint_violation=true;                                                  
  //mark if constraints violated                                                
  if (constraint_violation)                                                     
    fitness[a_f]=1;                                                             
  else                                                                          
    fitness[a_f]=0;                                                             
                                                                                
  //return l_f objective levels                                                 
  for (i=0; i<l_f; i++)                                                         
    pop->objectives[i]=fitness[i];                                              
}

//---------------------------
void DTLZ8vlv(decision_vector *pop,int p_l,int l_f, double *fitness)
{
  //vlv==very large values (this refers to the method of maintaining constraints

//Page 23, constraint surface approach
  //DTLZ7 in conference paper
  //genome parameters p_l
  //number of objectives l_f
  // p_l>l_f (suggested p_l=10*l_f)
  int i,j;
  double min=10.0; //initialised above constraint violation, it doesn't matter what it really starts at as long as its >=0
  bool constraint_violation =false;
  for (j=0; j<l_f; j++){
    fitness[j]=0.0;
    //printf("\t %d %d %d %d %d\n",(int) ceil(j*(p_l/(double) l_f)),(int) ceil((j+1)*(p_l/(double) l_f)),j,p_l,l_f);
    for (i=(int) ceil(j*(p_l/(double) l_f)); i<(int) ceil((j+1)*(p_l/(double) l_f))-1; i++)
      fitness[j]+=pop->chromosome[i];
    fitness[j]/=ceil(p_l/(double) l_f);
  }
  //printf("\nassigned fitness\n");
  //now calculate constraints, if any infringed - max objectives infeasibley large
  for (j=0; j<l_f-1; j++){
    if (fitness[l_f-1]+4.0*fitness[j]-1.0<0.0)
      constraint_violation=true;
  }
  //printf("\ncv2\n");
  for (i=0; i<l_f-1; i++)
    for (j=i+1; j<l_f-1; j++)
      if(min>(fitness[i]+fitness[j]))
	min=fitness[i]+fitness[j];
  //printf("\ncv1\n");
  if (2.0*fitness[l_f-1]+min-1.0<0)
    constraint_violation=true;
  //printf("\ncv2\n");
  if (constraint_violation){
    //constraints violated so objectives put infeasibley large
    for (j=0; j<l_f; j++)
      fitness[j]=1000000000.0;
  }
  //return l_f objective levels 
  for (i=0; i<l_f; i++)
    pop->objectives[i]=fitness[i];
}
//---------------------------
void DTLZ9(decision_vector *pop,int p_l,int l_f, double *fitness)
{
//Page 23, another lovely constraint surface approach
  //not numbered in conference paper
  //genome parameters p_l
  //number of objectives l_f
  // p_l>l_f (suggested p_l=10*l_f)
  int i,j;
 
  bool constraint_violation =false;
   for (j=0; j<l_f; j++){
    fitness[j]=0.0;
    for (i=(int) ceil(j*p_l/(double) l_f); i<(int) ceil((j+1)*p_l/(double) l_f)-1; i++)
      fitness[j]+=pow(pop->chromosome[i],0.1);
  }
  //now calculate constraints, if any infringed - max objectives infeasibley large
  for (j=0; j<l_f-1; j++){
    if ((pow(fitness[l_f-1],2.0)+pow(fitness[j],2.0)-1.0)<0.0)
      constraint_violation=true;
  }
  if (constraint_violation){
    //constraints violated so objectives put infeasibley large
    for (j=0; j<l_f; j++)
      fitness[j]=1000000000.0;
  }
  //return l_f objective levels 
  for (i=0; i<l_f; i++)
    pop->objectives[i]=fitness[i];
}
//---------------------------

