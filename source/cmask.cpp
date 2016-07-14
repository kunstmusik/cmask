//  							CmaskMain.c


//MSLHeaders.x86

#include "globals.h" 

 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <iostream>

#if defined D_MAC
	#include <console.h>
	#include <sioux.h>
#endif

#include "event.h"
#include "mask.h"
#include "utils.h"
#include "fileio.h"
#include "parser.h"
 
	
main(int argc, char *argv[])
{	 
	scanner s;
	int	i;
	char	*inname, *outname;


	inname = new char [32];
	outname = new char [32];

	if (!inname || !outname) error(MEMORY,"file name string");

#if defined D_MAC
	strcpy(inname,"indummy");
	strcpy(outname,"outdummy");
	//c = InstallConsole(cout);
	//SIOUXSettings.autocloseonquit = TRUE;
	SIOUXSettings.asktosaveonclose = FALSE;
	SIOUXSetTitle("\pCMask");
	//SIOUXSettings.setupmenus = FALSE;	
#endif

	cout << "-------- CMask 0.31 --------\n" << endl;
	
#if defined D_MAC
	cout << "Choose parameter file ..." << endl;
#endif	

#if defined D_IRIX
	//argc = ccommand(&argv);
	if(argc!=3 && argc!=2)
		{
		cout << "Usage: " << argv[0] << " parameterfilename [scorefilename]" << endl;
		cout << "\n----------------------------\n" << endl;
        exit(-1);
		}
    sscanf(argv[1],"%s",inname);
    if(argc==3)
        sscanf(argv[2],"%s",outname);
    else
		strcpy(outname,inname);
	//cout << "parameter file: " << inname << endl;
#endif

	frandinit();

	s.scn(inname,outname);
	s.analyze(); 

	cout << "\nscore file OK" << endl;
	
#if defined D_MAC
	cout << "\nCmd-Q to exit ! " << endl;
#endif	

#if defined D_IRIX
	cout << "\n----------------------------\n" << endl;
#endif	
	
	delete inname;
	delete outname;
	return 0 ;
	}




