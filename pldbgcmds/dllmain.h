// dllmain.h : Declaration of module class.

class CpldbgcmdsModule : public ATL::CAtlDllModuleT< CpldbgcmdsModule >
{
public :
	DECLARE_LIBID(LIBID_pldbgcmdsLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_PLDBGCMDS, "{E3D91770-779C-4C5A-9288-22FBD51BDA55}")
};

extern class CpldbgcmdsModule _AtlModule;
