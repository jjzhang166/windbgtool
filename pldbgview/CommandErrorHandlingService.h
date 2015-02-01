// CommandErrorHandlingService.h : Declaration of the CCommandErrorHandlingService

#pragma once
#include "resource.h"       // main symbols
#include "pldbgview_i.h"

using namespace ATL;


// CCommandErrorHandlingService

class ATL_NO_VTABLE CCommandErrorHandlingService :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CCommandErrorHandlingService, &CLSID_CommandErrorHandlingService>,
	public IPluginSupportNotifications,
	public IViewDebugEngineServiceEvents,
	public IInitializeWithControlImpl
{
public:
	CCommandErrorHandlingService()
	{
		m_dwAdvice = 0;
	}

	DECLARE_NO_REGISTRY()

	BEGIN_COM_MAP(CCommandErrorHandlingService)
		COM_INTERFACE_ENTRY(IPluginSupportNotifications)
		COM_INTERFACE_ENTRY(IViewDebugEngineServiceEvents)
		COM_INTERFACE_ENTRY(IInitializeWithControl)
	END_COM_MAP()

private:

	CComPtr<IServiceProvider> m_pServiceProvider;
	CComPtr<IViewDebugEngineService> m_pViewDebugEngineService;
	DWORD m_dwAdvice;

public:

	STDMETHOD(OnInitialized)(IServiceProvider* pServiceProvider);
	STDMETHOD(OnShutdown)();

	STDMETHOD(OnBeforeCommandExecute)(REFGUID guidCommandId, VARIANT* vParam);
	STDMETHOD(OnAfterCommandExecute)(REFGUID guidCommandId, VARIANT* vParam, IVariantObject* pResult);
};

OBJECT_ENTRY_AUTO(__uuidof(CommandErrorHandlingService), CCommandErrorHandlingService)
