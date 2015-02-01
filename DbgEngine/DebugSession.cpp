// DebugSession.cpp : Implementation of CDebugSession

#include "stdafx.h"
#include "DebugSession.h"
#include "DebugCommand.h"


// CDebugSession

HRESULT CDebugSession::FinalConstruct()
{
	RETURN_IF_FAILED(HrGetPluginManager(&m_pPluginManager));
	m_pDebugOutputCallbacks = std::make_shared<COutputCallbacks>(this);
	return S_OK;
}

void CDebugSession::FinalRelease()
{
}

STDMETHODIMP CDebugSession::CloseDumpFile()
{
	RETURN_IF_FAILED(m_pDebugClient->EndSession(DEBUG_END_ACTIVE_DETACH));
	return S_OK;
}

STDMETHODIMP CDebugSession::Close()
{
	RETURN_IF_FAILED(m_pDebugClient->SetOutputCallbacks(NULL));
	m_pOutputCallbacks.clear();
	m_pDebugClient.Release();
	return S_OK;
}

STDMETHODIMP CDebugSession::Open()
{
	RETURN_IF_FAILED(DebugCreate(__uuidof(IDebugClient4), (LPVOID*)&m_pDebugClient));
	RETURN_IF_FAILED(m_pDebugClient->SetOutputCallbacks(m_pDebugOutputCallbacks.get()));
	return S_OK;
}

STDMETHODIMP CDebugSession::OpenDumpFile(LPCTSTR dumpFilePath)
{
	CHECK_E_POINTER(dumpFilePath);

	CComQIPtr<IDebugControl> pDebugControl = m_pDebugClient;
	if(!pDebugControl)
		return E_NOINTERFACE;

	USES_CONVERSION;
	HRESULT hr = m_pDebugClient->OpenDumpFile(W2A(dumpFilePath));
	if(FAILED(hr))
		return hr;

	return pDebugControl->WaitForEvent(DEBUG_WAIT_DEFAULT, INFINITE);
}

STDMETHODIMP CDebugSession::CreateCommand(IDebugCommand** ppCmd)
{
	CHECK_E_POINTER(ppCmd);
	HRESULT hr = CDebugCommand::_CreatorClass::CreateInstance(NULL, IID_IDebugCommand, (LPVOID*)ppCmd);
	if(FAILED(hr))
		return hr;

	return  (*ppCmd)->SetDebugSession(this);
}

STDMETHODIMP CDebugSession::AddOutputCallback(IDebugOutputCallback* pCallback)
{
	CHECK_E_POINTER(pCallback);
	auto result = m_pOutputCallbacks.insert(CAdapt<CComPtr<IDebugOutputCallback> >(pCallback));
	return result.second ? S_OK : E_FAIL;
}

STDMETHODIMP CDebugSession::RemoveOutputCallback(IDebugOutputCallback* pCallback)
{
	CHECK_E_POINTER(pCallback);
	auto result = m_pOutputCallbacks.erase(CAdapt<CComPtr<IDebugOutputCallback> >(pCallback));
	return result != 0 ? S_OK : E_FAIL;
}

STDMETHODIMP CDebugSession::Output(ULONG Mask, LPCSTR Text)
{
	CHECK_E_POINTER(Text);
	if(Mask != m_Mask)
		FlushOutput();

	m_Mask = Mask;
	CString strwText(Text);

	m_strBulkBuffer += strwText;

	if(m_bBulkOutput)
	{
		return S_OK;
	}

	FlushOutput();
	return S_OK;
}

STDMETHODIMP CDebugSession::FlushOutput()
{
	if(m_strBulkBuffer.IsEmpty())
		return S_OK;

	for(auto it = m_pOutputCallbacks.begin(); it != m_pOutputCallbacks.end(); it++)
	{
		(*it)->Output(0, m_strBulkBuffer);
	}
	m_strBulkBuffer.Empty();
	return S_OK;
}

STDMETHODIMP CDebugSession::Execute(ULONG OutputControl, LPCTSTR lpszCommandText, ULONG Flags)
{
	UNREFERENCED_PARAMETER(OutputControl);
	UNREFERENCED_PARAMETER(Flags);
	CHECK_E_POINTER(lpszCommandText);
	CComQIPtr<IDebugControl4> pDebugControl = m_pDebugClient;
	if(!pDebugControl)
		return E_NOINTERFACE;

	{
		CBulkOutput bulkOutput(this);
		RETURN_IF_FAILED(pDebugControl->ExecuteWide(DEBUG_OUTCTL_THIS_CLIENT, lpszCommandText, 0));
	}

	return S_OK;
}
