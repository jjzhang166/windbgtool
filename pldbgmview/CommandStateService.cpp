// CommandStateService.cpp : Implementation of CCommandStateService

#include "stdafx.h"
#include "CommandStateService.h"


// CCommandStateService

CCommandStateService::CCommandStateService()
{
	m_hashSet.insert(COMMAND_COMMANDS);
	m_hashSet.insert(COMMAND_HELP);
	m_hashSet.insert(COMMAND_OPENDUMP);
	m_hashSet.insert(COMMAND_CLOSEDUMP);
	m_hashSet.insert(COMMAND_HEAPSTAT);
	m_hashSet.insert(COMMAND_THREADS);
	m_hashSet.insert(COMMAND_DOTNETINFO);

	m_hashSet.insert(CMD_OBJECTS);
	m_hashSet.insert(CMD_GCROOT);
	m_hashSet.insert(CMD_OBJ_PROPS);
	m_hashSet.insert(CMD_OBJ_VALUES);
}

STDMETHODIMP CCommandStateService::OnInitialized(IServiceProvider* pServiceProvider)
{
	CHECK_E_POINTER(pServiceProvider);
	RETURN_IF_FAILED(pServiceProvider->QueryService(SERVICE_VIEW_DEBUG_SESSION_SERVICE, &m_pViewDebugEngineService));

	CComPtr<IUnknown> pThis;
	RETURN_IF_FAILED(QueryInterface(IID_IUnknown, (LPVOID*)&pThis));
	RETURN_IF_FAILED(m_pViewDebugEngineService.Advise(pThis, IID_IViewDebugEngineServiceEvents, &m_dwAdvice));
	m_pCommandSupport = m_pControl;
	ATLENSURE(m_pCommandSupport);
	return S_OK;
}

STDMETHODIMP CCommandStateService::OnShutdown()
{
	if (m_dwAdvice)
	{
		RETURN_IF_FAILED(AtlUnadvise(m_pViewDebugEngineService, IID_IViewDebugEngineServiceEvents, m_dwAdvice));
		m_dwAdvice = 0;
	}

	m_pControl = NULL;
	m_pCommandSupport.Release();

	return S_OK;
}

STDMETHODIMP CCommandStateService::OnBeforeCommandExecute(REFGUID guidCommandId, VARIANT* vParam)
{
	UNREFERENCED_PARAMETER(guidCommandId);
	UNREFERENCED_PARAMETER(vParam);

	RETURN_IF_FAILED(EnableCommands(FALSE));
	return S_OK;
}

STDMETHODIMP CCommandStateService::OnAfterCommandExecute(REFGUID guidCommandId, VARIANT* vParam, IVariantObject* pResult)
{
	UNREFERENCED_PARAMETER(guidCommandId);
	UNREFERENCED_PARAMETER(vParam);
	UNREFERENCED_PARAMETER(pResult);

	RETURN_IF_FAILED(EnableCommands(TRUE));
	return S_OK;
}

STDMETHODIMP CCommandStateService::EnableCommands(BOOL bEnabled)
{
	for (auto it = m_hashSet.begin(); it != m_hashSet.end(); it++)
	{
		RETURN_IF_FAILED(m_pCommandSupport->EnableCommand(*it, bEnabled));
	}
	HWND hWnd = 0;
	RETURN_IF_FAILED(m_pControl->GetHWND(&hWnd));
	DrawMenuBar(hWnd);

	CComQIPtr<IMainWindow> pMainWindow = m_pControl;
	ATLENSURE(pMainWindow);
	CComPtr<IContainerControl> pContainerControl;
	RETURN_IF_FAILED(pMainWindow->GetContainerControl(&pContainerControl));
	CComQIPtr<ITabbedControl> pTabbedControl = pContainerControl;
	ATLENSURE(pTabbedControl);
	DWORD dwCount = 0;
	RETURN_IF_FAILED(pTabbedControl->GetPageCount(&dwCount));
	for (DWORD i = 0; i < dwCount; i++)
	{
		CComPtr<IControl> pControl;
		RETURN_IF_FAILED(pTabbedControl->GetPage(i, &pControl));
		CComQIPtr<ICommandSupport> pCommandSupport = pControl;
		if (!pCommandSupport)
			continue;

		for (auto it = m_hashSet.begin(); it != m_hashSet.end(); it++)
		{
			RETURN_IF_FAILED(pCommandSupport->EnableCommand(*it, bEnabled));
		}
	}

	return S_OK;
}
