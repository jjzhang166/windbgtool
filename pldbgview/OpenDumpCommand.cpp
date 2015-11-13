#include "stdafx.h"
#include "OpenDumpCommand.h"

STDMETHODIMP COpenDumpCommand::GetHBITMAP(REFGUID guidId, HBITMAP* pHBITMAP)
{
	UNREFERENCED_PARAMETER(guidId);
	CHECK_E_POINTER(pHBITMAP);
	*pHBITMAP = m_Bitmap;
	return S_OK;
}

STDMETHODIMP COpenDumpCommand::GetCommandText(REFGUID guidId, BSTR* bstrText)
{
	UNREFERENCED_PARAMETER(guidId);
	CHECK_E_POINTER(bstrText);
	*bstrText = CComBSTR(L"Open dump...").Copy();
	return S_OK;
}

STDMETHODIMP COpenDumpCommand::Invoke(REFGUID guidId)
{
	UNREFERENCED_PARAMETER(guidId);
	CFileDialog dlg(TRUE);
	if (dlg.DoModal() != IDOK)
		return HRESULT_FROM_WIN32(ERROR_CANCELLED);

	CComQIPtr<IServiceProvider> pServiceProvider = m_pControl;
	ATLASSERT(pServiceProvider);

	CWaitCursor w;

	CComPtr<IViewDebugEngineService> pViewDebugEngineService;
	RETURN_IF_FAILED(pServiceProvider->QueryService(SERVICE_VIEW_DEBUG_SESSION_SERVICE, IID_IViewDebugEngineService, (LPVOID*)&pViewDebugEngineService));
	RETURN_IF_FAILED(pViewDebugEngineService->OpenSession());

	CComPtr<IConsoleService> pConsoleService;
	RETURN_IF_FAILED(pServiceProvider->QueryService(SERVICE_CONSOLE, &pConsoleService));
	RETURN_IF_FAILED(pConsoleService->OpenConsole());

	ATLENSURE(pViewDebugEngineService);
	RETURN_IF_FAILED(pViewDebugEngineService->ExecuteCommandSimpleAsync(DBGCOMMAND_OPENDUMP, &CComVar(dlg.m_ofn.lpstrFile)));
	return S_OK;
}

STDMETHODIMP COpenDumpCommand::InstallToolbar(IToolbar* pToolbar)
{
	CHECK_E_POINTER(pToolbar);
	return pToolbar->InsertToolbarCommand(COMMAND_OPENDUMP, 0, this);
}

STDMETHODIMP COpenDumpCommand::InstallMenu(IMenu* pMenu)
{
	CHECK_E_POINTER(pMenu);
	return pMenu->InsertMenuCommand(COMMAND_FILE, COMMAND_OPENDUMP, 0, this);
}

STDMETHODIMP COpenDumpCommand::OnIdle(BOOL *bResult)
{
	CHECK_E_POINTER(bResult);
	return S_OK;
}

STDMETHODIMP COpenDumpCommand::GetEnabled(REFGUID guidId, BOOL *pbEnabled)
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
		*pbEnabled = TRUE;
		return S_OK;
	}
	else if (FAILED(hr))
		return hr;

	*pbEnabled = FALSE;
	return S_OK;
}
