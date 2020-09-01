// DllWithExports1.cpp : Defines the exported functions for the DLL.
//

#include "pch.h"
#include "framework.h"
#include "DllWithExports1.h"


// This is an example of an exported variable
DLLWITHEXPORTS1_API int nDllWithExports1=0;

// This is an example of an exported function.
DLLWITHEXPORTS1_API int fnDllWithExports1(void)
{
    return 0;
}

// This is the constructor of a class that has been exported.
CDllWithExports1::CDllWithExports1()
{
    return;
}
