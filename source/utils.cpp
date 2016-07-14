//							Utility-Functions

#include "globals.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <string.h>
#include <time.h>

#include "utils.h"



void frandinit(void)					// Initialisierung von rand() mit clock
	{
	int  s;
	s = (unsigned int) clock();
	srand(s);
	}

double frand(void)						// Floating point version of rand() { 0 ... 1}
	{
	double x;
	x = (double)rand() / RAND_MAX;
	return (x);
	}


void error(long tx, char* s, int n)
	{
	char *c="";
	
	switch (tx)
		{
		case SYNTAX: 	cerr << "\n---------> Syntaxerror: " << s << '\n';
						break;
		case ZERO: 		cerr << "\n---------> Division by 0 during " << s << '\n';
						break;
		case STRING: 	cerr << "\n---------> String too long: " << s << '\n';
						break;
		case MEMORY: 	cerr << "\n---------> Memory allocation failed: " << s;
#if defined D_MAC
						cerr << "\n---------> Increase application memory size !\n";
#endif						
						break;	
		case FILERR: 		cerr << "\n---------> File I/O error " << n << ", while " << s << '\n';
						break;					
		}							
	//t.close();
#if defined D_MAC
	cerr << "\nCmd-Q to exit ! \n" ;
#endif	
	exit(-1);
	}	
	

double round(double val, long dec)
	{
	double erg,p;
	
	p = pow(10.0, (double) dec);
	erg = floor(val * p + 0.5) / p;
	return erg;
	}
	

double modul(double x, double u, double o)
	{
	double diff,xn,e;
	
	diff = o - u;
	if (diff == 0.0) x = u;
	if (x > o) 
		{
		xn = x - u; 
		x = u + fmod (xn,diff);
		}
	if (x < u)
		{
		xn = o - x; 
		x = o - fmod (xn,diff);
		}
	return x;
	}	

double mirror(double x, double u, double o)
	{
	double diff,xn,e;
	
	diff = o - u;
	if (diff == 0.0) x = u;
	if (x > o) 
		{
		xn = x - u; 
		x = o - fmod (xn,diff);
		}
	if (x < u)
		{
		xn = o - x; 
		x = u + fmod (xn,diff);
		}
	return x;
	}	

	
int compare( const void *n1, const void *n2 )
{
	return ( *((int *) n1) - *((int *) n2) );
}

int dcompare( const double *n1, const double *n2 )
{
	if( *n1 >= *n2 ) return (1);
	else return (-1);
}

int oddeven(long n)		//0=gerade, 1=ungerade
	{
	int erg;
	
	erg = n % 2;
	return erg;
	}  
	

double dminimum(double *x, long n)
	{
	long i;
	double min;
	
	min = x[0];
	for (i=1; i<n; i++)
		if(x[i]<min) min=x[i];
	return min;
	}
	
double dmaximum(double *x, long n)
	{
	long i;
	double max;
	
	max = x[0];
	for (i=1; i<n; i++)
		if(x[i]>max) max=x[i];
	return max;
	}
	
	
double interpol(double ex, double r,double a, double b)
	{
	double erg;
	
	if(ex == 0.0)	
		erg = a + r * (b - a); 
	else if((ex > 0.0) && (b >= a))
		erg = a + pow(r,ex + 1.0) * (b - a);
	else if((ex > 0.0) && (b < a))
		erg = b + pow(1.0-r,ex +1.0) * (a - b);	 				
	else if((ex < 0.0) && (b >= a))
		erg = b + pow(1.0-r,abs(ex) +1.0) * (a - b);	 				
	else if((ex < 0.0) && (b < a))
		erg = a + pow(r,abs(ex) +1.0) * (b - a);	 				

	return erg;
	}	
	
double interpolcos(double r,double a, double b)
	{
	double erg,cx;
	
	cx = cos(ONEPI*r + ONEPI)/2.0 + 0.5;
	erg = a + cx * (b - a); 
	return erg;
	}	
	
		
double mapper(double x, double e)
	{
	if (!e) return x;
	else return pow(x,e);
	}		
		
#if defined D_MAC		
void PStrtoCStr(char *cs, Str255 ps)	
	{long n,i;
	
	n = (long) ps[0];
	for(i=0; i<n; i++)
		cs[i] = ps[i+1];
	cs[n] = NULL;
	}	
	
void CStrtoPStr( Str255 ps,char *cs)	
	{long n,i;
	
	n = strlen(cs);
	for(i=0; i<n; i++)
		ps[i+1] = cs[i];
	ps[0] = (char) n;
	}	
#endif
				
