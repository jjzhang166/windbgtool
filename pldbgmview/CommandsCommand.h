// CommandsCommand.h : Declaration of the CCommandsCommand

#pragma once
#include "resource.h"       // main symbols
#include "pldbgmview_i.h"

using namespace ATL;


// CCommandsCommand

class ATL_NO_VTABLE CCommandsCommand :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CCommandsCommand, &CLSID_CommandsCommand>,
	public ICommand
{
public:
	CCommandsCommand()
	{
	}

	DECLARE_NO_REGISTRY()

	BEGIN_COM_MAP(CCommandsCommand)
		COM_INTERFACE_ENTRY(ICommand)
	END_COM_MAP()

public:

	STDMETHOD(GetCommandText)(REFGUID guidId, BSTR* bstrText)
	{
		UNREFERENCED_PARAMETER(guidId);
		CHECK_E_POINTER(bstrText);
		*bstrText = CComBSTR(L".NET Commands").Copy();
		return S_OK;
	}

	STDMETHOD(Invoke)(REFGUID guidId)
	{
		UNREFERENCED_PARAMETER(guidId);
		return E_NOTIMPL;
	}

	STDMETHOD(InstallMenu)(IMenu* pMenu)
	{
		CHECK_E_POINTER(pMenu);
		return pMenu->AddMenuCommand(GUID_NULL, COMMAND_COMMANDS, this);
	}

};

OBJECT_ENTRY_AUTO(__uuidof(CommandsCommand), CCommandsCommand)
