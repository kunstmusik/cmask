//							Event-Functions
#include "globals.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "event.h"
#include "utils.h"


event::event(long n)
	{
	val = new double[n+1];
	if(!val) error(MEMORY, "event");
	pcnt = n;
	}

event::~event(void) {delete [] val;}


void event::setop(char s)
	{
	op = s;
	}

void event::setp(long n, double x)
	{
	val[n] = x;
	}

void event::get(char *s)
	{
	char dummy[16];
	long i;

	sprintf(s,"%c",op);
	for(i=1; i <= pcnt; i++)
		{
		if(fabs(val[i])<0.0001) val[i] = 0;
		sprintf(dummy,"%.7g\t",val[i]);
		strcat(s,dummy);
		}
	//sprintf(dummy,NL);
	//strcat(s,dummy);
	strcat(s,NL);
	}
	

