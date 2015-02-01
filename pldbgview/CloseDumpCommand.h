#pragma once
#include "resource.h"       // main symbols
#include "pldbgview_i.h"

using namespace ATL;

class ATL_NO_VTABLE CCloseDumpCommand :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CCloseDumpCommand, &CLSID_CloseDumpCommand>,
	public ICommand2,
	public IInitializeWithControlImpl
{
private:

public:
	CCloseDumpCommand()
	{
	}

	DECLARE_NO_REGISTRY()

	BEGIN_COM_MAP(CCloseDumpCommand)
		COM_INTERFACE_ENTRY(ICommand)
		COM_INTERFACE_ENTRY(ICommand2)
		COM_INTERFACE_ENTRY(IInitializeWithControl)
	END_COM_MAP()

public:

	STDMETHOD(GetHBITMAP)(REFGUID guidId, HBITMAP* pHBITMAP)
	{
		UNREFERENCED_PARAMETER(guidId);
		UNREFERENCED_PARAMETER(pHBITMAP);
		return E_NOTIMPL;
	}

	STDMETHOD(GetCommandText)(REFGUID guidId, BSTR* bstrText)
	{
		UNREFERENCED_PARAMETER(guidId);
		CHECK_E_POINTER(bstrText);
		*bstrText = CComBSTR(L"Close dump").Copy();
		return S_OK;
	}

	STDMETHOD(Invoke)(REFGUID guidId)
	{
		UNREFERENCED_PARAMETER(guidId);
		CWaitCursor w;

		CComQIPtr<IServiceProvider> pServiceProvider = m_pControl;
		ATLENSURE(pServiceProvider);
		CComPtr<IFormManager> pFormManager;
		RETURN_IF_FAILED(pServiceProvider->QueryService(SERVICE_FORM_MANAGER, IID_IFormManager, (LPVOID*)&pFormManager));
		RETURN_IF_FAILED(pFormManager->CloseAll());
		CComPtr<IViewDebugEngineService> pViewDebugEngineService;
		RETURN_IF_FAILED(pServiceProvider->QueryService(SERVICE_VIEW_DEBUG_SESSION_SERVICE, IID_IViewDebugEngineService, (LPVOID*)&pViewDebugEngineService));
		RETURN_IF_FAILED(pViewDebugEngineService->CloseDumpFile());
		RETURN_IF_FAILED(pViewDebugEngineService->CloseSession());

		return S_OK;
	}

	STDMETHOD(InstallToolbar)(IToolbar* pToolbar)
	{
		UNREFERENCED_PARAMETER(pToolbar);
		return E_NOTIMPL;
	}

	STDMETHOD(InstallMenu)(IMenu* pMenu)
	{
		CHECK_E_POINTER(pMenu);
		return pMenu->InsertMenuCommand(COMMAND_FILE, COMMAND_CLOSEDUMP, 0, this);
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
		if (!pServiceProvider)
			return E_NOINTERFACE;

		CComPtr<ICommand2> pCommand2;
		RETURN_IF_FAILED(pServiceProvider->QueryService(COMMAND_OPENDUMP, IID_ICommand2, (LPVOID*)&pCommand2));
		RETURN_IF_FAILED(pCommand2->GetEnabled(COMMAND_OPENDUMP, pbEnabled));
		*pbEnabled = !*pbEnabled;
		return S_OK;
	}
};

OBJECT_ENTRY_AUTO(__uuidof(CloseDumpCommand), CCloseDumpCommand)
