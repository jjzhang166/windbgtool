// dllmain.h : Declaration of module class.

class CpldbgviewModule : public ATL::CAtlDllModuleT< CpldbgviewModule >
{
public :
	DECLARE_LIBID(LIBID_pldbgviewLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_PLDBGVIEW, "{FEA49CB7-5704-489E-809C-AA18FEF0330C}")
};

extern class CpldbgviewModule _AtlModule;
