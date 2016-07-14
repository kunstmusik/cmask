//  gen.h

#include "tables.h"

#ifndef GENHEADER
#define GENHEADER

enum		{genrnd=1, genosc=2};

class gtable : public table {
private:
	double	pw2;	//Hilfswert für Integration
	double	integr(double, double,double, double); //bestimmtes Integral
public:
				gtable(double*, double*, long);				
				//~gtable();					//Speicher freigeben
	void		setipl(double a=0.0, long b=1); //Interpolationswerte setzen
	double	getphs(double);				//Phasenzeiger lesen
	};
	

class gen {
protected:
	char		name[16];
	enum		{rnduni=1, rndlin=2, rndtri=3, rndexp=4, rndrexp=5, 
				rndbexp=6, rndgauss=7, rndcauchy=8, rndbeta=9, 
				rndweibull=10, rndpoisson=11, rndrlin=12,
				oscsin=20, osccos=21, oscsawup=22, oscsawdown=23, 
				oscsquare=24, osctriangle=25, oscpowerup=26, oscpowerdown=27} funcs;
	double	erg, phs, ex,fr,ra,rb,itime, phpt;
	int		typ,fun,dyn1,dyn2;
	double	tincr;					//control increment
	void		(gen::*gp)(double,double);
				
private:		
	void		setname(void);	
	
	void  	frandinit(void);
	void 		runiform(double d1=0, double d2=0);
	void 		rlinear(double sign=1, double d2=0);
	void 		rtriang(double d1=0, double d2=0);
	void 		rexpone(double lambda=1, double d2=0);
	void  	rrevexp(double lambda=1, double d2=0);
	void  	rbilexp(double lambda=1, double d2=0);
	void   	rgauss(double sigma=0.1, double mu=0.5) ;
	void   	rcauchy(double alpha=0.1, double mu=0.5) ;
	void   	rbeta(double a=0.3,double b=0.3) ;
	void   	rweibull(double s, double t);
	void   	rpoisson(double lambda, double mb);
	
	//void		forward(double);	
	void 		osin(double,double);
	void 		ocos(double,double);
	void 		osawup(double,double);
	void  	osawdown(double,double);
	void  	osquare(double,double);
	void 	 	otriangle(double,double);
	void 	 	opowerdown(double,double);
	void  	opowerup(double,double);
public:
	gtable	*c1,*c2;

			//	gen(long z1=1, long z2=1);		
				gen(void);		
				~gen(void);
	void		setval(long, double);
	void		setval(long, double*, double*, long);
	void		clear(void);	
	//void		size(long z1=1, long z2=1);
	double	getval(double);
	int		settyp(int);
	int		setfunc(char*);
	int		setfunc(int);
	void		setpar(int, double);
	void		setdyn(int, int);
	void		setall(char*, double, double, double);
	void		setc(long, double, double);
	void 		setipl(long, double, long);
	void		getname(char*);
	}; 

#endif