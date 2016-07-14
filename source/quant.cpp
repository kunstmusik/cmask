#include "globals.h"

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <errno.h>
#include "utils.h"
#include "quant.h"


/*
quantizer::quantizer(long l1, long l2, long l3) 	
	: q(l1), o(l2), s(l3)
	{
	clear();
	}
*/

quantizer::~quantizer(void)
	{
	if(dyn1) delete q;
	if(dyn2) delete o;
	if(dyn3) delete s;
	}

void quantizer::setval(long n, double v)
	{
	switch(n)
		{
		case 1: qv = v; dyn1=0; break;	
		case 2: ov = v; dyn2=0; break;	
		case 3: sv = v; dyn3=0; break;	
		}
	}

void quantizer::setval(long n, double *xv, double *yv,  long len)
	{
	switch(n)
		{
		case 1: 	q = new table(xv, yv, len); 
					if(!q) error(MEMORY, "table");
					dyn1=1; break;	
		case 2: 	o = new table(xv, yv, len); 
					if(!o) error(MEMORY, "table");
					dyn2=1; break;	
		case 3: 	s = new table(xv, yv, len); 
					if(!s) error(MEMORY, "table");
					dyn3=1; break;	
//		case 1: q.create(xv, yv, len); dyn1=1; break;	
//		case 2: o.create(xv, yv, len); dyn2=1; break;	
//		case 3: s.create(xv, yv, len); dyn3=1; break;	
		}
	}


void quantizer::clear() 
	{
/*
	q.clear(); 
	o.clear(); 
	s.clear(); 
*/	
	dyn1 = 0;
	dyn2 = 0;
	dyn3 = 0;
	qmode = qquant;
	qv = 1.0;
	ov = 0.0;
	sv = 1.0;
	limit = 0;
	}	

void quantizer::setall(long a, double v1, double v2, double v3)
	{
	setqmode(a);
	qv = v1;
	ov = v2;
	sv = v3;
	dyn1 = 0;
	dyn2 = 0;
	dyn3 = 0;
	}

void quantizer::setipl(long tab, double a, long b)
	{
	switch (tab)
		{
		case 1 : q->setipl(a,b); break;
		case 2 : o->setipl(a,b); break;
		case 3 : s->setipl(a,b); break;
		}
	}
	
void quantizer::setc(long tab, double ex, double ey)
	{
	switch (tab)
		{
		case TABQQ : q->set(ex,ey); dyn1=1; break;
		case TABQO : o->set(ex,ey); dyn2=1; break;
		case TABQS : s->set(ex,ey); dyn3=1; break;
		}
	}

void quantizer::unsetc(long tab, long z)
	{
	switch (tab)
		{
		case TABQQ : q->unset(z); break;
		case TABQO : o->unset(z); break;
		case TABQS : s->unset(z); break;
		}
	}

void quantizer::setqmode(long a) {qmode = a;}
void quantizer::setqlimit(int a) {limit = a;}

double quantizer::getval(double xt, double gx, double g1, double g2) 
	{	//quantisierter Eingangswert gx an der Stelle xt
	long n = 0;
	double erg,err,d,r;
	 
	if (dyn1) qv = q->getval(xt);
	if (dyn2) ov = o->getval(xt);
	if (dyn3) sv = s->getval(xt);
	
	if(fabs(qv) < 0.0001) cout << "\nWarning: quantization intervall nearly 0 !\n" ;
	if(qv == 0) error (ZERO,"quantization") ;
	d = gx - ov;
	r = floor((d + qv / 2.0) / qv);
	err = d / qv - r;
	erg = ov + (r + err * (1 - sv)) * qv;
	if(limit)
		{
		if (erg < g1) erg = ov + (r + 1.0 + err*(1-sv)) * qv;
		if (erg > g2) erg = ov + (r - 1.0 + err*(1-sv)) * qv;
		}
	return erg; 
	}


// ************* class quantizer  : end                  *************



