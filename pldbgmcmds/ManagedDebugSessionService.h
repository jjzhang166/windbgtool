// ManagedDebugSessionService.h : Declaration of the CManagedDebugSessionService

#pragma once
#include "resource.h"       // main symbols
#include "pldbgmcmds_i.h"

using namespace ATL;


// CManagedDebugSessionService

class ATL_NO_VTABLE CManagedDebugSessionService :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CManagedDebugSessionService, &CLSID_ManagedDebugSessionService>,
	public IPluginSupportNotifications,
	public IDebugSessionServiceEvents
{
public:
	CManagedDebugSessionService()
	{
		m_dwAdvice = 0;
	}

	DECLARE_NO_REGISTRY()

	BEGIN_COM_MAP(CManagedDebugSessionService)
		COM_INTERFACE_ENTRY(IPluginSupportNotifications)
		COM_INTERFACE_ENTRY(IDebugSessionServiceEvents)
	END_COM_MAP()

private:

	CComPtr<IDebugSessionService> m_pDebugSessionService;
	DWORD m_dwAdvice;

public:

	STDMETHOD(OnInitialized)(IServiceProvider* pServiceProvider);
	STDMETHOD(OnShutdown)();

	STDMETHOD(OnBeforeCommandExecute)(REFGUID guidCommandId, VARIANT* vParam);
	STDMETHOD(OnAfterCommandExecute)(REFGUID guidCommandId, VARIANT* vParam, IVariantObject* pResult);

};

OBJECT_ENTRY_AUTO(__uuidof(ManagedDebugSessionService), CManagedDebugSessionService)
