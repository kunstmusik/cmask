#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <cerrno>

#include "globals.h"
#include "tables.h"
#include "utils.h"


table::table(double *xv, double *yv, long len)
	{
	long i;

	M = len;
//	if(M > TPMAX) M = TPMAX;				
	x = new double[M];
	y = new double[M];
	if(!x || !y) error(MEMORY, "table");
	for (i=0; i<M; i++)
		{
		x[i] = xv[i];
		y[i] = yv[i];
		}
	N = M;
	iplval = 0.0;
	iplflag = 1;
 	}

/*
table::table(long z) {					//Konstruktor 

	M = z;
	if(M > TPMAX) M = TPMAX;				
	x = new double[M];
	y = new double[M];
	if(!x || !y) error(MEMORY, "table");
	clear(); 
	}
*/

table::~table() {delete[] x; delete[] y;}

//table::~table() {;}
/*
void table::size(long z) {					//Pseudo-Konstruktor 
	M = z;
	if(M > TPMAX) M = TPMAX;				
	x = new double[M];
	y = new double[M];
	if(!x || !y) error(MEMORY, "table");
	clear(); }
*/

/*

void table::create(double *xv, double *yv, long len)
	{
	long i;

	M = len;
//	if(M > TPMAX) M = TPMAX;				
	x = new double[M];
	y = new double[M];
	if(!x || !y) error(MEMORY, "table");
	for (i=0; i<M; i++)
		{
		x[i] = xv[i];
		y[i] = yv[i];
		}
	N = M;
	iplval = 0.0;
	iplflag = 1;
 	}
*/

void table::clear(double v) {
	long i; 
	for(i=0; i<M; i++) { x[i]=0.0; y[i]=v; }
	N=0; 
	iplval = 0.0;
	iplflag = 1;

	}	

void table::setipl(double a, long b) { iplval = a; iplflag = b;}
	
void table::set(double ex, double ey) {		//Setzen eines Wertepaares
	long i,k;
	
	if(N==0) { x[0]=ex; y[0]=ey; N++;}
	else if(N==M) return;					//Falls Maximum, sofort zurŸck
	else if(ex > x[N-1]) { x[N]=ex; y[N]=ey; N++;}
	else 
		{
		for(i=0; (ex >= x[i]) && i<=N  ; i++) ;
		if(ex==x[i])						//Falls x-Wert schon vorhanden, Wert Šndern
			{ x[i]=ex; y[i]=ey;}	
		else 
			{	
			for(k=N; k>i; k--)				//Wertepaare nach vorn verschieben
				{ x[k]=x[k-1]; y[k]=y[k-1]; }
			x[i]=ex; y[i]=ey; 
			N++;
			}	
		} 
	return;
	}	

void table::unset(long z) {					//Lšschen eines Wertepaares
	long i;

	if((z < 0) || (z >= N)) return;
	for(i = z; i < (N-1); i++)					//Wertepaare zurŸck verschieben
		{ x[i] = x[i+1]; y[i] = y[i+1]; } 
	N--; 
	return;	}

void table::change(long z, double ex,double ey) {x[z]=ex;y[z]=ey;return;}
	
double table::getval(double xt) {			//interpolierter Tabellenwert an der Stelle xt
	long i = 0;
	double erg,f,xtr;
	
	xtr = round(xt,10); 
	if(!N) 					erg = y[0];
	else if(xtr <= x[0]) 	erg = y[0];
	else if(xtr > x[N-1]) 	erg = y[N-1];
	else 
		{
		while (x[i] < xtr) i++;					//Suche nach 1. x-Tabelleneintrag grš§ergleich xt
		switch (iplflag)
			{
			case 0 : 	//keine Interpolation = Stufe
						erg = (xtr < x[i] ? y[i-1] : y[i]);
						break;
			case 1 :	//Prozent des Zeitintervalls an der Stelle xt
						//Interpolation des Wertes an Stelle xt
						f = (xtr - x[i-1]) / (x[i] - x[i-1]);
						erg = interpol(iplval,f,y[i-1], y[i]);
						break;
			case 2 :	//Interpolation mit halbem Cosinus an Stelle xt
						f = (xtr - x[i-1]) / (x[i] - x[i-1]);
						erg = interpolcos(f,y[i-1], y[i]);
						break;
			}
		}	
	return erg; }

void table::print(void) {
	long i;

	printf("\n Tabellenpunkte : %ld \n",N);
	for(i=0; i<N; i++)
		printf("x(%ld)=%.4g  y(%ld)=%.4g\n",i,x[i],i,y[i]);
	return;
	}


// ************* class table  : end                  *************




table2::table2(long z) {		//Konstruktor
	M = z;
//	if(M > TPMAX) M = TPMAX;				
	x = new double[M];
	y1 = new double[M];
	y2 = new double[M];
	if(!x || !y1 || !y2) error(MEMORY, "table2");
	clear(); 
	}

table2::~table2() {delete[]x; delete[]y1; delete[]y2;}
//table2::~table2() {;}

/*
void table2::size(long z) {				//Pseudo-Konstruktor
	M = z;
	if(M > TPMAX) M = TPMAX;				
	x = new double[M];
	y1 = new double[M];
	y2 = new double[M];
	if(!x || !y1 || !y2) error(MEMORY, "table2");
	clear(); 
	}
*/	

void table2::create(double *xv, double *y1v, double *y2v, long len)
	{
	long i;

	M = len;
	//if(M > TPMAX) M = TPMAX;				
	x = new double[M];
	y1 = new double[M];
	y2 = new double[M];
	if(!x || !y1 || !y2) error(MEMORY, "table");
	for (i=0; i<M; i++)
		{
		x[i] = xv[i];
		y1[i] = y1v[i];
		y2[i] = y2v[i];
		}
	N = M;
	iplval = 0.0;
	iplflag = 1;
 	}


void table2::clear(double v1, double v2) {
	long i; 
	for(i=0; i<M; i++) { x[i]=0.0; y1[i]=v1; y2[i]=v2; }
	N=0; 
	iplval = 0.0;
	iplflag = 1;
	}	

void table2::setipl(double a, long b) { iplval = a; iplflag = b;}

void table2::set(double ex, double ey1, double ey2) {		//Setzen eines Wertepaares
	long i,k;
	
	if(N==0) { x[0]=ex; y1[0]=ey1; y2[0]=ey2; N++;}
	else if(N==M) return;						//Falls Maximum, sofort zurŸck
	else if(ex > x[N-1]) { x[N]=ex; y1[N]=ey1; y2[N]=ey2; N++;}
	else 
		{
		for(i=0; (ex >= x[i]) && i<=N  ; i++) ;
		if(ex==x[i])						//Falls x-Wert schon vorhanden, Wert Šndern
			{ x[i]=ex; y1[i]=ey1; y2[i]=ey2;}	
		else 
			{	
			for(k=N; k>i; k--)				//Wertepaare nach vorn verschieben
				{ x[k]=x[k-1]; y1[k]=y1[k-1]; y2[k]=y2[k-1];}
			x[i]=ex; y1[i]=ey1; y2[i]=ey2; 
			N++;
			}	
		} 
	return;
	}	
	
void table2::unset(long z) {					//Lšschen eines Wertetripels
	long i;

	if((z < 0) || (z >= N)) return;
	for(i = z; i < (N-1); i++)					//Wertepaare zurŸck verschieben
		{ x[i] = x[i+1]; y1[i] = y1[i+1]; y2[i] = y2[i+1]; } 
	N--; 
	return;	}

void table2::change(long z, double ex,double ey1, double ey2) 
	{x[z]=ex;y1[z]=ey1;y2[z]=ey2;return;}

void table2::getvals(double xt, double *e1, double *e2) {	 //interpolierte Tabellenwerte an der Stelle xt
	long i = 0;
	double erg1,erg2,f,xtr;

	xtr = round(xt,10); 
	if(!N)					{erg1 = y1[0]; erg2 = y2[0];}
	else if(xtr <= x[0]) 	{erg1 = y1[0]; erg2 = y2[0];}
	else if(xtr > x[N-1]) 	{erg1 = y1[N-1]; erg2 = y2[N-1];}
	else 
		{ 
		while (x[i] < xtr) i++;					//Suche nach 1. x-Tabelleneintrag grš§ergleich xt
		switch (iplflag)
			{
			case 0 : 	//keine Interpolation = Stufe
						erg1 = (xtr < x[i] ? y1[i-1] : y1[i]);	
						erg2 = (xtr < x[i] ? y2[i-1] : y2[i]);
						break;
			case 1 :	//Prozent des Zeitintervalls an der Stelle xt
						//Interpolation des Wertes an Stelle xt
						f = (xtr - x[i-1]) / (x[i] - x[i-1]);
						erg1 = interpol(iplval,f,y1[i-1], y1[i]);	
						erg2 = interpol(iplval,f,y2[i-1], y2[i]);
						break;
			case 2 :	//Interpolation mit halbem Cosinus an Stelle xt
						f = (xtr - x[i-1]) / (x[i] - x[i-1]);
						erg1 = interpolcos(f,y1[i-1], y1[i]);	
						erg2 = interpolcos(f,y2[i-1], y2[i]);
						break;
			}
		}
	*e1 = erg1;
	*e2 = erg2;
	//return erg;
	}


void table2::print(void) {
	long i;

	printf("\n Tabellenpunkte : %ld \n\n",N);
	for(i=0; i<N; i++)
		printf("x(%ld)=%.4g  y1(%ld)=%.4g  y2(%ld)=%.4g\n",i,x[i],i,y1[i],i,y2[i]);
	return;
	}

// ************* class table2  : end                  *************

