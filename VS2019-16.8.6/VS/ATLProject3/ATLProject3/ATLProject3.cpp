// ATLProject3.cpp : Implementation of WinMain


#include "pch.h"
#include "framework.h"
#include "resource.h"
#include "ATLProject3_i.h"


using namespace ATL;

#include <stdio.h>

class CATLProject3Module : public ATL::CAtlServiceModuleT< CATLProject3Module, IDS_SERVICENAME >
{
public :
	DECLARE_LIBID(LIBID_ATLProject3Lib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_ATLPROJECT3, "{61f516e7-a7bc-434d-a1fa-96963c3b0a87}")
	HRESULT InitializeSecurity() throw()
	{
		// TODO : Call CoInitializeSecurity and provide the appropriate security settings for your service
		// Suggested - PKT Level Authentication,
		// Impersonation Level of RPC_C_IMP_LEVEL_IDENTIFY
		// and an appropriate non-null Security Descriptor.

		return S_OK;
	}
};

CATLProject3Module _AtlModule;



//
extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/,
								LPTSTR /*lpCmdLine*/, int nShowCmd)
{
	return _AtlModule.WinMain(nShowCmd);
}

