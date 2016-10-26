// Laden Ÿber Standard dialog

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <iostream>
#include <cctype>
#include <cerrno>

#include "globals.h"

#if defined D_MAC
	#include <console.h>
#endif	

#include "gen.h"
#include "event.h"
#include "mask.h"
#include "tables.h"
#include "utils.h"
#include "fileio.h"
#include "parser.h"


int	isnumber(char *s)
	{
	long i,n;
	int	erg;
	
	erg = 1;
	n = strlen(s);
	for(i=0;  i<n; i++)
		if (!(isdigit(s[i]) || (s[i] == '-') || (s[i] == '+') || (s[i] == '.')))
			erg = 0;
	return erg;
	}		
	
int isliston(char *s)
	{
	int erg;
	erg = 0;
	if(s[0]=='(') erg = 1;
	return erg;
	}	

int islistoff(char *s)
	{
	int erg;
	erg = 0;
	if(s[0]==')') erg = 1;
	return erg;
	}	

int ispairon(char *s)
	{
	int erg;
	erg = 0;
	if(s[0]=='[') erg = 1;
	return erg;
	}	

int ispairoff(char *s)
	{
	int erg;
	erg = 0;
	if(s[0]==']') erg = 1;
	return erg;
	}	

int	ispfield(char *s)
	{
	long i,n;
	int	erg;
	
	erg = 1;
	n = strlen(s);
	if (tolower(s[0]) != 'p') erg = 0;
	for(i=1;  i<n; i++)
		if (!(isdigit(s[i])))
			erg = 0;
	return erg;
	}	
	
int	issymbol(char *s)
	{
	long i,n;
	int	erg;
	
	erg = 1;
	n = strlen(s);
	for(i=0;  i<n; i++)
		if (!(isalpha(s[i])))
			erg = 0;
	return erg;
	}		
	

int	xsymbol(char *s)
	{
	int erg,num;
	char *dummy="";
	
	erg = -1;
	switch (tolower(s[0]))
		{
		case 'a' :  erg = xaccum; break;
		case 'f' : 	erg = xfield; break;
		case 'l' : 	erg = xqlimit; break;
		case 'c' : 	erg = xconst; break;
		case 'r' : 	switch (tolower(s[1]))
							{
							case 'a' : erg = xrang; break;
							case 'n' : erg = xrnd; break;
							};
					break;
		case 'i' : 	switch (tolower(s[1]))
							{
							case 'p' : erg = xipl; break;
							case 'n' : erg = xbinit; break;
							case 't' : erg = xitem; break;
							};
					break;		
		case 's' : 	erg = xseg; break;
		case 'p' : 	if (isdigit(s[1]))
						{
						num = (long) strtod(s+1,&dummy);
						if ((num >= 1) && (num <=PARMAX)) erg = xpar;
						}
					else	
						switch (tolower(s[1]))
								{
								case 'r' : 	switch (tolower(s[2]))
												{
												case 'e' : erg = xprec; break;
												};
												break;
								};
					break;							
		case 'm' : 	switch (tolower(s[1]))
						{
						case 'a' : 	switch (tolower(s[2]))
											{
											case 'p' : erg = xmap; break;
											case 's' : erg = xmask; break;
										};
										break;
						};
					break;							
		case 'q' : 	switch (tolower(s[1]))
						{
						case 'u' : 	erg = xquant; break;
						};
					break;	
		case 'o' : 	erg = xosc; break;
					break;							
		default :	erg = -1;									
		}
	return erg;
	}		

int	xsymipl(char *s)
	{
	int erg;
	
	if ((isdigit(s[0])) || (s[0] == '-') || (s[0] == '+') || (s[0] == '.')) erg = xnum;
	else if (isalpha(s[0]))
			{
			if (tolower(s[0]) == 'o') erg = xoff;
			if (tolower(s[0]) == 'c') erg = xcos;
			}
	else erg = -1;		
	return erg;
	}

int	xsymbmode(char *s)
	{
	int erg;
	
	switch (tolower(s[0]))
		{
		case 'a' : 	erg = bakku; break;
		case 'w' : 	erg = bwrap; break;
		case 'm' : 	erg = bmirror; break;
		case 'l' : 	erg = blimit; break;
		case 'o' : 	erg = boff; break;
		default	 :	erg = -1; 
		}
	return erg;
	}
	
int	xsymimode(char *s)
	{
	int erg;
	
	switch (tolower(s[0]))
		{
		case 'c' : 	erg = CYCLE; break;
		case 's' : 	erg = SWING; break;
		case 'r' : 	erg = RANDOM; break;
		case 'h' : 	erg = HEAP; break;
		default	 :	erg = -1; 
		}
	return erg;
	}
	

int lenlist(tokliste *tk, long s, int *ifl)
	{
	int i;
	char to[16]="";
	
	i = s;
	*ifl = 0;
	tk->nth(to,i);
	while(!islistoff(to)) 
		{
		tk->nth(to,++i);
		if (xsymbol(to) == xipl) *ifl=1;
		}
	return (int)(i-s);
	}

void funlist(tokliste *tk, long s, long n, double *xa, double *ya, double *ip, long *im)
	{
	long i, ni;
	char *dum = "";
	char to[16];

	*ip = 0.0;
	*im = 1;
	ni = n;
	tk->nth(to,s+n-2);
	if (xsymbol(to) == xipl) 
		{
		ni = n-2;
		tk->nth(to,s+n-1);
		if (xsymipl(to) == xoff)
			{
			*im = 0;
			*ip = 0.0;
			}
		else if (xsymipl(to) == xcos)
			{
			*im = 2;
			*ip = 0.0;
			}
	
		else {
			*im = 1;
			//tk->nth(to,s+n-1);
			*ip = strtod(to,&dum);
			if(errno) error (SYNTAX,"invalid interpolation exponent in list");
			}
		}	
	for(i=0; i<ni; i += 2)
		{
		tk->nth(to,s+i);
		xa[i/2] = strtod(to,&dum);
		if(errno) error (SYNTAX,"invalid time in list");
		tk->nth(to,s+i+1);
		ya[i/2] = strtod(to,&dum);
		if(errno) error (SYNTAX,"invalid value in list");
		}
	}	
	
int lenpairlist(tokliste *tk, long s, int *ifl)
	{
	int i;
	char to[16]="";
	
	i = s;
	*ifl = 0;
	tk->nth(to,i);
	while(!ispairoff(to)) 
		{
		tk->nth(to,++i);
		if (xsymbol(to) == xipl) *ifl=1;
		}
	if(((*ifl == 1) && ((i-s) != 4))	|| ((*ifl == 0) && ((i-s) != 2)))
		 error (SYNTAX,"invalid number of []-list values");
	return (int)(i-s);
	}

void pairlist(tokliste *tk, long s, long n, double beg, double end,double *xa, double *ya, double *ip, long *im)
	{
	char *dum = "";
	char to[16];

	*ip = 0.0;
	*im = 1;
	if (n == 4)
		{
		tk->nth(to,s+3);
		if (xsymipl(to) == xoff)
			{
			*im = 0;
			*ip = 0.0;
			}
		else {
			*im = 1;
			*ip = strtod(to,&dum);
			if(errno) error (SYNTAX,"invalid interpolation exponent in list");
			}
		}	
	xa[0] = beg;
	xa[1] = end;
	tk->nth(to,s);
	ya[0] = strtod(to,&dum);
	if(errno) error (SYNTAX,"invalid value in list");
	tk->nth(to,s+1);
	ya[1] = strtod(to,&dum);
	if(errno) error (SYNTAX,"invalid value in list");
	}	


void itmlist(tokliste *tk, long s, long n, double *xa)
	{
	long i, ni;
	char *dum = "";
	char to[16];

	ni = n;
	for(i=0; i<ni; i++)
		{
		tk->nth(to,s+i);
		xa[i] = strtod(to,&dum);
		if(errno) error (SYNTAX,"invalid value in item list");
		}
	}	


// liste - functions ----------------------------------------

liste::liste()
	{
	n = 0;
	start = new knoten;
	if(!start) error(MEMORY, "list");
	end = new knoten;
	if(!end) error(MEMORY, "list");
	start->next = end;
	end->next = end;
	actual = start;
	}
	
liste::~liste()
	{
	clear();
	}	
		
void liste::clear(void)
	{
	actual = start;
	while (actual != end)
		{
		start = actual;
		actual = actual->next;
		delete start;
		}
	n = 0;	
	}		

void liste::append(char *str)	//fŸge vor end ein
	{
	if (strlen(str) > 16)  error(STRING, str);
	actual = end;
	strcpy(actual->wert,str);
	end = new knoten;
	if(!end) error(MEMORY, "list");
	actual->next = end;
	end->next = end;
	n++;
	}
	
void liste::prepend(char *str)	//fŸge vor start ein
	{
	if (strlen(str) > 16)  error(STRING, str);
	actual = new knoten;
	if(!actual) error(MEMORY, "list");
	strcpy(actual->wert,str);
	actual->next = start;
	start = actual;
	n++;
	}

void liste::cuthead(void)
	{
	if(start->next != end)
		{
		actual = start;
		start = start->next;
		delete actual;
		n--;
		}
	}
	
void liste::cuttail(void)
	{
	;
	}		 	

void liste::reset(void)
	{
	actual = start->next;
	}

void liste::nth(char *str,long x)
	{
	long i, k;
	
	reset();
	if(x>=n)
		{
		strcpy(str,"\0\0\0");		
		strcpy(str,actual->wert);
		}
	else
		{ 
		//k = (x < n) ? x : n;
		if (x)
			for (i=0; i<x; i++)
				actual = actual->next;
		strcpy(str,"               ");		
		strcpy(str,actual->wert);
		}
	}

void liste::readnext(char *str)
	{
	strcpy(str,actual->wert);
	//actual = actual->next;
	}
	
long liste::len(void)	
	{
	return n-1;
	}
	
/*
void liste::next(void)
	{
	actual = actual->next;
	}
*/	
// liste - functions ----------------------------------------

			
// tokliste - functions ----------------------------------------

	
void tokliste::findnext(void)
	{
	char *in,dummy[16];
	long ln;
	char *delims = " ,;\n\t\r\f" ;

	in = t + strspn(t,delims);	// erster Nichttrenner
	if(in > (t+tn)) return;
	ln = strcspn(in,delims); 	// nŠchster Trenner
	strncpy(dummy,in, ln);  
	dummy[ln]='\0';
	
	//Was wird mit den Klammern ?
	
	if(dummy[0]=='(')
		{
		append("(");
		if(strlen(dummy) > 1)
			append(dummy+1);
		}
	else if(dummy[0]=='[')
		{
		append("[");
		if(strlen(dummy) > 1)
			append(dummy+1);
		}
	else if(dummy[ln-1]==')')
		{
		if(strlen(dummy) > 1)
			{
			dummy[ln-1]='\0';
			append(dummy);
			}
		append(")");
		}
	else if(dummy[ln-1]==']')
		{
		if(strlen(dummy) > 1)
			{
			dummy[ln-1]='\0';
			append(dummy);
			}
		append("]");
		}
	else append(dummy);
	t = in+ln;
	}
		
void tokliste::run (char *str)
	{
	char *tend;
	
	t = str;
	tn = strlen(t);
	tend = t + tn;
	while (t < tend) findnext();
	}

long tokliste::findtok (long x, char *str)
	{
	long i;
	char dummy[16];
	
	i = 0;
	nth(dummy, x+i);
	while (strcmp(dummy, str) != 0) i++;
	return i;
	}



// tokliste - functions ----------------------------------------


// text - functions ----------------------------------------
	
text::text() : filein()
	{	
	theText = NULL;
	n = 0;
	}

text::~text() {
	delete[] theText;
	}
	
#if defined D_MAC 
void text::load(char *str)	
	{
	filein.open();
	theText=filein.read();
	filein.getname(filename);
	filein.close();
	n = strlen(theText);
	strcpy(str, filename);
	}
#endif

#if defined D_IRIX 
void text::load(char *strg)	
	{
	//strcpy(filename,inname);
	filein.setname(strg);
	filein.open();
	theText=filein.read();
	//filein.getname(filename);
	filein.close();
	n = strlen(theText);
	}
#endif	

void text::convertnewline()
	{
	long i;

	for(i=0; i < n; i++)
#if defined D_MAC	
		if( ((int) theText[i]) == 10) theText[i] = (char)13;
#elif defined D_IRIX
		if( ((int) theText[i]) == 13) theText[i] = (char)10;
#endif		
	}	

char *text::savescoreheader(void)
	{
	char *in, *out,*str;
	long lh,lr;

	if (n)
		{	
		in = strchr(theText,'{');
		if (in) 
			{
			out = strchr(in+1,'}');
			lh = out-in-1;
			lr = strlen(out);
			str = new char[lh+1];
			if(!str) error(MEMORY, "string");
			memcpy(str,in+1,lh);
			strcpy(str+lh,NL);
			memmove(in,out+1,lr);
			n -= lh;
			return str;
			}
		else return NULL;	
		}
	else return NULL;	
	}	

void text::removecomments(void)
	{
	char *in, *out;
	long l,d;
	
	if(n)
		{
		in = strchr(theText,';');
		while (in) 
			{
			d = strcspn(in,"\r\n");
			out = in + d ;
			l = strlen(out);
			memmove(in,out,l+1);
			in = strchr(theText,';');
			}
		}	
	}	
	
// text - functions ----------------------------------------

// scanner - functions ----------------------------------------

void scanner::scn(char *instring, char *outstring)
	{
	text	tx;
	long	i;
	char	str[16]; 
	
	//tx = new text;
#if defined D_MAC
	tx.load(filename);	//Name wird durch tx.load in filename gespeichert
#endif 	
#if defined D_IRIX
	tx.load(instring);	//Name wird durch tx.load Ÿbergeben
#endif 	
	tx.convertnewline();
	//tx.savescoreheader(scohdr);
	scohdr=tx.savescoreheader();
	//strcpy(scohdr,tx.savescoreheader());
	//cout << "\nheader" << scohdr << endl;
	tx.removecomments();
	//cout << "\ntext:" << tx.theText << endl;
	if(tx.len()) token.run(tx.theText);
	nt=token.len() - 1;   // <------- ?????
#if defined D_MAC
	fileout.create(filename);
#endif 	
#if defined D_IRIX
	fileout.create(outstring);	
#endif 	
	if(scohdr) fileout.write(scohdr);
	
	//token.reset();
	/*
	for (i=0; i < nt; i++)
		{
		token.nth(str,i);
		cout << i << ": " << str << endl;
		}
	*/
	//delete tx;
	}

void scanner::analyze(void)
	{
	long fn;
	
	n=0;
	fn=0;
	token.nth(tok,n);
	while(n<nt)
		{
		if (xsymbol(tok) == xfield)
			{
			fn++;
			cout << "\n===== Field: " << fn << " ==================\n\n";
			analyzefield(fn);
			//cout << "nt : " << nt << ", n : " << n << endl;
			}
		else error (SYNTAX,"no field header or invalid character");	
		}
	fileout.close();	
	}	
	
	
void scanner::analyzefield(long fn)
	{
	double beginn,ende;
	paramliste	p;
	long npar;

//	f.clear();
	errno=0;
	 								
	token.nth(tok,++n);				// beginn
	beginn = strtod(tok,&dummy);
	if((beginn < 0.0) || errno)  error (SYNTAX,"invalid number for begin");	
									
	token.nth(tok,++n);				// ende
	ende = strtod(tok,&dummy);
	if(!ende || (ende < beginn))  error (SYNTAX,"invalid number for end");	
	cout << "Duration from " << beginn << " to " << ende << '\n';
	p.beginn=beginn;
	p.ende=ende;
	
	token.nth(tok,++n);
	cout << "\npfields: ";
	
	npar = 0;
	while (xsymbol(tok) == xpar) 
		{
		npar++;
		analyzeparam(p); 
		}
	calc(fn, p, npar);
	//cout << "calc ?" << endl;
	}
	
void scanner::analyzeparam(paramliste& p)	
	{
	double x,y1,map, iplval;
	double *fx, *fy;
	long k,pnum,r,i,ix,iy, iplmode;
	int	ifl;

	pnum = (long) strtod(tok+1,&dummy);
	if(!pnum) error (SYNTAX,"no pfield number");
	if(pnum < 1) error (SYNTAX,"invalid pfield number");
	//cout << "\n----- Pfield number: " << pnum << " ----------\n\n";
	cout << "p" << pnum << " ";
	//f.setpnum(m,pnum);	
	
	token.nth(tok,++n);
	while (n<nt && (xsymbol(tok) != xpar) && (xsymbol(tok) != xfield) )
		switch (xsymbol(tok))
			{
			case xconst :	// pfield mit Konstante-Makro
				token.nth(tok,++n);
				x = strtod(tok,&dummy);
				if(errno) error (SYNTAX,"no constant value");
				p.p[pnum].setconst(x);
				token.nth(tok,++n);
				break;
			case xrang	:	// pfield mit Range-Makro
				token.nth(tok,++n);
				x = strtod(tok,&dummy);
				if(errno) error (SYNTAX,"no lower range value");
				token.nth(tok,++n);
				y1 = strtod(tok,&dummy);
				if(errno) error (SYNTAX,"no upper range value");
				p.p[pnum].setrange(x,y1);
				p.p[pnum].setflag(RANGE);
				p.p[pnum].setflag(GEN);
				token.nth(tok,++n);
				break;
				
			case xseg	:	// pfield mit Segment-Makro
				token.nth(tok,++n);
				if(isnumber(tok))
					error (SYNTAX,"only list allowed for segment");
				else if(ispairon(tok))
					{
					n++;
					ix = lenpairlist(&token,n, &ifl);
					fx = new double[2];
					fy = new double[2];
					pairlist(&token,n, ix,p.beginn, p.ende,fx,fy,&iplval,&iplmode);
					p.p[pnum].pmask.setval(1,fx,fy,2);
					p.p[pnum].pmask.setipl(1,iplval,iplmode);
					p.p[pnum].pmask.setval(2,fx,fy,2);
					p.p[pnum].pmask.setipl(2,iplval,iplmode);
					p.p[pnum].setflag(MASK);
					p.p[pnum].delflag(GEN);
					delete fx;
					delete fy;
					n += ix;
					token.nth(tok,++n);
					}
				else if(isliston(tok))
					{
					n++;
					ix = iy = lenlist(&token,n, &ifl);
					if(ifl) iy = ix-2;
					fx = new double[iy/2];
					fy = new double[iy/2];
					funlist(&token,n, ix,fx,fy,&iplval,&iplmode);
					p.p[pnum].pmask.setval(1,fx,fy,iy/2);
					p.p[pnum].pmask.setipl(1,iplval,iplmode);
					p.p[pnum].pmask.setval(2,fx,fy,iy/2);
					p.p[pnum].pmask.setipl(2,iplval,iplmode);
					p.p[pnum].setflag(MASK);
					p.p[pnum].delflag(GEN);
					delete fx;
					delete fy;
					n += ix;
					token.nth(tok,++n);
					}
				break;


			case xprec	:	// precision
				token.nth(tok,++n);
				k = atoi(tok);
				if(errno || (k < 0)) error (SYNTAX,"invalid precision");	
				p.p[pnum].setprec(k);
				token.nth(tok,++n);
				break;	
				
			case xitem	:	// itemlist
				p.p[pnum].setflag(ITEM);
				p.p[pnum].delflag(GEN);
				p.p[pnum].delflag(MASK);
				token.nth(tok,++n);
				r = p.p[pnum].pitem.setmode(tok);		
				if(r == -1) error (SYNTAX,"no valid item mode");
				token.nth(tok,++n);
				if(isliston(tok))
					{
					n++;
					ix = iy = lenlist(&token,n, &ifl);
					fx = new double[ix];
					itmlist(&token,n, ix,fx);
					p.p[pnum].pitem.setval(fx,ix);
					delete fx;
					n += ix;
					token.nth(tok,++n);
					}
				else error (SYNTAX,"no valid item list");
				break;
				
			case xrnd	:	// prob.function
				token.nth(tok,++n);
				p.p[pnum].pgen.settyp(genrnd);
				r = p.p[pnum].pgen.setfunc(tok);	
				if(!r) error (SYNTAX,"invalid probability function");
				p.p[pnum].setflag(GEN);
				// erster Wert optional
				token.nth(tok,++n);
				if(isnumber(tok))
					{
					 x = strtod(tok,&dummy);
					if(errno) error (SYNTAX,"invalid value for control A");
					p.p[pnum].pgen.setval(1,x);
					}
				else if(ispairon(tok))
					{
					n++;
					ix = lenpairlist(&token,n, &ifl);
					fx = new double[2];
					fy = new double[2];
					pairlist(&token,n, ix,p.beginn, p.ende,fx,fy,&iplval,&iplmode);
					p.p[pnum].pgen.setval(1,fx,fy,2);
					p.p[pnum].pgen.setipl(1,iplval,iplmode);
					delete fx;
					delete fy;
					n += ix;
					}
				else if(isliston(tok))
					{
					n++;
					ix = iy = lenlist(&token,n, &ifl);
					if(ifl) iy = ix-2;
					fx = new double[iy/2];
					fy = new double[iy/2];
					funlist(&token,n, ix,fx,fy,&iplval,&iplmode);
					p.p[pnum].pgen.setval(1,fx,fy,iy/2);
					p.p[pnum].pgen.setipl(1,iplval,iplmode);
					delete fx;
					delete fy;
					n += ix;
					}
				else break;
				// zweiter Wert optional
				if(n<nt)
					{
					token.nth(tok,++n);
					if(isnumber(tok))
						{
						x = strtod(tok,&dummy);
						if(errno) error (SYNTAX,"invalid value for control A");
						p.p[pnum].pgen.setval(2,x);
						token.nth(tok,++n);
						}
					else if(ispairon(tok))
						{
						n++;
						ix = lenpairlist(&token,n, &ifl);
						fx = new double[2];
						fy = new double[2];
						pairlist(&token,n, ix,p.beginn, p.ende,fx,fy,&iplval,&iplmode);
						p.p[pnum].pgen.setval(2,fx,fy,2);
						p.p[pnum].pgen.setipl(2,iplval,iplmode);
						delete fx;
						delete fy;
						n += ix;
						}
					else if(isliston(tok))
						{
						n++;
						ix = iy = lenlist(&token,n, &ifl);
						if(ifl) iy = ix-2;
						fx = new double[iy/2];
						fy = new double[iy/2];
						funlist(&token,n, ix,fx,fy,&iplval,&iplmode);
						p.p[pnum].pgen.setval(2,fx,fy,iy/2);
						p.p[pnum].pgen.setipl(2,iplval,iplmode);
						delete fx;
						delete fy;
						n += ix;
						token.nth(tok,++n);
						}
					}	
					break;	


			case xosc	:	// osc.function
				token.nth(tok,++n);
				p.p[pnum].pgen.settyp(genosc);
				r = p.p[pnum].pgen.setfunc(tok);	
				if(!r) error (SYNTAX,"invalid oscillator function");
				p.p[pnum].setflag(GEN);
				// erster Wert optional
				token.nth(tok,++n);
				if(isnumber(tok))
					{
					 x = strtod(tok,&dummy);
					if(errno) error (SYNTAX,"invalid frequency value");
					p.p[pnum].pgen.setval(1,x);
					}
				else if(ispairon(tok))
					{
					n++;
					ix = lenpairlist(&token,n, &ifl);
					fx = new double[2];
					fy = new double[2];
					pairlist(&token,n, ix,p.beginn, p.ende,fx,fy,&iplval,&iplmode);
					p.p[pnum].pgen.setval(1,fx,fy,2);
					p.p[pnum].pgen.setipl(1,iplval,iplmode);
					delete fx;
					delete fy;
					n += ix;
					}
				else if(isliston(tok))
					{
					n++;
					ix = iy = lenlist(&token,n, &ifl);
					if(ifl) iy = ix-2;
					fx = new double[iy/2];
					fy = new double[iy/2];
					funlist(&token,n, ix,fx,fy,&iplval,&iplmode);
					p.p[pnum].pgen.setval(1,fx,fy,iy/2);
					p.p[pnum].pgen.setipl(1,iplval,iplmode);
					delete fx;
					delete fy;
					n += ix;
					}
				else break;
				// zweiter Wert optional
				token.nth(tok,++n);
				if(isnumber(tok))
					{
					x = strtod(tok,&dummy);
					if(errno) error (SYNTAX,"invalid phase value");
					p.p[pnum].pgen.setval(2,x);
					}
				else break;	
				// dritter Wert optional
				token.nth(tok,++n);
				if(isnumber(tok))
					{
					x = strtod(tok,&dummy);
					if(errno) error (SYNTAX,"invalid exponent value");
					p.p[pnum].pgen.setval(3,x);
					token.nth(tok,++n);
					}
				break;	
				
			case xmask	:	// mask
				// erster Wert obligatorisch
				p.p[pnum].setflag(MASK);
				p.p[pnum].setflag(GEN);
				token.nth(tok,++n);
				if(isnumber(tok))
					{
					 x = strtod(tok,&dummy);
					if(errno) error (SYNTAX,"invalid value for lower mask boundary");
					p.p[pnum].pmask.setval(1,x);
					}
				else if(ispairon(tok))
					{
					n++;
					ix = lenpairlist(&token,n, &ifl);
					fx = new double[2];
					fy = new double[2];
					pairlist(&token,n, ix,p.beginn, p.ende,fx,fy,&iplval,&iplmode);
					p.p[pnum].pmask.setval(1,fx,fy,2);
					p.p[pnum].pmask.setipl(1,iplval,iplmode);
					delete fx;
					delete fy;
					n += ix;
					}
				else if(isliston(tok))
					{
					n++;
					ix = iy = lenlist(&token,n, &ifl);
					if(ifl) iy = ix-2;
					fx = new double[iy/2];
					fy = new double[iy/2];
					funlist(&token,n, ix,fx,fy,&iplval,&iplmode);
					p.p[pnum].pmask.setval(1,fx,fy,iy/2);
					p.p[pnum].pmask.setipl(1,iplval,iplmode);
					delete fx;
					delete fy;
					n += ix;
					}
				else error (SYNTAX,"invalid value for lower mask boundary");
				// zweiter Wert obligatorisch
				token.nth(tok,++n);
				if(isnumber(tok))
					{
					x = strtod(tok,&dummy);
					if(errno) error (SYNTAX,"invalid value for higher mask boundary");
					p.p[pnum].pmask.setval(2,x);
					}
				else if(ispairon(tok))
					{
					n++;
					ix = lenpairlist(&token,n, &ifl);
					fx = new double[2];
					fy = new double[2];
					pairlist(&token,n, ix,p.beginn, p.ende,fx,fy,&iplval,&iplmode);
					p.p[pnum].pmask.setval(2,fx,fy,2);
					p.p[pnum].pmask.setipl(2,iplval,iplmode);
					delete fx;
					delete fy;
					n += ix;
					}
				else if(isliston(tok))
					{
					n++;
					ix = iy = lenlist(&token,n, &ifl);
					if(ifl) iy = ix-2;
					fx = new double[iy/2];
					fy = new double[iy/2];
					funlist(&token,n, ix,fx,fy,&iplval,&iplmode);
					p.p[pnum].pmask.setval(2,fx,fy,iy/2);
					p.p[pnum].pmask.setipl(2,iplval,iplmode);
					delete fx;
					delete fy;
					n += ix;
					}
				else error (SYNTAX,"invalid value for higher mask boundary");
				// Mapping Wert optional
				token.nth(tok,++n);
				if(xsymbol(tok) == xmap)
					{
					token.nth(tok,++n);			
					map = strtod(tok,&dummy);
					if(errno) error (SYNTAX,"no mapping exponent");
					p.p[pnum].pmask.setmap(map);	
					token.nth(tok,++n);
					}
				break;	

	
			case xquant	:	// quant.function
				// erster Wert
				p.p[pnum].setflag(QUANT);
				token.nth(tok,++n);
				if(isnumber(tok))
					{
					 x = strtod(tok,&dummy);
					if(errno) error (SYNTAX,"invalid quantization interval");
					p.p[pnum].pquant.setval(1,x);
					p.p[pnum].pquant.setval(3,1.0);
					p.p[pnum].pquant.setval(2,0.0);
					}
				else if(ispairon(tok))
					{
					n++;
					ix = lenpairlist(&token,n, &ifl);
					fx = new double[2];
					fy = new double[2];
					pairlist(&token,n, ix,p.beginn, p.ende,fx,fy,&iplval,&iplmode);
					p.p[pnum].pquant.setval(1,fx,fy,2);
					p.p[pnum].pquant.setipl(1,iplval,iplmode);
					p.p[pnum].pquant.setval(3,1.0);
					p.p[pnum].pquant.setval(2,0.0);
					delete fx;
					delete fy;
					n += ix;
					}
				else if(isliston(tok))
					{
					n++;
					ix = iy = lenlist(&token,n, &ifl);
					if(ifl) iy = ix-2;
					fx = new double[iy/2];
					fy = new double[iy/2];
					funlist(&token,n, ix,fx,fy,&iplval,&iplmode);
					p.p[pnum].pquant.setval(1,fx,fy,iy/2);
					p.p[pnum].pquant.setipl(1,iplval,iplmode);
					p.p[pnum].pquant.setval(3,1.0);
					p.p[pnum].pquant.setval(2,0.0);
					delete fx;
					delete fy;
					n += ix;
					}
				else error (SYNTAX,"invalid quantization interval");
				// zweiter Wert optional
				if(n<nt)
					{
					token.nth(tok,++n);
					if(isnumber(tok))
						{
						 x = strtod(tok,&dummy);
						if(errno) error (SYNTAX,"invalid quantization strength");
						p.p[pnum].pquant.setval(3,x);
						}
					else if(ispairon(tok))
						{
						n++;
						ix = lenpairlist(&token,n, &ifl);
						fx = new double[2];
						fy = new double[2];
						pairlist(&token,n, ix,p.beginn, p.ende,fx,fy,&iplval,&iplmode);
						p.p[pnum].pquant.setval(3,fx,fy,2);
						p.p[pnum].pquant.setipl(3,iplval,iplmode);
						delete fx;
						delete fy;
						n += ix;
						}
					else if(isliston(tok))
						{
						n++;
						ix = iy = lenlist(&token,n, &ifl);
						if(ifl) iy = ix-2;
						fx = new double[iy/2];
						fy = new double[iy/2];
						funlist(&token,n, ix,fx,fy,&iplval,&iplmode);
						p.p[pnum].pquant.setval(3,fx,fy,iy/2);
						p.p[pnum].pquant.setipl(3,iplval,iplmode);
						delete fx;
						delete fy;
						n += ix;
						}
				/*	else if(xsymbol(tok) == xqlimit)
						{
						p.p[pnum].pquant.setqlimit(1);
						//token.nth(tok,++n);
						break;
						}
				*/		
					else break;	
				}	
				// dritter Wert optional
				if(n<nt)
					{
					token.nth(tok,++n);
					if(isnumber(tok))
						{
						x = strtod(tok,&dummy);
						if(errno) error (SYNTAX,"invalid quantization offset");
						p.p[pnum].pquant.setval(2,x);
						token.nth(tok,++n);
						}
					else if(ispairon(tok))
						{
						n++;
						ix = lenpairlist(&token,n, &ifl);
						fx = new double[2];
						fy = new double[2];
						pairlist(&token,n, ix,p.beginn, p.ende,fx,fy,&iplval,&iplmode);
						p.p[pnum].pquant.setval(2,fx,fy,2);
						p.p[pnum].pquant.setipl(2,iplval,iplmode);
						delete fx;
						delete fy;
						n += ix;
						token.nth(tok,++n);
						}
					else if(isliston(tok))
						{
						n++;
						ix = iy = lenlist(&token,n, &ifl);
						if(ifl) iy = ix-2;
						fx = new double[iy/2];
						fy = new double[iy/2];
						funlist(&token,n, ix,fx,fy,&iplval,&iplmode);
						p.p[pnum].pquant.setval(2,fx,fy,iy/2);
						p.p[pnum].pquant.setipl(2,iplval,iplmode);
						delete fx;
						delete fy;
						n += ix;
						token.nth(tok,++n);
						}
					else break;	
				/*	else if(xsymbol(tok) == xqlimit)
						{
						p.p[pnum].pquant.setqlimit(1);
						//token.nth(tok,++n);
						}
				*/		
					}	
					
			/*	if(n<nt)
					{
					token.nth(tok,++n);
					if(xsymbol(tok) == xqlimit)
						{
						p.p[pnum].pquant.setqlimit(1);
						token.nth(tok,++n);
						break;
						}
					}
			*/			
				break;	
				
			case xaccum	:	
				// Modus obligatorisch
				p.p[pnum].setflag(ACCUM);
				token.nth(tok,++n);
				r = p.p[pnum].paccum.setbmode(tok);		
				if(r == -1) error (SYNTAX,"no valid accumulation mode");
				switch (r)
					{
					case bmirror	:
					case bwrap		:
					case blimit		:
							token.nth(tok,++n);
							if(isnumber(tok))
								{
								 x = strtod(tok,&dummy);
								if(errno) error (SYNTAX,"invalid value for lower accum boundary");
								p.p[pnum].paccum.setval(1,x);
								}
							else if(ispairon(tok))
								{
								n++;
								ix = lenpairlist(&token,n, &ifl);
								fx = new double[2];
								fy = new double[2];
								pairlist(&token,n, ix,p.beginn, p.ende,fx,fy,&iplval,&iplmode);
								p.p[pnum].paccum.setval(1,fx,fy,2);
								p.p[pnum].paccum.setipl(1,iplval,iplmode);
								delete fx;
								delete fy;
								n += ix;
								}
							else if(isliston(tok))
								{
								n++;
								ix = iy = lenlist(&token,n, &ifl);
								if(ifl) iy = ix-2;
								fx = new double[iy/2];
								fy = new double[iy/2];
								funlist(&token,n, ix,fx,fy,&iplval,&iplmode);
								p.p[pnum].paccum.setval(1,fx,fy,iy/2);
								p.p[pnum].paccum.setipl(1,iplval,iplmode);
								delete fx;
								delete fy;
								n += ix;
								}
							else error (SYNTAX,"invalid value for lower accum boundary");
							// zweiter Wert obligatorisch
							token.nth(tok,++n);
							if(isnumber(tok))
								{
								x = strtod(tok,&dummy);
								if(errno) error (SYNTAX,"invalid value for higher accum boundary");
								p.p[pnum].paccum.setval(2,x);
								}
							else if(ispairon(tok))
								{
								n++;
								ix = lenpairlist(&token,n, &ifl);
								fx = new double[2];
								fy = new double[2];
								pairlist(&token,n, ix,p.beginn, p.ende,fx,fy,&iplval,&iplmode);
								p.p[pnum].paccum.setval(2,fx,fy,2);
								p.p[pnum].paccum.setipl(2,iplval,iplmode);
								delete fx;
								delete fy;
								n += ix;
								//cout << "akkuliste!" << endl;
								}
							else if(isliston(tok))
								{
								n++;
								ix = iy = lenlist(&token,n, &ifl);
								if(ifl) iy = ix-2;
								fx = new double[iy/2];
								fy = new double[iy/2];
								funlist(&token,n, ix,fx,fy,&iplval,&iplmode);
								p.p[pnum].paccum.setval(2,fx,fy,iy/2);
								p.p[pnum].paccum.setipl(2,iplval,iplmode);
								delete fx;
								delete fy;
								n += ix;
								//cout << "akkuliste: n: " << n << ", nt: " << nt << endl;
								}
							else error (SYNTAX,"invalid value for higher accum boundary");
					case bakku		:			// Start Wert optional
							token.nth(tok,++n);
							if(xsymbol(tok) == xbinit)
								{
								token.nth(tok,++n);	
								x = strtod(tok,&dummy);
								if ((pnum == 2) && (x != 0.0)) error (SYNTAX,"for p2: only init value 0 allowed!");		
								if(errno) error (SYNTAX,"no accum init value");
								p.p[pnum].paccum.initakku(x);	
								token.nth(tok,++n);
								}
							break;	
					case boff		: 	
							p.p[pnum].delflag(ACCUM);
							token.nth(tok,++n);
							break;
					}
				break;	
	
	
	
			default :   
				cout << "invalid token: " << tok << '\n';
				token.nth(tok,++n);
				break;			
			} 	// switch end	
		} 		// while par end


// ------------------
			
void	scanner::calc(long fn, paramliste& p, long npar)
	{			
	long  ne, cnt,k,i;
	double xt,x;
	char outstr[1024];
	char dummy[1024];
	char pstr[5];
	
	strcpy(outstr," ");
	
	event e(npar);
	//cout << "\n--> " << m+1 << " parameters in field " << fn << endl;
	
	cout << "\n\n";
	xt = p.beginn;
	ne = 0;
	cnt = 0;
	//dur = ende-beginn;
	
	//for(k=0; k< pn; k++) f.initakku(k);
	
	sprintf(dummy,";ins\ttime\tdur");
	for(i=4; i<=npar; i++)
		{
		sprintf(pstr,"\tp%ld",i);
		strcat(dummy,pstr);
		}
	sprintf(outstr, "%s; ------- begin of field %ld --- seconds: %.2f - %.2f --------%s%s%s%s",NL,fn,p.beginn,p.ende,NL,dummy,NL,NL);
	fileout.write(outstr);
	//cout << "\n--> beg: " << beginn << ", end: " << ende << endl;
	//f.setbmode(1,boff);
	while (xt < p.ende)
		{
		e.setop('i');
		x = p.p[1].getval(xt);	//variables Instrument
		e.setp(1, (x < 1.0 ? 1 : round(x,0))) ; 
		e.setp(2,xt);			//einsatzzeit
		for(k=3; k <= npar; k++)
			e.setp(k,p.p[k].getval(xt));
		e.get(outstr);
		fileout.write(outstr);

		
		//cout << "Zeit: " << xt << endl;
		x = p.p[2].getval(xt);	
		if (x == 0) 
			cout << "\nWarning: p2 = 0 !\n" ;	
		xt += x;	
		ne++;
		
		cnt++;
		if (cnt==100) 
			{
			cnt = 0;
			cout << '.' << flush;
			} 
		}
	
	sprintf(outstr, "%s; ------- end of field %ld --- number of events: %ld -------%s",NL,fn,ne,NL);
	fileout.write(outstr);
	
	cout << "\n" << ne << " events generated.\n" << endl;
	//sprintf(outstr,"s\n");
	//countBytes = strlen(outstr);
	//write();	
	}
