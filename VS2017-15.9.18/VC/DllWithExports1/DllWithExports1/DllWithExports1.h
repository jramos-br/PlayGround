// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the DLLWITHEXPORTS1_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// DLLWITHEXPORTS1_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef DLLWITHEXPORTS1_EXPORTS
#define DLLWITHEXPORTS1_API __declspec(dllexport)
#else
#define DLLWITHEXPORTS1_API __declspec(dllimport)
#endif

// This class is exported from the dll
class DLLWITHEXPORTS1_API CDllWithExports1 {
public:
	CDllWithExports1(void);
	// TODO: add your methods here.
};

extern DLLWITHEXPORTS1_API int nDllWithExports1;

DLLWITHEXPORTS1_API int fnDllWithExports1(void);
