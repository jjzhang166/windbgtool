// ViewDebugEngineService.h : Declaration of the CViewDebugEngineService

#pragma once
#include "resource.h"       // main symbols
#include "pldbgview_i.h"
#include "..\DbgEngine\Plugins.h"
#include "ViewDebugEngineServiceEventSink.h"
#include "CommandContext.h"

using namespace ATL;

// CViewDebugEngineService

class ATL_NO_VTABLE CViewDebugEngineService :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CViewDebugEngineService, &CLSID_ViewDebugEngineService>,
	public IViewDebugEngineService,
	public IInitializeWithControl,
	public IPluginSupportNotifications,
	public IMsgHandler,
	public IConnectionPointContainerImpl<CViewDebugEngineService>,
	public ViewDebugEngineServiceEventSink<CViewDebugEngineService>
{
private:
	CComPtr<IPluginManager> m_pPluginManager;
	CComPtr<IDebugSessionService> m_pDebugSessionService;
public:
	CViewDebugEngineService()
	{
		m_bInProgress = FALSE;
		m_hr = S_OK;
		m_hWnd = 0;
	}

	DECLARE_NO_REGISTRY()
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(CViewDebugEngineService)
		COM_INTERFACE_ENTRY(IViewDebugEngineService)
		COM_INTERFACE_ENTRY(IInitializeWithControl)
		COM_INTERFACE_ENTRY(IMsgHandler)
		COM_INTERFACE_ENTRY(IConnectionPointContainer)
		COM_INTERFACE_ENTRY(IPluginSupportNotifications)
		COM_INTERFACE_ENTRY_AGGREGATE(IID_IDebugSessionService, m_pDebugSessionService)
	END_COM_MAP()

	BEGIN_CONNECTION_POINT_MAP(CViewDebugEngineService)
		CONNECTION_POINT_ENTRY(__uuidof(IViewDebugEngineServiceEvents))
	END_CONNECTION_POINT_MAP()

	HRESULT FinalConstruct();
	void FinalRelease();

private:
	HRESULT m_hr;
	HWND m_hWnd;
	volatile bool m_bInProgress;

	boost::mutex m_mutex;
	boost::mutex m_mutexThread;
	boost::condition_variable m_condition;
	std::queue<CCommandContext> m_commandsToExecute;
	std::queue<CCommandContext> m_commandsResult;

	CComPtr<IControl> m_pControl;
	CComPtr<IServiceProvider> m_pServiceProvider;

	static void ThreadProcStatic(void* pData);
	HRESULT ThreadProc();
	void ClearState();

	void OnBeforeCommandExecute();
	void OnAfterCommandExecute();

public:
	STDMETHOD(SetControl)(IControl* pControl);
	STDMETHOD(ProcessWindowMessage)(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* plResult, BOOL* bResult);

	STDMETHOD(OpenSession)();
	STDMETHOD(CloseSession)();
	STDMETHOD(OpenDumpFile)(BSTR bstrDumpFile);
	STDMETHOD(CloseDumpFile)();
	STDMETHOD(ExecuteCommandSimpleAsync)(REFIID iidCommand, VARIANT* vParam);
	STDMETHOD(ExecuteCommandAsync)(IDebugCommand* pDebugCommand);
	STDMETHOD(GetLastError)();
	
	STDMETHOD(OnInitialized)(IServiceProvider* pServiceProvider);
	STDMETHOD(OnShutdown)();
private:

	class CBoolFLagSafeScope
	{
		bool m_bPrevValue;
		volatile bool* m_pbFlag;

	public:
		CBoolFLagSafeScope(volatile bool* pbFlag)
		{
			m_bPrevValue = *pbFlag;
			*pbFlag = TRUE;
			m_pbFlag = pbFlag;
		}

		~CBoolFLagSafeScope()
		{
			*m_pbFlag = m_bPrevValue;
			m_pbFlag = NULL;
		}
	};

	class CEventScope
	{
	private:
		HWND m_hWnd;
	public:
		CEventScope(HWND hWnd)
		{
			m_hWnd = hWnd;
			::PostMessage(m_hWnd, WM_COMMAND_START, 0, 0);
		}

		~CEventScope()
		{
			::PostMessage(m_hWnd, WM_COMMAND_END, 0, 0);
		}
	};
};

OBJECT_ENTRY_AUTO(__uuidof(ViewDebugEngineService), CViewDebugEngineService)
