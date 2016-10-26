//#include "fileio.h"


#include "field.h"


#ifndef PARSHEADER
#define PARSHEADER



enum   {xnull, xfield, xpar, xrnd,
		xosc,
		xitem, xicycle, xiswing, xirandom, xiheap,
		xmask, xmap, xipl,
		xaccum,
		xbound, xbint, xbmode, xbinit,
		xquant,xqlimit,
		xconst, xrang, xseg,
		xprec, xoff,xcos, xnum }; // xsymbols; 


int myisnumber(char *s);
int ispfield(char *s);
int issymbol(char *s);
int	isliston(char *s);
int	islistoff(char *s);
int	ispairon(char *s);
int	ispairoff(char *s);

int xsymbol(char *s);
int xsymbmode (char *s);
int xsymimode (char *s);
int xsymipl (char *s);
 
	
class liste {
	struct knoten {
		char 	wert[16];
		struct knoten	*next;
		};
	long	n;
	struct knoten *end, *start, *actual;
public:
			liste();
			~liste();		
	void	append(const char *str);
	void	prepend(const char *str);
	void	cuthead(void);
	void	cuttail(void);
	void	clear(void);
	void	reset(void);
	void 	nth(char *str,long x);
	void 	readnext(char *str);
	long	len(void);
	};
	

class tokliste : public liste {
	char  	*t ;			//Zeiger auf aktuellen text
	long	tn;				//textlänge
	void	findnext(void); //lese nächstes Token & setze Zeiger
public:
			tokliste() : liste() {;};
			~tokliste() {;};		
	void	run(char*);
	long	findtok(long, char*);
 	}; 

	
class text {
	infile		filein;
	char		filename[MAX_FILENAME];
	long		n;					
public:
	char		*theText;			// Buffer für text
	
				text(void);
				~text(void);
	void		load(char*);				// laden
	long		len(void) {return n;}
	void		convertnewline(void);
	char		*savescoreheader(void);
	void		removecomments(void); 	// entfernt Kommentare
	};


	
class scanner {
	char		*scohdr;			// String für Scoreheader (ftable etc)
	tokliste	token;
	char 		tok[16];
	char 		*dummy;

	outfile		fileout;
	char		filename[MAX_FILENAME];
	long		nt,n;
	
	void		analyzefield(long);
	void		analyzeparam(paramliste&);
	void		calc(long, paramliste&, long);			// field berechnen und schreiben
public:
				scanner(void) {;}
				~scanner(void) {;}
	void		scn(char*, char*);   		// Lese Token in stringarray
	void		analyze(void);		// Analysiere und speichere Tabellen
	};

int lenlist(tokliste *tk, long s, int*);
void funlist(tokliste *tk, long s, long n, double *xa, double *ya, double *ip, long *im);
int lenpairlist(tokliste *tk, long s, int *ifl);
void pairlist(tokliste *tk, long s, long n, double beg, double end, double *xa,double *ya, double *ip, long *im);
void itmlist(tokliste *tk, long s, long n, double *xa);


#endif

