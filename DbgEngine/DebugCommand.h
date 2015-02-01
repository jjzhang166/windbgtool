// DbgCommand.h : Declaration of the CDbgCommand

#pragma once
#include "resource.h"       // main symbols
#include "DbgEngine_i.h"

using namespace ATL;


// CDbgCommand

class ATL_NO_VTABLE CDebugCommand :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDebugCommand, &CLSID_DebugCommand>,
	public IDebugCommand,
	public IDebugOutputCallback
{
public:
	CDebugCommand()
	{
	}

	DECLARE_NO_REGISTRY()

	BEGIN_COM_MAP(CDebugCommand)
		COM_INTERFACE_ENTRY(IDebugCommand)
		COM_INTERFACE_ENTRY(IDebugOutputCallback)
	END_COM_MAP()

private:
	CString m_strCommandText;
	CString m_strCommandResult;
	CComPtr<IDebugSession> m_pDebugSession;
public:

	STDMETHOD(SetDebugSession)(IDebugSession* pDebugSession);

	STDMETHOD(SetCommandText)(BSTR bstrCmdText);
	STDMETHOD(Execute)(VARIANT* vParam, VARIANT* vResult);

	STDMETHOD(Output)(ULONG Mask, LPCTSTR Text);

};

OBJECT_ENTRY_AUTO(__uuidof(DebugCommand), CDebugCommand)
