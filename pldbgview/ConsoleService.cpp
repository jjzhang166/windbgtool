// ConsoleService.cpp : Implementation of CShowLogService

#include "stdafx.h"
#include "ConsoleService.h"


// CConsoleService

STDMETHODIMP CConsoleService::WriteLine(BSTR bstr)
{
	CComPtr<IFormManager> pFormManager;
	RETURN_IF_FAILED(GetFormManager(&pFormManager));

	RETURN_IF_FAILED(OpenConsole());
	CComPtr<IControl> pConsoleForm;
	RETURN_IF_FAILED(pFormManager->FindForm(CONTROL_LOG, &pConsoleForm));
	CComQIPtr<ILogFormView> pLogFormView = pConsoleForm;
	ATLENSURE(pLogFormView);
	RETURN_IF_FAILED(pLogFormView->WriteLine(bstr));
	return S_OK;
}

STDMETHODIMP CConsoleService::OpenConsole()
{
	CComPtr<IFormManager> pFormManager;
	RETURN_IF_FAILED(GetFormManager(&pFormManager));

	HRESULT hr = pFormManager->ActivateForm(CONTROL_LOG);
	if (hr == S_OK)
		return S_OK;

	CComPtr<IControl> pControl;
	RETURN_IF_FAILED(pFormManager->OpenForm2(PNAMESP_HOSTFORM, PVIEWTYPE_INPLACE_CONTROL, CONTROL_LOG, &pControl));
	return S_OK;
}

STDMETHODIMP CConsoleService::IsOpened(BOOL* pbOpened)
{
	CHECK_E_POINTER(pbOpened);
	CComPtr<IFormManager> pFormManager;
	RETURN_IF_FAILED(GetFormManager(&pFormManager));

	CComPtr<IControl> pControl;
	HRESULT hr = pFormManager->FindForm(CONTROL_LOG, &pControl);
	*pbOpened = hr == S_OK;
	return S_OK;
}

STDMETHODIMP CConsoleService::GetFormManager(IFormManager** ppFormManager)
{
	CHECK_E_POINTER(ppFormManager);
	CComQIPtr<IServiceProvider> pServiceProvider = m_pControl;
	if (!pServiceProvider)
		return E_NOINTERFACE;

	return pServiceProvider->QueryService(SERVICE_FORM_MANAGER, IID_IFormManager, (LPVOID*)ppFormManager);
}
