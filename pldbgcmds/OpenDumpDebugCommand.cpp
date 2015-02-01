#include "stdafx.h"
#include "OpenDumpDebugCommand.h"

STDMETHODIMP COpenDumpDebugCommand::Execute(VARIANT* vParam, IVariantObject** ppVariantObject)
{
	UNREFERENCED_PARAMETER(ppVariantObject);
	RETURN_IF_FAILED(m_pDebugSessionService->OpenDumpFile(vParam->bstrVal));
	return S_OK;
}

STDMETHODIMP COpenDumpDebugCommand::SetDebugSession(IDebugSession* pDebugSession)
{
	UNREFERENCED_PARAMETER(pDebugSession);
	return S_OK;
}

STDMETHODIMP COpenDumpDebugCommand::SetDebugSessionService(IDebugSessionService *pDebugSessionService)
{
	m_pDebugSessionService = pDebugSessionService;
	return S_OK;
}