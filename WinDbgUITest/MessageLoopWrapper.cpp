#include "stdafx.h"
#include "MessageLoopWrapper.h"

STDMETHODIMP CMessageLoopWrapper::AddMessageFilter(IMsgFilter* pMsgFilter)
{
	CHECK_E_POINTER(pMsgFilter);
	m_pFilters.push_back(CComPtr<IMsgFilter>(pMsgFilter));
	return S_OK;
}

STDMETHODIMP CMessageLoopWrapper::AddIdleHandler(IIdleHandler* pIdleHandler)
{
	CHECK_E_POINTER(pIdleHandler);
	m_pIdleHandlers.push_back(CComPtr<IIdleHandler>(pIdleHandler));
	return S_OK;
}

STDMETHODIMP CMessageLoopWrapper::RemoveMessageFilter(IMsgFilter* pMsgFilter)
{
	CHECK_E_POINTER(pMsgFilter);
	m_pFilters.remove(CComPtr<IMsgFilter>(pMsgFilter));
	return S_OK;
}

STDMETHODIMP CMessageLoopWrapper::RemoveIdleHandler(IIdleHandler* pIdleHandler)
{
	CHECK_E_POINTER(pIdleHandler);
	m_pIdleHandlers.remove(CComPtr<IIdleHandler>(pIdleHandler));
	return S_OK;
}

void CMessageLoopWrapper::SetMessageLoop(CMessageLoop* messageLoop)
{
	m_pMessageLoop = messageLoop; 
	m_pMessageLoop->AddMessageFilter(this);
	m_pMessageLoop->AddIdleHandler(this);
}

BOOL CMessageLoopWrapper::PreTranslateMessage(MSG* pMsg)
{
	for(auto it = m_pFilters.begin(); it != m_pFilters.end(); it++)
	{
		BOOL bRes = FALSE;
		if (FAILED(it->m_T->PreTranslateMessage(pMsg, &bRes)))
		{
			ATLENSURE(FALSE);
			return FALSE;
		}
		if(bRes)
			return TRUE;
	}
	return FALSE;
}

BOOL CMessageLoopWrapper::OnIdle()
{
	for(auto it = m_pIdleHandlers.begin(); it != m_pIdleHandlers.end(); it++)
	{
		BOOL bRes = FALSE;
		if(FAILED(it->m_T->OnIdle(&bRes)))
		{
			ATLENSURE(FALSE);
			return FALSE;
		}
	}
	return FALSE;
}
