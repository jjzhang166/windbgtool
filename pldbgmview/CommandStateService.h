// CommandStateService.h : Declaration of the CCommandStateService

#pragma once
#include "resource.h"       // main symbols
#include "pldbgmview_i.h"

using namespace ATL;


// CCommandStateService

class ATL_NO_VTABLE CCommandStateService :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CCommandStateService, &CLSID_CommandStateService>,
	public IPluginSupportNotifications,
	public IViewDebugEngineServiceEvents,
	public IInitializeWithControlImpl
{
public:
	CCommandStateService();

	DECLARE_NO_REGISTRY()

	BEGIN_COM_MAP(CCommandStateService)
		COM_INTERFACE_ENTRY(IPluginSupportNotifications)
		COM_INTERFACE_ENTRY(IViewDebugEngineServiceEvents)
		COM_INTERFACE_ENTRY(IInitializeWithControl)
	END_COM_MAP()

private:

	CComPtr<IViewDebugEngineService> m_pViewDebugEngineService;
	CComQIPtr<ICommandSupport> m_pCommandSupport;
	DWORD m_dwAdvice;
	STDMETHOD(EnableCommands)(BOOL bEnabled);

	std::set< GUID, GUIDComparer > m_hashSet;

public:

	STDMETHOD(OnInitialized)(IServiceProvider* pServiceProvider);
	STDMETHOD(OnShutdown)();

	STDMETHOD(OnBeforeCommandExecute)(REFGUID guidCommandId, VARIANT* vParam);
	STDMETHOD(OnAfterCommandExecute)(REFGUID guidCommandId, VARIANT* vParam, IVariantObject* pResult);

};

OBJECT_ENTRY_AUTO(__uuidof(CommandStateService), CCommandStateService)
