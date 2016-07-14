#include "tables.h"

#ifndef QUANTHEADER
#define QUANTHEADER

//#define QTMAX 50	// Maximale Anzahl Tabellenpunkte Quantizertabellen


enum	{qoff = 0, qsieve, qquant}; // qmodes;
enum	{TABQQ, TABQO, TABQS}; // tables;

	
// ************* class mask  :  *************
class quantizer {
	table		*q,*o,*s;				
	long		qmode;			//Modus: off,qsieve, qquant
	double	qv,ov,sv;
	int		dyn1,dyn2,dyn3,limit;

public:
				//quantizer(long l1=1, long l2=1, long l3=1);				 
				~quantizer(void); 	
				quantizer(void) {clear();}
	void		setval(long, double);
	void		setval(long, double*, double*, long);					 
	void		clear();
	void		setc(long, double, double);
	void		unsetc(long,long);
	void		setqmode(long a);				//Quant-Modus setzen
	void		setqlimit(int a);			//Limit-Flag setzen
	void		setipl(long c, double a=0.0, long b=0); //intpol
	void		setall(long a, double, double, double);
	double	getval(double, double, double, double);			
	};


#endif 