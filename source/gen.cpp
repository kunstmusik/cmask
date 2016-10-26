//                        generator functions

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <cmath>
#include <ctime>

#include "globals.h"
#include "gen.h"
#include "utils.h"

#define PI 3.141592654
#define PI2 6.283185308


gtable::gtable(double *xv, double *yv, long len) : table(xv, yv,len)
	{
	pw2 = 1.0;
 	}


//gtable::~gtable() {delete[] x; delete[] y;}

double gtable::integr(double x1, double xe, double y1, double y2)  
	// bestimmtes Integral zwischen 0 und x1 für y1+(y2-y1)*(x^pw2)/(xe^pw2)
	{
	double erg;
	
	erg = x1*y1 + pow(x1,1.0+pw2)*(y2-y1) / ((1.0+pw2) * pow(xe,pw2));
	//erg = fmod(erg, 1.0);
	return erg;
	}

void gtable::setipl(double a, long b) 
	{ 
	iplval = a; 
	iplflag = b;
	if(iplflag) pw2 = pow(2.0,a);
	}

double gtable::getphs(double xt) {			//Phasenzeiger an der Stelle xt
	long i,k;
	double erg,f,xtr,phsum;
	
	phsum = 0.0;
	i = 0;
	xtr = round(xt,10); 
	if(!N) 						erg = 0.0;
	else if(N==1) 				erg = xtr*y[0];
	else if(xtr <= x[0]) 	erg = x[0]*y[0] - xtr*y[0];
	//else if(xtr > x[N-1]) 	erg = x[y[N-1];
	else 
		{
		while ((x[i] < xtr) && (i<N)) i++;			//Suche nach 1. x-Tabelleneintrag größergleich xt
		if (iplflag)
			{
			if(i >= 2)
				for(k=0; k < (i-1); k++)	
					phsum += integr(x[k+1]-x[k],x[k+1]-x[k],y[k],y[k+1]); //Integration aller kompletten Segmente
			if (xtr >= x[N-1])
				phsum += xtr*y[N-1] - x[N-1]*y[N-1];
			else 
				{	
				//f = (xtr - x[i-1]) / (x[i] - x[i-1]);	//Prozent des Zeitintervalls an der Stelle xt
				//erg = interpol(iplval,f,y[i-1], y[i]);	//Interpolation des Wertes an Stelle xt
				phsum += integr(xtr-x[i-1],x[i]-x[i-1],y[i-1], y[i]);
				//cout << f << "  " << erg << endl;
				}
			erg = phsum;
			}
		else 
			{
			if(i >= 2)
				for(k=0; k < (i-1); k++)	
					phsum += x[k+1]*y[k] - x[k]*y[k]; //Integration aller Stufen-Segmente
			phsum += xtr*y[i-1] - x[i-1]*y[i-1];
			erg = phsum;
			}
		}	
	return erg; 
	}



gen::gen(void)
	{
	typ = genrnd;
	fun = rnduni;
	phs = 0.0;
	ex = 1.0;
	gp = &gen::runiform;
	setname();
	frandinit();
	dyn1 = 0;
	dyn2 = 0;
	fr = 1.0;
	ra = 0.5;
	rb = 0.5;
	itime = phpt = 0.0;
	tincr = 0.001;
	}

gen::~gen(void)
	{
	if(dyn1) delete c1;
	if(dyn2) delete c2;
	}

void gen::clear(void)
	{
	c1->clear(0.5);
	c2->clear(0.5);
	}

/*
void gen::size(long z1, long z2)
	{
	c1.size(z1);
	c2.size(z2);
	}
*/

void gen::setval(long n, double v)
	{
	switch(n)
		{
		case 1: ra = v; fr = v; dyn1=0; break;	
		case 2: rb = v; phs = phpt = v; dyn2=0;break;	
		case 3: ex = v; break;	
		}
	}

void gen::setval(long n, double *xv, double *yv,  long len)
	{
	switch(n)
		{
		case 1: 	c1 = new gtable(xv, yv, len); 
					if(!c1) error(MEMORY, "table");
					dyn1=1; 
					break;	
		case 2: 	c2 = new gtable(xv, yv, len);
					if(!c2) error(MEMORY, "table");
				 	dyn2=1; break;	
		//case 1: c1.create(xv, yv, len); dyn1=1; break;	
		//case 2: c2.create(xv, yv, len); dyn2=1; break;	
		}
	}

/*
void gen::setall(char *s, double v1, double v2 , double v3)
	{
	setfunc(s);
	switch(typ)
		{
		case genrnd: ra = v1; rb = v2; break;	
		case genosc: fr = v1; phs = v2; ex = v3; break;	
		}
	}
	
void gen::setc(long n, double x, double y)
	{
	switch(n)
		{
		case 1:	dyn1=1; c1->set(x,y); break;
		case 2:	dyn2=1; c2->set(x,y); break;
		}
	}			
*/

void gen::setipl(long n, double a, long b)
	{
	switch(n)
		{
		case 1:	c1->setipl(a,b); break;
		case 2:	c2->setipl(a,b); break;
		}
	}			

/*

void	gen::setdyn(int d1, int d2) {	dyn1=d1; dyn2=d2;}

void	gen::setpar(int n, double x) 
	{
	switch(n)
		{
		case 1:	ra = x; break;
		case 2:	rb = x; break;
		case 3:	fr = x; break;
		case 4:	phs = x; break;
		case 5:	ex = x; break;
		case 6:	start = x; break;
		}
	}	
*/			

int gen::settyp(int n)
	{
	int	eflag;
	
	eflag = 1;
	switch(n)
		{
		case genrnd: 		typ=genrnd; break;
		case genosc: 		typ=genosc; break;
		default:				eflag = 0; break;
		}
	return eflag;
	}	

int gen::setfunc(int n)
	{
	int	eflag;
	
	eflag = 1;
	switch(n)
		{
		case oscsin: 		gp = &gen::osin; typ=genosc; break;
		case osccos: 		gp = &gen::ocos; typ=genosc; break;
		case oscsawup: 	gp = &gen::osawup; typ=genosc; break;
		case oscsawdown: 	gp = &gen::osawdown; typ=genosc; break;
		case oscsquare: 	gp = &gen::osquare; typ=genosc; break;
		case osctriangle: gp = &gen::otriangle; typ=genosc; break;
		case oscpowerup:	gp = &gen::opowerup; typ=genosc; break;
		case oscpowerdown: gp = &gen::opowerdown; typ=genosc; break;
		case rnduni: 		gp = &gen::runiform; typ=genrnd; break;
		case rndlin: 		gp = &gen::rlinear; typ=genrnd; ra= 1.0; break;
		case rndrlin: 		gp = &gen::rlinear; typ=genrnd; ra= -1.0; break;
		case rndtri: 		gp = &gen::rtriang; typ=genrnd; break;
		case rndexp: 		gp = &gen::rexpone; typ=genrnd; ra=1.0; break;
		case rndrexp: 		gp = &gen::rrevexp; typ=genrnd; ra=1.0; break;
		case rndbexp: 		gp = &gen::rbilexp; typ=genrnd; ra=1.0; break;
		case rndbeta: 		gp = &gen::rbeta; typ=genrnd;  ra=.1; rb=.1; break;
		case rndgauss: 	gp = &gen::rgauss; typ=genrnd; ra=.1; rb=.5; break;
		case rndcauchy: 	gp = &gen::rcauchy; typ=genrnd; ra=.1; rb=.5; break;
		case rndweibull: 	gp = &gen::rweibull; typ=genrnd; ra=.5; rb=2.0; break;
		default: 			gp = &gen::runiform; typ=genrnd; eflag = 0; break;
		}
	if (eflag) fun = n;
	else fun = rnduni;	
	return eflag;
	}

		
int gen::setfunc(char *n)
	{
	int eflag;
	
	eflag = 0;
	if(!strncmp(n,"sine",2)) 		eflag = oscsin;
	else if(!strncmp(n,"cosine",2)) 	eflag = osccos;
	else if(!strncmp(n,"sawup",4)) 		eflag = oscsawup;
	else if(!strncmp(n,"sawdown",4)) 	eflag = oscsawdown;
	else if(!strncmp(n,"square",2)) 	eflag = oscsquare;
	else if((!strncmp(n,"triangle",2)) && (typ == genosc)) 	
											eflag = osctriangle;
	else if(!strncmp(n,"powup",4)) 		eflag = oscpowerup;
	else if(!strncmp(n,"powdown",4)) 	eflag = oscpowerdown;
	else if(!strncmp(n,"uniform",2)) 	eflag = rnduni;
	else if(!strncmp(n,"linear",2)) 	eflag = rndlin;
	else if(!strncmp(n,"revlinear",2)) 	eflag = rndrlin;
	else if(!strncmp(n,"rlinear",2)) 	eflag = rndrlin;
	else if((!strncmp(n,"triangular",2)) && (typ == genrnd)) 
											eflag = rndtri;
	else if(!strncmp(n,"expone",2)) 	eflag = rndexp;
	else if(!strncmp(n,"revexp",2)) 	eflag = rndrexp;
	else if(!strncmp(n,"rexpone",2)) 	eflag = rndrexp;
	else if(!strncmp(n,"bilexp",2)) 	eflag = rndbexp;
	else if(!strncmp(n,"bexp",2)) 		eflag = rndbexp;
	else if(!strncmp(n,"gauss",2)) 		eflag = rndgauss;
	else if(!strncmp(n,"cauchy",2)) 	eflag = rndcauchy;
	else if(!strncmp(n,"beta",2)) 		eflag = rndbeta;
	else if(!strncmp(n,"weibull",2)) 	eflag = rndweibull;
	else if(!strncmp(n,"poisson",2)) 	eflag = rndpoisson;
	return setfunc(eflag);
	}
		
void gen::setname(void)
	{
	switch(fun)				
		{
		case oscsin: 		strcpy(name,"sine"); break;
		case osccos: 		strcpy(name,"cosine"); break;
		case oscsawup: 	strcpy(name,"sawup"); break;
		case oscsawdown: 	strcpy(name,"sawdown"); break;
		case oscsquare: 	strcpy(name,"square"); break;
		case osctriangle: strcpy(name,"triangle"); break;
		case oscpowerup:	strcpy(name,"powerup"); break;
		case oscpowerdown: strcpy(name,"powerdown"); break;
		case rnduni: 		strcpy(name,"uniform"); break;
		case rndlin: 		strcpy(name,"linear"); break;
		case rndtri: 		strcpy(name,"triangular"); break;
		case rndexp: 		strcpy(name,"expon"); break;
		case rndrexp: 		strcpy(name,"revexp"); break;
		case rndbexp: 		strcpy(name,"bilexp"); break;
		case rndgauss: 	strcpy(name,"gauss"); break;
		case rndcauchy:	strcpy(name,"cauchy"); break;
		case rndbeta: 		strcpy(name,"beta"); break;
		case rndweibull: 	strcpy(name,"weibull"); break;
		case rndpoisson: 	strcpy(name,"poisson"); break;
		}
	}

void	gen::getname(char *str)
	{
	int	n;
	
	n = strlen(str);
	strncpy(str,name,n);
	}	


//   ---------------        periodische Funktionen


/*
void gen::forward(double time)
	{
	double dtime, erg;
	long	i,n;
	
	dtime = time - itime;
	n = (long)round(dtime/tincr, 0);
	if(dyn1) 
		{
		for(i=0; i<=n; i++) 
			phpt += tincr * c1->getval(itime+i*tincr);
		//phpt -= tincr * c1->getval(time) / 2.0;
		}
	else 
		phpt = fr * time + phs;
	itime = time;	
	}
*/

void gen::osin(double a,double b)
	{
	erg = sin(PI2 * phpt) * 0.5 + 0.5;
	}

void gen::ocos(double a,double b)
	{
	erg = cos(PI2 * phpt) * 0.5 + 0.5;
	}


void gen::osawup(double a,double b)
	{
	erg = fabs(fmod(phpt, 1.0));
	}

void gen::osawdown(double a,double b)
	{
	erg = 1.0 - fabs(fmod(phpt, 1.0));
	}

void gen::osquare(double a,double b)
	{
	double x;
	x = fabs(fmod(phpt, 1.0));
	erg = (x < 0.5 ? 1.0 : 0.0) ;
	}

void gen::otriangle(double a,double b)
	{
	double x;
	x = fabs(fmod(phpt, 1.0));
	erg = (x < 0.5 ? (2.0 * x) : (2.0 * (1.0 - x)));
	}

void gen::opowerup(double a,double b)
	{
	erg = pow(fabs(fmod(phpt, 1.0)), pow(2.0,ex));
	}

void gen::opowerdown(double a,double b)
	{
	erg = pow(1.0 - fabs(fmod(phpt, 1.0)),pow(2.0,ex));
	}


//transienten 
/*
double trpowerdown(double period, double time, double ex)
	{
	double x;
		x = fabs(fmod(time / period, 1.0);
	x = (x >= 1.0 ? 0.0 : pow(1.0 - x,ex));
	return(x);
	}

double trpowerup(double period, double time, double ex)
	{
	double x;
	x = fabs(fmod(time / period, 1.0);
	x = (x >= 1.0 ? 1.0 : pow(x,ex));
	return(x);
	}
*/


void gen::frandinit(void)					// Initialisierung von rand() mit clock
	{
	int  s;
	s = (unsigned int) clock();
	srand(s);
	}


void 	gen::runiform(double d1, double d2)
	{
	erg = frand();
	}
 
void gen::rlinear(double sign, double d2)
	{
	double x1,x2;
	x1 = frand();
	x2 = frand();
	if (sign >= 0.0) 	erg = x1 < x2 ? x1 : x2;
	else 					erg = x1 > x2 ? x1 : x2;
	}
 
void gen::rtriang(double d1, double d2)
	{
	double x1,x2;
	x1 = frand();
	x2 = frand();
	erg = (0.5 * (x1 + x2));
	}
 
void gen::rexpone(double lambda, double d2)         		//lambda >0 !!
	{
	double x;
	do
		{
		while ((x = frand()) == 0);
		x = -(double)log((double)x) / 7.0 / lambda;         // /7 -> "Normierung für 99,9%"
		}
	while (x > 1.0);
	erg = x;
	}
 
void gen::rrevexp(double lambda, double d2)         		//lambda >0 !!
	{
	double x;
	
	do
		{
		while ((x = frand()) == 0);
		x = -(double)log((double)x) / 7.0 / lambda;         // /7 -> "Normierung für 99,9%"
		}
	while (x > 1.0);
	erg = 1.0 - x;
	}

 
void gen::rbilexp(double lambda, double d)
	{
	double x,e;
	do
		{
		x = 2.0 * frand();
		if (x > 1.0)
			{
			x = 2.0 - x;
			e = -(double)log((double)x);
			}
		else
			e = (double)log((double)x);
		e = (e / 14.0 / lambda) + 0.5;
		}
	while ((e > 1.0) || (e < 0.0));
	erg = e;
	}
 
void gen::rgauss(double sigma, double mu)    		// sigma = Standardabweichung  mu = Mittelwert
    {                   					// mu+-sigma -> 68.26% aller x
 	double e,sum;
 	int i;
	do
		{
		sum = 0;
		for(i = 1; i <= 12; i++) sum += frand();
		e = sigma * (sum-6.0) + mu;
		}
	while ((e > 1.0) || (e < 0.0));
	erg = e;
	}
 
void gen::rcauchy(double alpha, double mu)          //alpha -> Bereich für 50% aller x
	{
	double x,e;
	do                               		// 318*alpha -> 99.9% aller x
		{
		do                             		// mu -> Mittelwert
			x = frand();
		while ( x == 0.5);
		e = alpha * tan(x * PI) + mu;
		}
	while ((e > 1.0) || (e < 0.0));
	erg = e;
	}
 
void gen::rbeta(double a,double b)                 // a -> Breite bei Null
	{										// b -> Breite bei Eins
	double x1,x2,yps1,yps2,sum;
	do
		{
		x1 = frand();
		x2 = frand();
		yps1 = pow (x1, (1.0 / a));
		yps2 = pow (x2, (1.0 / b));
		sum = yps1 + yps2;
		}
	while (sum > 1.0);
	erg = yps1 / sum;
	}
 
void gen::rweibull(double s, double t)             // s -> horizontale Dehnung
	{										// t -> shape
	double x, a, e;							// t>1 -> max bei s
	do
		{
		x = frand();                              
		a = 1.0 / (1.0 - x);
		e = s * pow(log(a), (1.0 / t));
		}
	while (e > 1);
	erg = e;
	}
 
void gen::rpoisson(double lambda, double mb)         // max bei int(lambda)
	{										// if lambda is int
	double x,v;								//     -> max bei lambda und lambda-1
	int n;
	do
		{                                
		n = 0;                                 
		x = frand();
		v = (double)exp(-(double)lambda);
		while (x < v)
			{
			n++;
			x *= frand();
			}
		}	
	while (n > mb);
	erg = n;
	}
 
double gen::getval(double xt)
	{
	if(typ == genrnd)
		{
		if(dyn1) ra = c1->getval(xt);
		if(dyn2) rb = c2->getval(xt);
		(this->*gp)(ra,rb);
		}
	if(typ == genosc)
		{
		//if(dyn1) fr = c1->getval(xt);
		if(dyn1)
			phpt = phs + c1->getphs(xt);
		else
			phpt = phs + fr * xt;
		//erg = phpt;		
		(this->*gp)(0,0);
		}
	return erg;
	}
 	
