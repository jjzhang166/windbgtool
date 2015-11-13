// DbgCommand.cpp : Implementation of CDbgCommand

#include "stdafx.h"
#include "DebugCommand.h"

// CDebugCommand

STDMETHODIMP CDebugCommand::SetDebugSession(IDebugSession* pDebugSession)
{
	CHECK_E_POINTER(pDebugSession);
	m_pDebugSession = pDebugSession;
	return S_OK;
}

STDMETHODIMP CDebugCommand::SetCommandText(BSTR bstrCmdText)
{
	CHECK_E_POINTER(bstrCmdText);
	m_strCommandText = bstrCmdText;
	return S_OK;
}

STDMETHODIMP CDebugCommand::Execute(VARIANT* vParam, VARIANT* vResult)
{
	m_strCommandResult.Empty();

	if (!m_pDebugSession)
		return E_NOT_VALID_STATE;

	CString strCommandText = m_strCommandText;
	if (vParam != NULL && vParam->vt != VT_EMPTY)
	{
		switch (vParam->vt)
		{
		case VT_BSTR:
			strCommandText.Format(m_strCommandText, vParam->bstrVal);
			break;

		case VT_UNKNOWN:
		{
			CComQIPtr<IBstrCollection> pBstrCollection = vParam->punkVal;
			ATLENSURE(pBstrCollection);
			UINT_PTR uiCount = 0;
			RETURN_IF_FAILED(pBstrCollection->GetCount(&uiCount));
			for (UINT_PTR i = 0; i < uiCount; i++)
			{
				CComBSTR bstr;
				RETURN_IF_FAILED(pBstrCollection->GetItem(i, &bstr));
				CString strToken;
				strToken.Format(L"%%%d", i);
				strCommandText.Replace(strToken, bstr);
			}
			break;
		}

		default:
			return E_INVALIDARG;
		}
	}

	CComQIPtr<IDebugControl4> pDebugControl4 = m_pDebugSession;
	ATLENSURE(pDebugControl4);
	if (pDebugControl4)
	{
		RETURN_IF_FAILED(pDebugControl4->OutputWide(DEBUG_OUTCTL_ALL_CLIENTS, L"\n>>>" + strCommandText + L"\n"));
	}

	HRESULT hr = m_pDebugSession->AddOutputCallback(this);
	if (FAILED(hr))
		return hr;

	hr = m_pDebugSession->Execute(DEBUG_OUTCTL_THIS_CLIENT, strCommandText, 0);
	if (FAILED(hr))
	{
		m_pDebugSession->RemoveOutputCallback(this);
		return hr;
	}

	hr = m_pDebugSession->RemoveOutputCallback(this);
	if (FAILED(hr))
		return hr;

	CComVar cv(m_strCommandResult);
	RETURN_IF_FAILED(cv.Detach(vResult));

	return S_OK;
}

STDMETHODIMP CDebugCommand::Output(ULONG Mask, LPCTSTR Text)
{
	UNREFERENCED_PARAMETER(Mask);
	CHECK_E_POINTER(Text);
	m_strCommandResult.Append(Text);
	return S_OK;
}