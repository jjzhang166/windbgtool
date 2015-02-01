// DebugFormManager.h : Declaration of the CDebugFormManager

#pragma once
#include "resource.h"       // main symbols
#include "pldbgview_i.h"

using namespace ATL;

// CDebugFormManager

class ATL_NO_VTABLE CDebugFormManager :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDebugFormManager, &CLSID_DebugFormManager>,
	public IDebugFormManager,
	public IPluginSupportNotifications,
	public IViewDebugEngineServiceEvents
{
public:
	CDebugFormManager()
	{
		m_dwCookie = 0;
		m_bSelfCall = FALSE;
	}

	DECLARE_NO_REGISTRY()

	BEGIN_COM_MAP(CDebugFormManager)
		COM_INTERFACE_ENTRY(IDebugFormManager)
		COM_INTERFACE_ENTRY(IViewDebugEngineServiceEvents)
		COM_INTERFACE_ENTRY(IPluginSupportNotifications)
	END_COM_MAP()

private:
	bool m_bSelfCall;
	DWORD m_dwCookie;
	CComPtr<IServiceProvider> m_pServiceProvider;
	CComPtr<IViewDebugEngineService> m_pViewDebugEngineService;
public:
	STDMETHOD(OpenForm)(REFGUID guidCommandId, VARIANT* vParam);
	STDMETHOD(OnInitialized)(IServiceProvider* pServiceProvider);
	STDMETHOD(OnShutdown)();

	STDMETHOD(OnBeforeCommandExecute)(REFGUID guidCommandId, VARIANT* vParam);
	STDMETHOD(OnAfterCommandExecute)(REFGUID guidCommandId, VARIANT* vParam, IVariantObject* pResult);

};

OBJECT_ENTRY_AUTO(__uuidof(DebugFormManager), CDebugFormManager)
