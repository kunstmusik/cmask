// globale Einstellungen für CMask 

#ifndef GLOBS
#define GLOBS


#define D_IRIX
//#define D_MAC
//#define D_DOS
//#define D_WIN

#if defined D_IRIX
	
	typedef char** 	Handle;
	typedef int 	OSErr;	

#endif

#if defined D_MAC
	#define NL "\r"

#elif defined D_IRIX
	#define NL "\n"

#elif defined D_DOS
	#define NL "\n"
#endif

#endif

using namespace std;
