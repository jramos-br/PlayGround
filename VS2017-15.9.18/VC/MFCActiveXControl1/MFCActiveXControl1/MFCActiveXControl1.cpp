// MFCActiveXControl1.cpp : Implementation of CMFCActiveXControl1App and DLL registration.

#include "pch.h"
#include "framework.h"
#include "MFCActiveXControl1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CMFCActiveXControl1App theApp;

const GUID CDECL _tlid = {0x96e7b8f4,0x003d,0x44a1,{0x9c,0x2f,0x54,0xf1,0x88,0x01,0x53,0xed}};
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;



// CMFCActiveXControl1App::InitInstance - DLL initialization

BOOL CMFCActiveXControl1App::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: Add your own module initialization code here.
	}

	return bInit;
}



// CMFCActiveXControl1App::ExitInstance - DLL termination

int CMFCActiveXControl1App::ExitInstance()
{
	// TODO: Add your own module termination code here.

	return COleControlModule::ExitInstance();
}



// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}



// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}
