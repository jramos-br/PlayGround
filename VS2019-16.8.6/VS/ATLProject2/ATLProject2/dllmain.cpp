// dllmain.cpp : Implementation of DllMain.

#include "pch.h"
#include "framework.h"
#include "resource.h"
#include "ATLProject2_i.h"
#include "dllmain.h"

CATLProject2Module _AtlModule;

class CATLProject2App : public CWinApp
{
public:

// Overrides
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CATLProject2App, CWinApp)
END_MESSAGE_MAP()

CATLProject2App theApp;

BOOL CATLProject2App::InitInstance()
{
	return CWinApp::InitInstance();
}

int CATLProject2App::ExitInstance()
{
	return CWinApp::ExitInstance();
}
