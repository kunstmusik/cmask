#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <cerrno>

#include "globals.h"
#include "utils.h"
#include "field.h"
#include "mask.h"
#include "gen.h"
#include "tables.h"
#include "quant.h"
#include "items.h"


parameter::parameter(void)	
	{
	pprec	= 5;
	//num = n;
	pcflag = pgflag = pmflag = paflag = pqflag = piflag = 0;
	}



void parameter::clear(void) {				
	pgen.clear();
	pmask.clear();
	pquant.clear();
	pprec= 5;
	}


void parameter::setprec(long d)
	{
	pprec = d;
	}


void parameter::setconst(double x)
	{
	pconst = x;
	pcflag = 1;
	}

void parameter::setrange(double x1, double x2)
	{
	pmask.setval(TABMLO, x1);
	pmask.setval(TABMHI, x2);
	}

void parameter::setflag(int f)
	{
	switch(f)
		{
		case RANGE	:
		case MASK	: pmflag = 1; break;
		case ACCUM	: paflag = 1; break;
		case QUANT	: pqflag = 1; break;
		case GEN		: pgflag = 1; break;
		case ITEM	: piflag = 1; break;
		}
	}	

void parameter::delflag(int f)
	{
	switch(f)
		{
		case RANGE	:
		case MASK	: pmflag = 0; break;
		case ACCUM	: paflag = 0; break;
		case QUANT	: pqflag = 0; break;
		case GEN		: pgflag = 0; break;
		case ITEM	: piflag = 0; break;
		}
	}	
	
	
double parameter::getval(double xt)
	{	
	double erg,g1,g2;
	
	erg = 1.0;
	if (pcflag) 				erg = pconst;
	else if (pgflag) 			erg = pgen.getval(xt);
	else if (piflag) 			erg = pitem.getval();
	if(pmflag && !piflag) 	erg = pmask.getval(xt,erg,&g1,&g2);
	if(pqflag && !piflag) 	erg = pquant.getval(xt,erg,g1,g2);
	if(paflag) 					erg = paccum.getval(xt,erg,&g1,&g2);
	erg = round(erg,pprec);
	return erg;
	}



//--------------------------

/*
paramliste::paramliste(void) { n=0; beginn = 0.0; ende = 1.0; } 

paramliste::~paramliste(void)
	{
	long i;
	
	if (n)
		for(p=pe; p>p1; p--)
			delete p;
	}
*/

paramliste::paramliste(long pn) 
	{ 
	long i;
	
	n = (pn >= PARMAX ? PARMAX : pn); 
	p = new parameter[n+1];
	if(!p) error(MEMORY, "field");
	beginn = 0.0; 
	ende = 1.0; 
	for (i=1; i<=n; i++)
		p[i].setnum(i);
	} 

paramliste::~paramliste(void)
	{
	delete[] p;
	}


/*
void paramliste::neu(long pn, long l1, long l2, long l3, long l4, long l5, long l6, long l7, long l8, long l9)
	{
	n++;
	p = new parameter(pn, l1, l2, l3, l4, l5, l6, l7, l8, l9);
	if(!p) error(MEMORY, "pfield");
	if (n==1) p1 = p;
	pe = p;
	p++;
	}		
*/
/*
void paramliste::neu(long pn)
	{
	n++;
	p = new parameter(pn);
	if(!p) error(MEMORY, "pfield");
	if (n==1) p1 = p;
	pe = p;
	p++;
	//p->pmask.setval(1,3.4);
	}		
*/



/*
field::field(long n) 					// Konstruktor
	{
	P = n > PARMAX ? PARMAX : n;
	pmask = new mask[P];
	pgen = new gen[P];
	pquant = new quantizer[P];
	pprec = new int[P];
	clear();
	}

field::~field(void) 
	{ 
	delete[] pmask;
	delete[] pgen;
	delete[] pquant;
	delete[] pprec;
	;}

void field::clear(void) {				
	long i; 
	settime();
	for(i=0; i<P; i++)
		{
		pgen[i].clear();
		pmask[i].clear();
		pquant[i].clear();
		pprec[i]= 6;
		}
	}
	
void field::setmsize(long n, long l1, long l2, long l3, long l4)
	{
	pmask[n].size(l1,l2,l3,l4);
	}	

void field::setgsize(long n, long l1, long l2)
	{
	pgen[n].size(l1,l2);
	}	

void field::settime(double b, double e) { beg=b; end=e;}

//double field::getbeginn(void) { return beg;}

//double field::getende(void) { return end;}


void field::setdec(long n, long d) {pprec[n] = d;}	

void field::setconst(long n, double x)
	{
	setlimit(n,MTAB, 0.0, x, x);
	setdec(n,5);
	}

void field::setrange(long n, double x1, double x2)
	{
	setlimit(n,MTAB, 0.0, x1, x2);
	setdec(n,5);
	}

double field::getmval(long n, double xt) // Errechnen des Parameters n zur Zeit t
	{ 
	double erg,g1,g2;
	
	erg = pgen[n].getval(xt);
	erg = pmask[n].getval(xt,erg,&g1,&g2);
	erg = pquant[n].getval(xt,erg,g1,g2);
	erg = round(erg,pprec[n]);
	return erg;
	}

*/
