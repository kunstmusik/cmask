// Laden Ÿber Standard dialog
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <iostream>
#include <cerrno>

#include "globals.h"

#if defined D_MAC
	#include <console.h>
#endif

#include "fileio.h"
#include "utils.h"


// infile - functions ----------------------------------------

infile::infile(void) 
	{
	
#if defined D_MAC	
	where.h = where.v = 100;
#endif	

	countBytes = 0;
	err = 0;
	}

void infile::setname(char *stir) 
	{ 
	//printf("infile::setname : %s \n",stir);
	strcpy(filename,stir);
	}
		

void infile::getname(char *str) 
	{ 
	strcpy(str,filename);
	}

void infile::open(void)
	{

#if defined D_MAC
	typeList[0] = 'TEXT'; 
	SFGetFile(where,NULL,NULL,1,typeList,NULL,&reply);
	if (reply.good)
		{	
		err = FSOpen(reply.fName, reply.vRefNum, &fileRefNum);
		if (err) error(FILERR,"opening parameter file.",err);
		err = GetEOF (fileRefNum, &countBytes);
		if (err) error(FILERR,"get EOF parameter file.",err);
		//printf ("\nGrš§e %ld Bytes", countBytes);
		PStrtoCStr(filename,reply.fName);
		}
#endif

#if defined D_IRIX
	long i;
	
	fp = fopen(filename,"r");
	if(fp == NULL)
		{
       	error(FILERR," opening parameter file",1);
		}
		
	for(i=0; feof(fp) == 0; i++, fgetc(fp));
	countBytes = i-1;	
	rewind(fp);

#endif
	cout << "\nparameter file: " << filename << '\n';	
		
	}

void infile::close(void)
	{

#if defined D_MAC
	err = FSClose (fileRefNum);
	err = FlushVol (NULL, reply.vRefNum);
#endif

#if defined D_IRIX	
	err = fflush (fp);
	err = fclose (fp);
#endif

	if (err) error(FILERR,"closing parameterfile file",err);
	}
	
char *infile::read(void)	
	{
	char	*text;
	long	test;
	
	text = new char[countBytes+1];
	if (text)
		{
#if defined D_MAC
		err = FSRead (fileRefNum, &countBytes, text);
		if (err) error(FILERR,"reading parameter file.",err);
#endif

#if defined D_IRIX
		test = fread(text,sizeof(char),countBytes,fp);
		if (test != countBytes) 
			error(FILERR,"reading parameter file.",1);
#endif
		text[countBytes] = '\0';
		return text;
		}
	else 
		{
		error (MEMORY,"parameter text");
		return NULL;
		}
	}

// infile - functions ----------------------------------------

// outfile - functions ----------------------------------------

outfile::outfile(void) 
	{

#if defined D_MAC
	where.h = where.v = 100;
#endif
	
	countBytes = 0;
	err = 0;
	}
		
outfile::~outfile(void) {;}

void outfile::setname(char *str) { strcpy(filename,str);}

void outfile::create(char *str)
	{
	int i,k;
	char *dummy1, dummy2[MAX_FILENAME];

	for (i=0; i<MAX_FILENAME; dummy2[i]='\0', i++);
	setname(str);

//	cout << "Score file: " << filename << endl;	

	i = strlen(filename);
	k = strcmp(filename+i-4,".sco");
	if (k)
		{
		dummy1 = strchr(filename,'.');
		if(dummy1)
			{
			i = dummy1 -filename;
	//		cout << "i= " << i << endl;
			strncpy(dummy2,filename,i);
	//		cout << "dummy2: " << dummy2 << "| file: " << filename << endl;
			setname(dummy2);
	//		cout << " file: " << filename << "|" << endl;
			strcat(filename,".sco");
	//		cout << " file: " << filename << "|" << endl;
			}
		else	
			strcat(filename,".sco");
		}
	
#if defined D_MAC	
	CStrtoPStr(pname,filename);
	SFPutFile(where,"\pSave Csound score as:", pname,NULL,&reply);
	if (reply.good)
		{
		err = FSDelete (reply.fName, reply.vRefNum);
		err = Create (reply.fName, reply.vRefNum, 'CMSK', 'TEXT');
		err = FSOpen (reply.fName, reply.vRefNum, &fileRefNum);
		if (err) error(FILERR,"creating score file",err);
		}
#endif

#if defined D_IRIX	
	//cout << "Score file: " << filename << endl;	
	fp = fopen(filename,"w");
	if(fp == NULL)
		{
 		if (err) error(FILERR,"creating score file",err);
		}
#endif
	cout << "Score file: " << filename << endl;	
	
	}

void outfile::close(void)
	{
#if defined D_MAC
	err = FSClose (fileRefNum);
	err = FlushVol (NULL, reply.vRefNum);
#endif
#if defined D_IRIX		
	err = fflush (fp);
	err = fclose (fp);
#endif
	if (err) error(FILERR,"closing parameterfile file",err);
	}
	
void outfile::write(char *str)
	{
	long	test;

	countBytes = strlen(str);	
	//cout << "Len: " << countBytes << ", String: " << str ;
#if defined D_MAC
	err = FSWrite (fileRefNum, &countBytes, str);
	if (err) error(FILERR,"writing score file",err);
#endif
#if defined D_IRIX		
	
	test = fwrite(str,sizeof(char),countBytes,fp);
	if (test != countBytes) 
		error(FILERR,"writing score file.",1);
		
#endif
	}
	
// outfile - functions ----------------------------------------
