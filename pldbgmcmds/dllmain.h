// dllmain.h : Declaration of module class.

class CpldbgmcmdsModule : public ATL::CAtlDllModuleT< CpldbgmcmdsModule >
{
public :
	DECLARE_LIBID(LIBID_pldbgmcmdsLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_PLDBGMCMDS, "{80129650-C77B-4868-B44E-34606137185F}")
};

extern class CpldbgmcmdsModule _AtlModule;
