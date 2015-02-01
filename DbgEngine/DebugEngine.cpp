// DebugEngine.cpp : Implementation of CDebugEngine

#include "stdafx.h"
#include "DebugEngine.h"
#include "DebugSession.h"

// CDebugEngine

STDMETHODIMP CDebugEngine::OpenSession(IDebugSession** pSession)
{
	CHECK_E_POINTER(pSession);
	RETURN_IF_FAILED(CDebugSession::_CreatorClass::CreateInstance(NULL, IID_IDebugSession, (LPVOID*)pSession));
	return S_OK;
}