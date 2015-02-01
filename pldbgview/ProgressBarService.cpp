// ProgressBarService.cpp : Implementation of CProgressBarService

#include "stdafx.h"
#include "ProgressBarService.h"
#include <vector>

// CProgressBarService

STDMETHODIMP CProgressBarService::AddPartIfNeed()
{
	int nCount = 0;
	RETURN_IF_FAILED(m_pStatusBar->GetPartsCount(&nCount));
	std::unique_ptr<int[]> puArrayWidth(new int[nCount]);
	auto pArrayWidth = puArrayWidth.get();
	RETURN_IF_FAILED(m_pStatusBar->GetParts(nCount, pArrayWidth));
	std::vector<int> pVector;
	for (int i = 0; i < nCount; i++)
	{
		pVector.push_back(pArrayWidth[i]);
	}

	if(!pVector.size() || (pVector.size() && *(--pVector.cend()) != -1))
	{
		pVector.push_back(-1);
	}

	if((size_t)nCount != pVector.size())
	{
		std::unique_ptr<int[]> puNewArrayWidth(new int[pVector.size()]);
		int i = 0;
		for(auto it = pVector.cbegin(); it != pVector.cend(); it++)
		{
			puNewArrayWidth[i] = *it;
			i++;
		}
		RETURN_IF_FAILED(m_pStatusBar->SetParts(i, puNewArrayWidth.get()))
	}
	return S_OK;
}

STDMETHODIMP CProgressBarService::OnInitialized(IServiceProvider* pServiceProvider)
{
	CHECK_E_POINTER(pServiceProvider);
	RETURN_IF_FAILED(pServiceProvider->QueryService(SERVICE_VIEW_DEBUG_SESSION_SERVICE, &m_pViewDebugEngineService));

	CComPtr<IUnknown> pThis;
	RETURN_IF_FAILED(QueryInterface(IID_IUnknown, (LPVOID*)&pThis));
	RETURN_IF_FAILED(m_pViewDebugEngineService.Advise(pThis, IID_IViewDebugEngineServiceEvents, &m_dwAdvice));

	CComQIPtr<IMainWindow> pMainWindow = m_pControl;
	if(pMainWindow)
	{
		RETURN_IF_FAILED(pMainWindow->GetStatusBar(&m_pStatusBar));
		RETURN_IF_FAILED(m_pStatusBar->GetHWND(&m_hWndStatusBar));
		RETURN_IF_FAILED(AddPartIfNeed());
		RECT rc = {0};
		RETURN_IF_FAILED(m_pStatusBar->GetPartRect(1, &rc));
		if(rc.right - rc.left > 200)
		{
			rc.left = rc.right - 200;
		}
		m_progress.Create(m_hWndStatusBar, rc, NULL, WS_CHILD|PBS_SMOOTH|PBS_MARQUEE);
	}

	return S_OK;
}

STDMETHODIMP CProgressBarService::OnShutdown()
{
	m_pStatusBar = NULL;
	if(m_dwAdvice)
	{
		RETURN_IF_FAILED(AtlUnadvise(m_pViewDebugEngineService, IID_IViewDebugEngineServiceEvents, m_dwAdvice));
		m_dwAdvice = 0;
	}

	return S_OK;
}

STDMETHODIMP CProgressBarService::OnBeforeCommandExecute(REFGUID guidCommandId, VARIANT* vParam)
{
	UNREFERENCED_PARAMETER(guidCommandId);
	UNREFERENCED_PARAMETER(vParam);
	m_progress.ShowWindow(SW_SHOW);
	m_progress.SetMarquee(TRUE);
	return S_OK;
}

STDMETHODIMP CProgressBarService::OnAfterCommandExecute(REFGUID guidCommandId, VARIANT* vParam, IVariantObject* pResult)
{
	UNREFERENCED_PARAMETER(guidCommandId);
	UNREFERENCED_PARAMETER(vParam);
	UNREFERENCED_PARAMETER(pResult);
	m_progress.SetMarquee(FALSE);
	m_progress.ShowWindow(SW_HIDE);
	return S_OK;
}

STDMETHODIMP CProgressBarService::PreTranslateMessage(MSG* pMsg, BOOL* pbResult)
{
	CHECK_E_POINTER(pMsg);
	CHECK_E_POINTER(pbResult);

	if(pMsg->hwnd == m_hWndStatusBar)
	{
		int nCount = 0;
		RETURN_IF_FAILED(m_pStatusBar->GetPartsCount(&nCount));
		RECT rc = {0};
		RETURN_IF_FAILED(m_pStatusBar->GetPartRect(nCount - 1, &rc));
		if(rc.right - rc.left > 200)
		{
			rc.left = rc.right - 200;
		}
		m_progress.MoveWindow(&rc);
	}
	return S_OK;
}
