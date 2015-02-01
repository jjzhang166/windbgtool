// dllmain.h : Declaration of module class.

class CDbgEngineModule : public ATL::CAtlDllModuleT< CDbgEngineModule >
{
public :
	DECLARE_LIBID(LIBID_DbgEngineLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_DBGENGINE, "{BD428835-6E4D-4936-B3A7-7B640D92B2D2}")
};

extern class CDbgEngineModule _AtlModule;
