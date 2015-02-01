// DebugFormManager.cpp : Implementation of CDebugFormManager

#include "stdafx.h"
#include "DebugFormManager.h"


// CDebugFormManager

STDMETHODIMP CDebugFormManager::OnBeforeCommandExecute(REFGUID guidCommandId, VARIANT* vParam)
{
	UNREFERENCED_PARAMETER(guidCommandId);
	UNREFERENCED_PARAMETER(vParam);
	return S_OK;
}

STDMETHODIMP CDebugFormManager::OnAfterCommandExecute(REFGUID guidCommandId, VARIANT* vParam, IVariantObject* pResult)
{
	UNREFERENCED_PARAMETER(guidCommandId);
	UNREFERENCED_PARAMETER(vParam);
	UNREFERENCED_PARAMETER(pResult);

	if(!m_bSelfCall)
		return S_OK;

	CWaitCursor w;

	m_bSelfCall = FALSE;

	RETURN_IF_FAILED(m_pViewDebugEngineService->GetLastError());
	CComPtr<IFormManager> pFormManager;
	RETURN_IF_FAILED(m_pServiceProvider->QueryService(SERVICE_FORM_MANAGER, IID_IFormManager, (LPVOID*)&pFormManager));
	CComPtr<IControl> pControl;
	RETURN_IF_FAILED(pFormManager->OpenForm2(PNAMESP_HOSTFORM, PVIEWTYPE_INPLACE_CONTROL, guidCommandId, &pControl));

	CComQIPtr<IInitializeWithVariantObject> pInitializeWithVariantObject = pControl;
	ATLASSERT(pInitializeWithVariantObject);
	if(pInitializeWithVariantObject)
	{
		RETURN_IF_FAILED(pInitializeWithVariantObject->SetVariantObject(pResult));
	}

	return S_OK;
}

STDMETHODIMP CDebugFormManager::OpenForm(REFGUID guidCommandId, VARIANT* vParam)
{
	RETURN_IF_FAILED(m_pViewDebugEngineService->ExecuteCommandSimpleAsync(guidCommandId, vParam));
	m_bSelfCall = TRUE;
	return S_OK;
}

STDMETHODIMP CDebugFormManager::OnInitialized(IServiceProvider* pServiceProvider)
{
	CHECK_E_POINTER(pServiceProvider);
	m_pServiceProvider = pServiceProvider;
	RETURN_IF_FAILED(m_pServiceProvider->QueryService(SERVICE_VIEW_DEBUG_SESSION_SERVICE, &m_pViewDebugEngineService));

	CComPtr<IUnknown> pThis;
	RETURN_IF_FAILED(QueryInterface(IID_IUnknown, (LPVOID*)&pThis));
	RETURN_IF_FAILED(m_pViewDebugEngineService.Advise(pThis, IID_IViewDebugEngineServiceEvents, &m_dwCookie));

	return S_OK;
}

STDMETHODIMP CDebugFormManager::OnShutdown()
{
	CComPtr<IViewDebugEngineService> pViewDebugEngineService;
	RETURN_IF_FAILED(m_pServiceProvider->QueryService(SERVICE_VIEW_DEBUG_SESSION_SERVICE, &pViewDebugEngineService));
	RETURN_IF_FAILED(AtlUnadvise(pViewDebugEngineService, IID_IViewDebugEngineServiceEvents, m_dwCookie));
	m_pServiceProvider = NULL;

	return S_OK;
}
