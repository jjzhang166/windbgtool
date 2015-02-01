#include "stdafx.h"
#include "CommandErrorHandlingService.h"

STDMETHODIMP CCommandErrorHandlingService::OnInitialized(IServiceProvider* pServiceProvider)
{
	CHECK_E_POINTER(pServiceProvider);
	RETURN_IF_FAILED(pServiceProvider->QueryService(SERVICE_VIEW_DEBUG_SESSION_SERVICE, &m_pViewDebugEngineService));

	CComPtr<IUnknown> pThis;
	RETURN_IF_FAILED(QueryInterface(IID_IUnknown, (LPVOID*)&pThis));
	RETURN_IF_FAILED(m_pViewDebugEngineService.Advise(pThis, IID_IViewDebugEngineServiceEvents, &m_dwAdvice));
	m_pServiceProvider = pServiceProvider;
	return S_OK;
}

STDMETHODIMP CCommandErrorHandlingService::OnShutdown()
{
	if (m_dwAdvice)
	{
		RETURN_IF_FAILED(AtlUnadvise(m_pViewDebugEngineService, IID_IViewDebugEngineServiceEvents, m_dwAdvice));
		m_dwAdvice = 0;
	}
	return S_OK;
}

STDMETHODIMP CCommandErrorHandlingService::OnBeforeCommandExecute(REFGUID guidCommandId, VARIANT* vParam)
{
	UNREFERENCED_PARAMETER(guidCommandId);
	UNREFERENCED_PARAMETER(vParam);
	return S_OK;
}

STDMETHODIMP CCommandErrorHandlingService::OnAfterCommandExecute(REFGUID guidCommandId, VARIANT* vParam, IVariantObject* pResult)
{
	UNREFERENCED_PARAMETER(guidCommandId);
	UNREFERENCED_PARAMETER(vParam);
	UNREFERENCED_PARAMETER(pResult);
	CComPtr<IConsoleService> pConsoleService;
	RETURN_IF_FAILED(m_pServiceProvider->QueryService(SERVICE_CONSOLE, &pConsoleService));
	HRESULT hrLastError = m_pViewDebugEngineService->GetLastError();
	if (FAILED(hrLastError))
	{
		RETURN_IF_FAILED(pConsoleService->WriteLine(CComBSTR(_com_error(hrLastError).ErrorMessage())));
	}
	return S_OK;
}
