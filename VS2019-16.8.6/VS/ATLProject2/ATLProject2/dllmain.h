// dllmain.h : Declaration of module class.

class CATLProject2Module : public ATL::CAtlDllModuleT< CATLProject2Module >
{
public :
	DECLARE_LIBID(LIBID_ATLProject2Lib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_ATLPROJECT2, "{01240012-02f7-4cde-bcc8-16f638663bee}")
};

extern class CATLProject2Module _AtlModule;
