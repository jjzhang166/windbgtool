// LogFormView.cpp : Implementation of CLogFormView

#include "stdafx.h"
#include "LogFormView.h"

// CLogFormView

STDMETHODIMP CLogFormView::GetText(BSTR* pbstr)
{
	CHECK_E_POINTER(pbstr);
	*pbstr = CComBSTR(L"Console").Copy();

	return S_OK;
}

STDMETHODIMP CLogFormView::GetHWND(HWND *hWnd)
{
	CHECK_E_POINTER(hWnd);
	*hWnd = m_hWnd;
	return S_OK;
}

STDMETHODIMP CLogFormView::CreateEx2(HWND hWndParent, RECT rect, HWND* hWnd)
{
	CHECK_E_POINTER(hWnd);
	*hWnd = Create(hWndParent, rect);
	return S_OK;
}

STDMETHODIMP CLogFormView::CreateEx(HWND hWndParent, HWND *hWnd)
{
	CHECK_E_POINTER(hWnd);
	*hWnd = Create(hWndParent);
	return S_OK;
}

STDMETHODIMP CLogFormView::PreTranslateMessage(MSG *pMsg, BOOL *pbResult)
{
	CHECK_E_POINTER(pMsg);
	CHECK_E_POINTER(pbResult);

	if(pMsg->message == WM_KEYDOWN && pMsg->hwnd == GetDlgItem(IDC_EDIT2) && pMsg->wParam == VK_ESCAPE)
	{
		if (!m_editCmd.SetWindowText(L""))
			return HRESULT_FROM_WIN32(GetLastError());
	}

	if(pMsg->message == WM_KEYDOWN && pMsg->hwnd == GetDlgItem(IDC_EDIT2) && pMsg->wParam == VK_RETURN)
	{
		if(m_bIsInProgress)
			return S_OK;

		CWaitCursor waitCursor;
		CComPtr<IDebugSession> pDebugSession;
		RETURN_IF_FAILED(GetDebugSession(&pDebugSession));
		CComPtr<IDebugCommand> pCommand;
		RETURN_IF_FAILED(pDebugSession->CreateCommand(&pCommand));
		CComBSTR bstrText;
		m_editCmd.GetWindowText(&bstrText);
		if(bstrText.Length() != 0)
		{
			RETURN_IF_FAILED(pCommand->SetCommandText(bstrText));
			RETURN_IF_FAILED(m_pViewDebugEngineService->ExecuteCommandAsync(pCommand));
			if (!m_editCmd.SetWindowText(L""))
				return HRESULT_FROM_WIN32(GetLastError());

			m_vCommandsHistory.push_back(CString(bstrText));
			m_commandHistoryIndex = static_cast<int>(m_vCommandsHistory.size() - 1);
		}
		*pbResult = TRUE;
	}

	if(pMsg->message == WM_KEYDOWN && pMsg->hwnd == GetDlgItem(IDC_EDIT2) && pMsg->wParam == VK_UP)
	{
		if(!m_vCommandsHistory.empty())
		{
			m_editCmd.SetWindowText(m_vCommandsHistory[m_commandHistoryIndex]);
		}

		if(m_commandHistoryIndex != 0)
			m_commandHistoryIndex--;
	}

	if(pMsg->message == WM_KEYDOWN && pMsg->hwnd == GetDlgItem(IDC_EDIT2) && pMsg->wParam == VK_DOWN)
	{
		if(!m_vCommandsHistory.empty())
		{
			m_editCmd.SetWindowText(m_vCommandsHistory[m_commandHistoryIndex]);
		}

		if(m_commandHistoryIndex != m_vCommandsHistory.size() - 1)
			m_commandHistoryIndex++;
	}

	return S_OK;
}

STDMETHODIMP CLogFormView::OnActivate()
{
	if(m_bInitialized)
		return S_OK;

	CComPtr<IDebugSession> pDebugSession;
	RETURN_IF_FAILED(GetDebugSession(&pDebugSession));
	RETURN_IF_FAILED(pDebugSession->AddOutputCallback(this));
	m_bInitialized = TRUE;

	return S_OK;
}

STDMETHODIMP CLogFormView::GetDebugSession(IDebugSession** ppDebugSession)
{
	if(!ppDebugSession)
		return E_POINTER;

	CComQIPtr<IServiceProvider> pServiceProvider =  m_pControl;
	if(!pServiceProvider)
		return E_NOINTERFACE;

	CComPtr<IDebugSessionService> pDebugSessionService;
	RETURN_IF_FAILED(pServiceProvider->QueryService(SERVICE_DEBUG_SESSION_SERVICE, IID_IDebugSessionService, (LPVOID*)&pDebugSessionService));
	return pDebugSessionService->GetSession(ppDebugSession);
}

STDMETHODIMP CLogFormView::OnClose()
{
	CComQIPtr<IServiceProvider> pServiceProvider =  m_pControl;
	if(!pServiceProvider)
		return E_NOINTERFACE;

	if(m_dwAdvice)
	{
		RETURN_IF_FAILED(AtlUnadvise(m_pViewDebugEngineService, IID_IViewDebugEngineServiceEvents, m_dwAdvice));
		m_dwAdvice = 0;
	}

	CComPtr<IDebugSession> pDebugSession = NULL;
	GetDebugSession(&pDebugSession);
	if(pDebugSession)
		pDebugSession->RemoveOutputCallback(this);

	return S_OK;
}

void CLogFormView::FinalRelease()
{
}


STDMETHODIMP CLogFormView::OnDeactivate()
{
	return S_OK;
}

STDMETHODIMP CLogFormView::WriteLine(BSTR bstr)
{
	return Output(0, bstr);
}

STDMETHODIMP CLogFormView::Output(ULONG Mask, LPCTSTR Text)
{
	UNREFERENCED_PARAMETER(Mask);
	CString str(Text);
	str.Replace(L"\r", L"\r\n");
	str.Replace(L"\n", L"\r\n");
	m_edit.AppendText(str);
	return S_OK;
}

LRESULT CLogFormView::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	DlgResize_Init(false);
	m_editCmd.SubclassWindow(GetDlgItem(IDC_EDIT2));
	m_edit.Attach(GetDlgItem(IDC_EDIT1));
	m_edit.SetLimitText(200 * 1024 * 1024); //200 Mb
	HFONT hFont = m_edit.GetFont();
	LOGFONT logFont = {0};
	::GetObject(hFont, sizeof(LOGFONT), (LPVOID)&logFont);
	_tcscpy_s(logFont.lfFaceName, L"Consolas");
	m_editFont.CreateFontIndirect(&logFont);
	m_edit.SetFont(m_editFont);
	bHandled = FALSE;

	CComQIPtr<IServiceProvider> pServiceProvider =  m_pControl;
	if(!pServiceProvider)
		return E_NOINTERFACE;

	RETURN_IF_FAILED(pServiceProvider->QueryService(SERVICE_VIEW_DEBUG_SESSION_SERVICE, IID_IViewDebugEngineService, (LPVOID*)&m_pViewDebugEngineService));
	CComPtr<IUnknown> pThis;
	RETURN_IF_FAILED(QueryInterface(IID_IUnknown, (LPVOID*)&pThis));
	RETURN_IF_FAILED(m_pViewDebugEngineService.Advise(pThis, IID_IViewDebugEngineServiceEvents, &m_dwAdvice));

	return 0;
}

STDMETHODIMP CLogFormView::OnBeforeCommandExecute(REFGUID guidCommandId, VARIANT* vParam)
{
	UNREFERENCED_PARAMETER(guidCommandId);
	UNREFERENCED_PARAMETER(vParam);

	GetDlgItem(IDC_EDIT2).EnableWindow(FALSE);
	m_bIsInProgress = TRUE;
	return S_OK;
}

STDMETHODIMP CLogFormView::OnAfterCommandExecute(REFGUID guidCommandId, VARIANT* vParam, IVariantObject* pResult)
{
	UNREFERENCED_PARAMETER(guidCommandId);
	UNREFERENCED_PARAMETER(vParam);
	UNREFERENCED_PARAMETER(pResult);

	GetDlgItem(IDC_EDIT2).EnableWindow(TRUE);
	m_bIsInProgress = FALSE;
	if(IsWindowVisible())
	{
		GetDlgItem(IDC_EDIT2).SetFocus();
	}
	return S_OK;
}

