// ViewDebugEngineService.cpp : Implementation of CViewDebugEngineService

#include "stdafx.h"
#include "ViewDebugEngineService.h"


// CViewDebugEngineService

HRESULT CViewDebugEngineService::FinalConstruct()
{
	return S_OK;
}

void CViewDebugEngineService::FinalRelease()
{

}

STDMETHODIMP CViewDebugEngineService::OpenSession()
{
	return m_pDebugSessionService->OpenSession();
}

STDMETHODIMP CViewDebugEngineService::CloseSession()
{
	return m_pDebugSessionService->CloseSession();
}

STDMETHODIMP CViewDebugEngineService::OpenDumpFile(BSTR bstrDumpFile)
{
	return m_pDebugSessionService->OpenDumpFile(bstrDumpFile);
}

STDMETHODIMP CViewDebugEngineService::CloseDumpFile()
{
	return m_pDebugSessionService->CloseDumpFile();
}

STDMETHODIMP CViewDebugEngineService::ExecuteCommandAsync(IDebugCommand* pDebugCommand)
{
	CHECK_E_POINTER(pDebugCommand);

	CCommandContext cmdContext;
	cmdContext.m_pDebugCommand = pDebugCommand;

	{
		boost::lock_guard<boost::mutex> guard(m_mutex);
		m_commandsToExecute.push(cmdContext);
	}

	m_condition.notify_one();

	return S_OK;
}

STDMETHODIMP CViewDebugEngineService::ExecuteCommandSimpleAsync(REFIID iidCommand, VARIANT* vParam)
{
	CCommandContext cmdContext;
	cmdContext.m_guidCommand = iidCommand;
	if (vParam)
		cmdContext.m_vParam = *vParam;

	{
		boost::lock_guard<boost::mutex> guard(m_mutex);
		m_commandsToExecute.push(cmdContext);
	}

	m_condition.notify_one();

	return S_OK;
}

STDMETHODIMP CViewDebugEngineService::ProcessWindowMessage(HWND /*hWnd*/, UINT uMsg, WPARAM /*wParam*/, LPARAM /*lParam*/, LRESULT* /*plResult*/, BOOL* bResult)
{
	if (uMsg == WM_COMMAND_START)
	{
		OnBeforeCommandExecute();
		*bResult = TRUE;
	}

	if (uMsg == WM_COMMAND_END)
	{
		OnAfterCommandExecute();
		*bResult = TRUE;
	}

	return S_OK;
}

void CViewDebugEngineService::OnBeforeCommandExecute()
{
	CCommandContext cmdContext;
	{
		boost::lock_guard<boost::mutex> guard(m_mutex);
		cmdContext = m_commandsResult.front();
	}
	Fire_OnBeforeCommandExecute(cmdContext.m_guidCommand, &cmdContext.m_vParam);
}

void CViewDebugEngineService::OnAfterCommandExecute()
{
	CCommandContext cmdContext;
	{
		boost::lock_guard<boost::mutex> guard(m_mutex);
		cmdContext = m_commandsResult.front();
		m_commandsResult.pop();
	}
	Fire_OnAfterCommandExecute(cmdContext.m_guidCommand, &cmdContext.m_vParam, cmdContext.m_pResultObject);
}

HRESULT CViewDebugEngineService::ThreadProc()
{
	RETURN_IF_FAILED(m_pControl->GetHWND(&m_hWnd));
	RETURN_IF_FAILED(CoInitialize(NULL));

	while (true)
	{
		boost::unique_lock<boost::mutex> lock(m_mutexThread);
		m_condition.wait(lock);

		while (true)
		{
			{
				boost::lock_guard<boost::mutex> guard(m_mutex);
				bool isEmpty = m_commandsToExecute.empty();
				if (isEmpty)
					break;
			}

			CCommandContext cmdContext;

			{
				boost::lock_guard<boost::mutex> guard(m_mutex);
				cmdContext = m_commandsToExecute.front();
				m_commandsToExecute.pop();
				m_hr = S_OK;
			}

			while (true)
			{
				{
					boost::lock_guard<boost::mutex> guard(m_mutex);
					m_commandsResult.push(cmdContext);
				}

				CEventScope eventScope(m_hWnd);
				if (cmdContext.m_pDebugCommand)
				{
					CComVariant vResult;
					HRESULT hr = cmdContext.m_pDebugCommand->Execute(NULL, &vResult);
					if (FAILED(hr))
					{
						boost::lock_guard<boost::mutex> guard(m_mutex);
						m_hr = hr;
					}
					break;
				}
				else
				{
					CComQIPtr<IServiceProvider> pServiceProvider = m_pControl;
					if (!pServiceProvider)
						break;

					CComPtr<IDebugSessionService> pDebugSessionService;
					HRESULT hr = m_pServiceProvider->QueryService(SERVICE_DEBUG_SESSION_SERVICE, IID_IDebugSessionService, (LPVOID*)&pDebugSessionService);
					if (FAILED(hr))
					{
						boost::lock_guard<boost::mutex> guard(m_mutex);
						m_hr = hr;
						break;
					}
					hr = pDebugSessionService->ExecuteCommandSimple(cmdContext.m_guidCommand, &cmdContext.m_vParam, &cmdContext.m_pResultObject);
					if (FAILED(hr))
					{
						boost::lock_guard<boost::mutex> guard(m_mutex);
						m_hr = hr;
						break;
					}
					{
						boost::lock_guard<boost::mutex> guard(m_mutex);
						auto resultContext = m_commandsResult.front();
						resultContext.m_pResultObject = cmdContext.m_pResultObject;
						m_commandsResult.pop();
						m_commandsResult.push(resultContext);
					}
					break;
				}
			}
		}
	}
	return S_OK;
}

void CViewDebugEngineService::ThreadProcStatic(void* pData)
{
	CViewDebugEngineService* pWorkerTask = (CViewDebugEngineService*)pData;
	pWorkerTask->ThreadProc();
}

STDMETHODIMP CViewDebugEngineService::GetLastError()
{
	boost::lock_guard<boost::mutex> guard(m_mutex);
	return m_hr;
}

STDMETHODIMP CViewDebugEngineService::OnInitialized(IServiceProvider* pServiceProvider)
{
	CHECK_E_POINTER(pServiceProvider);
	m_pServiceProvider = pServiceProvider;
	RETURN_IF_FAILED(m_pServiceProvider->QueryService(SERVICE_DEBUG_SESSION_SERVICE, &m_pDebugSessionService));
	_beginthread(&CViewDebugEngineService::ThreadProcStatic, 0, this);
	return S_OK;
}

STDMETHODIMP CViewDebugEngineService::OnShutdown()
{
	m_pServiceProvider = NULL;
	m_pControl = NULL;
	return S_OK;
}

STDMETHODIMP CViewDebugEngineService::SetControl(IControl* pControl)
{
	CHECK_E_POINTER(pControl);
	m_pControl = pControl;
	RETURN_IF_FAILED(pControl->GetHWND(&m_hWnd));
	return S_OK;
}
