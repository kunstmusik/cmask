
#ifndef ITEMHEADER
#define ITEMHEADER

//#define QTMAX 50	// Maximale Anzahl Tabellenpunkte Quantizertabellen


enum	{CYCLE, SWING, RANDOM, HEAP}; //modes

	
// ************* class mask  :  *************
class itemlist {
	long	mode;			//Modus
	double	*elem;
	unsigned char *helem;
	long	ne;				//Anzahl
	long	ip;				//itempointer
	long	ih;				//heapzähler
	int		is;				//Richtung für Swing
	double	(itemlist::*f)(void);

	double	icycle(void);
	double	iswing(void);
	double	irandom(void);
	double	iheap(void);
	double	ifold(void);
	double	folder(void);

public:		
			itemlist(void);
			~itemlist(void); 	
	void	setval(double*,long);					 
	int		setmode(char *s); //Modus setzen
	double	getval(void);			
	};


#endif 