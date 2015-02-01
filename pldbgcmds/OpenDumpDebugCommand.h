// OpenDumpDebugCommand.h : Declaration of the COpenDumpDebugCommand

#pragma once
#include "resource.h"       // main symbols
#include "pldbgcmds_i.h"

using namespace ATL;

// COpenDumpDebugCommand

class ATL_NO_VTABLE COpenDumpDebugCommand :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<COpenDumpDebugCommand, &CLSID_OpenDumpDebugCommand>,
	public IPluginDebugCommand,
	public IInitializeWithDebugSessionService
{
public:
	COpenDumpDebugCommand()
	{
	}

	DECLARE_NO_REGISTRY()

	BEGIN_COM_MAP(COpenDumpDebugCommand)
		COM_INTERFACE_ENTRY(IInitializeWithDebugSessionService)
		COM_INTERFACE_ENTRY(IPluginDebugCommand)
	END_COM_MAP()

private:
	CComPtr<IDebugSessionService> m_pDebugSessionService;

public:
	STDMETHOD(Execute)(VARIANT* vParam, IVariantObject** ppVariantObject);
	STDMETHOD(SetDebugSession)(IDebugSession* pDebugSession);
	STDMETHOD(SetDebugSessionService)(IDebugSessionService *pDebugSessionService);
};

OBJECT_ENTRY_AUTO(__uuidof(OpenDumpDebugCommand), COpenDumpDebugCommand)
