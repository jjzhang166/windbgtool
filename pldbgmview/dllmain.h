// dllmain.h : Declaration of module class.

class CpldbgmviewModule : public ATL::CAtlDllModuleT< CpldbgmviewModule >
{
public :
	DECLARE_LIBID(LIBID_pldbgmviewLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_PLDBGMVIEW, "{8AF83115-0D7B-4A94-A69E-AB9A55B52030}")
};

extern class CpldbgmviewModule _AtlModule;
