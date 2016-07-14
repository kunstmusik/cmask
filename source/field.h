#include "tables.h"
#include "gen.h"
#include "mask.h"
#include "quant.h"
#include "items.h"

#ifndef FIELDHEADER
#define FIELDHEADER

#define PARMAX 50

enum	{CONST = 1, RANGE, MASK, ACCUM, QUANT, GEN, ITEM};

// ************* class parameter  : mehrere Masken, Zeitwerte, Dichte *************

class parameter {
public:
	long		num;				// pfieldnumber
	mask		pmask;			// Maske für Parameter
	accum		paccum;			// Maske für Parameter
	gen		pgen;				// Generator für Parameter
	itemlist pitem;			// Itemliste für Parameter
	quantizer pquant;			// Quantizer für Parameter
	int		pprec;			// Precision für Parameter
	double	pconst;			// konstanter Wert
	int		pcflag;			// Flag für konstanten Wert
	int		pgflag, pmflag, paflag, pqflag, piflag;
				
				parameter(void);
	void		clear(void);
	void		setnum(long n) {num = n;}
	void 		setprec(long d);	
	void 		setconst(double x);
	void		setflag(int f);
	void		delflag(int f);
	void 		setrange(double x1, double x2);
	double 	getval(double);			// Errechnen eines Wertes bei Event e
};


class paramliste {
public:
	parameter 	*p;
	long			n;
	double		beginn, ende;

					paramliste(long pn=PARMAX);
					~paramliste();		
	};


#endif
