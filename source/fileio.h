#ifndef FILEHEADER
#define FILEHEADER

#include "globals.h"

class infile {

#if defined D_MAC
	SFReply		reply;
	SFTypeList	typeList;
	short int	fileRefNum;
	Point		where;
#endif	

	FILE		*fp;
	char		filename[MAX_FILENAME];
	OSErr		err;
	long		countBytes;			// Gesamtzahl Bytes beim Laden, Bytes für Speichern
	
public:
				infile(void);
				~infile(void) {;}
	void		open(void);			
	char		*read(void);		// Lade file
	void		close(void);		// Beende Text
	void		getname(char*);
	void		setname(char*);
	};

class outfile {
#if defined D_MAC
	SFReply		reply;
	short int	fileRefNum;
	Str63		pname;
	Point		where;
#endif	

	FILE		*fp;
	char		filename[MAX_FILENAME];
	OSErr		err;
	long		countBytes;			// Gesamtzahl Bytes beim Laden, Bytes für Speichern
	
public:
				outfile(void);
				~outfile(void);
	void		create(char*);
	void		write(char*);		// Schreibe in Textfile
	void		close(void);		// Beende Text
	void		setname(char*);
	};

#endif