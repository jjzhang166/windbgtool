// ProgressBarService.h : Declaration of the CProgressBarService

#pragma once
#include "resource.h"       // main symbols
#include "pldbgview_i.h"

using namespace ATL;


// CProgressBarService

class ATL_NO_VTABLE CProgressBarService :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CProgressBarService, &CLSID_ProgressBarService>,
	public IPluginSupportNotifications,
	public IViewDebugEngineServiceEvents,
	public IInitializeWithControlImpl,
	public IMsgFilter
{
public:
	CProgressBarService()
	{
		m_dwAdvice = 0;
		m_hWndStatusBar = 0;
	}

	DECLARE_NO_REGISTRY()


	BEGIN_COM_MAP(CProgressBarService)
		COM_INTERFACE_ENTRY(IPluginSupportNotifications)
		COM_INTERFACE_ENTRY(IViewDebugEngineServiceEvents)
		COM_INTERFACE_ENTRY(IInitializeWithControl)
		COM_INTERFACE_ENTRY(IMsgFilter)
	END_COM_MAP()

private:
	CProgressBarCtrl m_progress;
	CComPtr<IViewDebugEngineService> m_pViewDebugEngineService;
	CComPtr<IStatusBar> m_pStatusBar;
	DWORD m_dwAdvice;
	HWND m_hWndStatusBar;

	STDMETHOD(AddPartIfNeed)();
public:

	STDMETHOD(OnInitialized)(IServiceProvider* pServiceProvider);
	STDMETHOD(OnShutdown)();

	STDMETHOD(OnBeforeCommandExecute)(REFGUID guidCommandId, VARIANT* vParam);
	STDMETHOD(OnAfterCommandExecute)(REFGUID guidCommandId, VARIANT* vParam, IVariantObject* pResult);
	STDMETHOD(PreTranslateMessage)(MSG* pMsg, BOOL* pbResult);
};

OBJECT_ENTRY_AUTO(__uuidof(ProgressBarService), CProgressBarService)
