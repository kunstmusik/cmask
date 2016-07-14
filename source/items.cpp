#include "globals.h"

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <errno.h>
#include "utils.h"
#include "items.h"


itemlist::itemlist(void)
	{
	elem = NULL;
	helem = NULL;
	ne = 0;
	ip = 0;
	ih = 0;
	is = 1;
	}

itemlist::~itemlist(void)
	{
	if(elem) delete elem;
	if(helem) delete helem;
	}

double itemlist::icycle(void)
	{
	double erg;
	
	erg = elem[ip];
	ip++;
	if (ip >= ne) ip = 0;
	return erg;	
	}

double itemlist::iswing(void)
	{
	double erg;
	
	erg = elem[ip];
	if (is) 
		{
		ip++;
		if (ip >= ne) 
			{
			ip-= 2;
			is = 0;
			}
		}	
	else 
		{
		ip--;
		if (ip < 0) 
			{
			ip = 1;
			is = 1;
			}
		}	
	return erg;	
	}

double itemlist::irandom(void)
	{
	double erg;
	
	erg = floor(frand() * (double)ne);
	erg = elem[(long)erg];
	return erg;
	}
	
double itemlist::iheap(void)
	{
	double erg;
	long i;
	
	do
		i = (long) (floor(frand() * (double)ne));
	while(helem[i]);
	erg = elem[i];
	helem[i] = 1;
	ih++;
	if(ih >= ne)
		{
		ih = 0;
		for(i=0; i<ne; i++) helem[i] = 0;
		}
	return erg;	
	}

double itemlist::ifold(void)
	{
	double erg;
	long i;
	
	erg = elem[ip];
	ip++;
	if (ip >= ne) 
		{
		ip = 0;
		folder();
		}
	return erg;	
	}

double itemlist::folder(void)
	{
	double erg;
	long i,mid;
	int odd;
	
	odd = oddeven(ne);
	mid = ne / 2;
	}


void itemlist::setval(double *ev, long len)
	{
	long i;
	
	elem = new double[len]; 
	if(!elem) error(MEMORY, "itemlist");
	ne = len;
	for(i=0; i<ne; i++) elem[i] = ev[i];
	if(mode==HEAP)
		{
		helem = new unsigned char[ne]; 
		if(!helem) error(MEMORY, "itemlist");
		for(i=0; i<ne; i++) helem[i] = 0;
		} 
	}

int itemlist::setmode(char *s) 
	{
	int eflag;
	long i;
	
	eflag = -1;
	if(!strncmp(s,"cycle",2)) 	{eflag = mode = CYCLE; f = &itemlist::icycle;}
	if(!strncmp(s,"swing",2)) 	{eflag = mode = SWING; f = &itemlist::iswing;}
	if(!strncmp(s,"random",2)) 	{eflag = mode = RANDOM; f = &itemlist::irandom;}
	if(!strncmp(s,"heap",2))  	{eflag = mode = HEAP; 	f = &itemlist::iheap;}
	return eflag;
	}

double itemlist::getval(void) 
	{	
	double erg;
	
	erg = (this->*f)();
	return erg; 
	}


// ************* class itemlist  : end                  *************



