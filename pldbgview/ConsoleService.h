// ShowLogService.h : Declaration of the CShowLogService

#pragma once
#include "resource.h"       // main symbols
#include "pldbgview_i.h"

// CConsoleService

using namespace ATL;

class ATL_NO_VTABLE CConsoleService :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CConsoleService, &CLSID_ConsoleService>,
	public IInitializeWithControlImpl,
	public IConsoleService
{
public:
	CConsoleService()
	{
	}

	DECLARE_NO_REGISTRY()

	BEGIN_COM_MAP(CConsoleService)
		COM_INTERFACE_ENTRY(IConsoleService)
		COM_INTERFACE_ENTRY(IInitializeWithControl)
	END_COM_MAP()

private:
	STDMETHOD(GetFormManager)(IFormManager** ppFormManager);

public:

	STDMETHOD(OpenConsole)();
	STDMETHOD(IsOpened)(BOOL* pbOpened);
	STDMETHOD(WriteLine)(BSTR bstr);
};

OBJECT_ENTRY_AUTO(__uuidof(ConsoleService), CConsoleService)
