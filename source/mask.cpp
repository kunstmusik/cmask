#include "globals.h"

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include "utils.h"
#include "mask.h"


mask::mask(void)  //Konstruktor 
	{	
	clear();
	}

mask::~mask(void)
	{
	if(dyn1) delete lo;
	if(dyn2) delete hi;
	}

void mask::setval(long n, double v)
	{
	switch(n)
		{
		case 1: lov = v; dyn1=0; break;	
		case 2: hiv = v; dyn2=0; break;	
		}
	}

void mask::setval(long n, double *xv, double *yv,  long len)
	{
	switch(n)
		{
		case 1: 	lo = new table(xv, yv, len); 
					if(!lo) error(MEMORY, "table");
					dyn1=1; break;	
		case 2: 	hi = new table(xv, yv, len); 
					if(!hi) error(MEMORY, "table");
					dyn2=1; break;	
		}
	}
 

void mask::clear() 
	{
	/*
	mlo.clear(); 
	mhi.clear(); 
	blo.clear(); 
	bhi.clear(); 
	*/
	map = 0.0;
	dyn1=0;
	dyn2=0;
	lov=0.0;
	hiv=1.0;
	}	

void mask::setipl(long tab, double a, long b)
	{
	switch (tab)
		{
		case 1 : lo->setipl(a,b); break;
		case 2 : hi->setipl(a,b); break;
		}
	}

/*	
void mask::setlimit(long tab, double ex, double ey)
	{
	switch (tab)
		{
		case 1 : mlo->set(ex,ey); break;
		case 2 : mhi->set(ex,ey); break;
		case 3 : blo->set(ex,ey); break;
		case 4 : bhi->set(ex,ey); break;
		}
	}

void mask::unsetlimit(long tab, long z)
	{
	switch (tab)
		{
		case 1 : mlo->unset(z); break;
		case 2 : mhi->unset(z); break;
		case 3 : blo->unset(z); break;
		case 4 : bhi->unset(z); break;
		}
	}
*/

void mask::setmap(double m) {map = pow(2.0,m);}

double mask::getval(double xt, double gx, double *g1, double *g2) 
	{	//skalierter Eingangswert gx {0,1} an der Stelle xt
	long n = 0;
	double erg;
	 
	if(dyn1) lov = lo->getval(xt);
	if(dyn2) hiv = hi->getval(xt);
	erg = lov + (hiv - lov) * mapper(gx,map);
	*g1 = lov; *g2 = hiv;
	return erg; 
	}
// ************* class mask  : end                  *************



// ************* class accum  : start               *************

accum::accum(void)  	//Konstruktor 
	{	
	clear();
	}

/*	
accum::~accum(void)		//Destruktor
	{
	if(dyn1) delete lo;
	if(dyn2) delete hi;
	}
*/

void accum::clear() 
	{
/*
	dyn1=0;
	dyn2=0;
	lov=0.0;
	hiv=1.0;
*/
	initakku(0.0);
	setbmode("off");
	}	


int accum::setbmode(char *s) 
	{
	int eflag;
	
	eflag = -1;
	if(!strncmp(s,"off",2)) 	eflag = bmode = boff;
	if(!strncmp(s,"on",2)) 		eflag = bmode = bakku;
	if(!strncmp(s,"wrap",2)) 	eflag = bmode = bwrap;
	if(!strncmp(s,"mirror",2)) 	eflag = bmode = bmirror;
	if(!strncmp(s,"limit",2)) 	eflag = bmode = blimit;
	return eflag;
	}


void accum::initakku(double a) {akku = a;}

double accum::getval(double xt, double gx, double *g1, double *g2) 
	{	//skalierter Eingangswert gx an der Stelle xt
	long n = 0;
	double erg;
	 
 	if (bmode) 
		{
		akku += gx;
		if(dyn1) lov = lo->getval(xt);
		if(dyn2) hiv = hi->getval(xt);
		switch (bmode) 
			{
			case bakku: 	break;
			case bwrap: 	akku = modul(akku, lov, hiv); break;
			case bmirror: 	akku = mirror(akku, lov, hiv); break;
			case blimit: 	if (akku < lov) akku = lov;
						 	if (akku > hiv) akku = hiv;
							break;
			}
		erg = akku; 
	/*
		if(bmode!=bakku) 			
			{
			*g1 = blov; *g2 = bhiv;
			}
	*/
		}
	return erg; 
	}


// ************* class accum  : end                  *************



