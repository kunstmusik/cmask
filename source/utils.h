//							Utility-Functions

#ifndef UTILHEADER
#define UTILHEADER

#include "globals.h"

#define SYNTAX 	1
#define MEMORY 	2
#define FILERR 	3
#define STRING 	4
#define ZERO 	5

#define LINE 	1
#define EXPON	2

#define TWOPI   6.283185308
#define ONEPI   3.141592654

void frandinit(void);
double	frand(void);
void error(long, char* s, int n=0 );
double round(double val, long dec);
double modul(double x, double u, double o);
double mirror(double x, double u, double o);
int compare( const void*, const void*);
int dcompare( const double*, const double*);
int oddeven(long);
double dminimum(double*, long);
double dmaximum(double*, long);
double interpol(double, double,double, double);
double interpolcos(double,double, double);
double mapper(double,double);

#if defined D_MAC
void PStrtoCStr(char *, Str255);
void CStrtoPStr( Str255,char *);	
#endif

#endif
