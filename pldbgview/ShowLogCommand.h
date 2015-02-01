#pragma once
#include "resource.h"       // main symbols
#include "pldbgview_i.h"

using namespace ATL;

class ATL_NO_VTABLE CShowLogCommand :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CShowLogCommand, &CLSID_ShowLogCommand>,
	public ICommand2,
	public IInitializeWithControlImpl
{
private:
	BOOL m_bEnabled;
	CBitmap m_Bitmap;

public:
	CShowLogCommand()
	{
		m_bEnabled = TRUE;
		m_Bitmap.LoadBitmap(IDB_BITMAP1);
	}

	DECLARE_NO_REGISTRY()

	BEGIN_COM_MAP(CShowLogCommand)
		COM_INTERFACE_ENTRY(ICommand)
		COM_INTERFACE_ENTRY(ICommand2)
		COM_INTERFACE_ENTRY(IInitializeWithControl)
	END_COM_MAP()

public:

	STDMETHOD(GetHBITMAP)(REFGUID guidId, HBITMAP* pHBITMAP)
	{
		UNREFERENCED_PARAMETER(guidId);
		CHECK_E_POINTER(pHBITMAP);
		*pHBITMAP = m_Bitmap;
		return S_OK;
	}

	STDMETHOD(GetCommandText)(REFGUID guidId, BSTR* bstrText)
	{
		UNREFERENCED_PARAMETER(guidId);
		CHECK_E_POINTER(bstrText);
		*bstrText = CComBSTR(L"Show console").Copy();
		return S_OK;
	}

	STDMETHOD(Invoke)(REFGUID guidId)
	{
		UNREFERENCED_PARAMETER(guidId);
		CComQIPtr<IServiceProvider> pServiceProvider = m_pControl;
		ATLASSERT(pServiceProvider);
		CComPtr<IConsoleService> pConsoleService;
		RETURN_IF_FAILED(pServiceProvider->QueryService(SERVICE_CONSOLE, &pConsoleService));
		RETURN_IF_FAILED(pConsoleService->OpenConsole());
		return S_OK;
	}

	STDMETHOD(InstallToolbar)(IToolbar* pToolbar)
	{
		CHECK_E_POINTER(pToolbar);
		return pToolbar->AddToolbarCommand(COMMAND_SHOWLOG, this);
	}

	STDMETHOD(InstallMenu)(IMenu* pMenu)
	{
		CHECK_E_POINTER(pMenu);
		return pMenu->InsertMenuCommand(COMMAND_VIEW, COMMAND_SHOWLOG, 0, this);
	}

	STDMETHOD(OnIdle)(BOOL *bResult)
	{
		CHECK_E_POINTER(bResult);
		return S_OK;
	}

	STDMETHOD(GetEnabled)(REFGUID guidId, BOOL *pbEnabled)
	{
		UNREFERENCED_PARAMETER(guidId);
		CHECK_E_POINTER(pbEnabled);
		CComQIPtr<IServiceProvider> pServiceProvider = m_pControl;
		ATLASSERT(pServiceProvider);

		CComPtr<IDebugSessionService> pDebugSessionService;
		RETURN_IF_FAILED(pServiceProvider->QueryService(SERVICE_DEBUG_SESSION_SERVICE, IID_IDebugSessionService, (LPVOID*)&pDebugSessionService));
		CComPtr<IDebugSession> pDebugSession;
		HRESULT hr = pDebugSessionService->GetSession(&pDebugSession);
		if (hr == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND))
		{
			*pbEnabled = FALSE;
			return S_OK;
		}
		else if (FAILED(hr))
			return hr;

		*pbEnabled = TRUE;
		return S_OK;
	}
};

OBJECT_ENTRY_AUTO(__uuidof(ShowLogCommand), CShowLogCommand)
