#include "tables.h"

#ifndef MASKHEADER
#define MASKHEADER

//#define MTMAX 50	// Maximale Anzahl Tabellenpunkte für Masken und Boundaries


enum	{boff = 0, bakku, bwrap, bmirror, blimit}; // bmodes;
enum	{TABMLO = 1, TABMHI, TABBLO, TABBHI}; // tables;


// ************* class mask  :  *************
class mask {
public:

	table		*lo,*hi;					//Tables für Maskenlimits
	double	map;						//Exponent für MaskenMapping
	double	lov, hiv;				//Maskenlimits
	int		dyn1, dyn2;				//Tableflags

				mask(void);				 
				~mask(void); 	
	void		setval(long, double);					 
	void		setval(long, double*, double*, long);					 
	void		clear();
	void		setipl(long c, double a=0.0, long b=0); //intpol
	void		setmap(double m=0.0);
	double	getval(double, double, double*, double*);				//Zufallswert lesen
	};


// ************* class accum  :  *************
class accum : public mask {
	long		bmode;							//Modus: limit, mirror, wrap, off, on
	double	akku;								//Akkumulator, Startwert für Randomwalks

public:
				accum(void);				 
				//~accum(void); 
	void		clear();
	int		setbmode(char *s);				//Bound-Modus setzen
	void		initakku(double a=0.0);				//StartWert setzen
	double	getval(double, double, double*, double*);				//Zufallswert lesen
	};



#endif