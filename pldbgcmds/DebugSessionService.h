// DebugEngineService.h : Declaration of the CDebugSessionService

#pragma once
#include "resource.h"       // main symbols
#include "pldbgcmds_i.h"
#include "DebugSessionServiceEventSink.h"

using namespace ATL;

// CDebugSessionService

class ATL_NO_VTABLE CDebugSessionService :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDebugSessionService, &CLSID_DebugSessionService>,
	public IDebugSessionService,
	public IConnectionPointContainerImpl<CDebugSessionService>,
	public CDebugSessionServiceEventsEventSink<CDebugSessionService>
{
private:
	CComPtr<IPluginManager> m_pPluginManager;
	CComPtr<IDebugEngine> m_pDebugEngine;
	CComPtr<IDebugSession> m_pDebugSession;

public:
	CDebugSessionService()
	{
	}

	DECLARE_NO_REGISTRY()

	BEGIN_COM_MAP(CDebugSessionService)
		COM_INTERFACE_ENTRY(IDebugSessionService)
		COM_INTERFACE_ENTRY(IConnectionPointContainer)
		COM_INTERFACE_ENTRY_AGGREGATE(IID_IDebugEngine, m_pDebugEngine)
	END_COM_MAP()

	BEGIN_CONNECTION_POINT_MAP(CDebugSessionService)
		CONNECTION_POINT_ENTRY(__uuidof(IDebugSessionServiceEvents))
	END_CONNECTION_POINT_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct();
	void FinalRelease(){}

public:
	STDMETHOD(OpenSession)();
	STDMETHOD(CloseSession)();
	STDMETHOD(OpenDumpFile)(BSTR bstrDumpFile);
	STDMETHOD(CloseDumpFile)();
	STDMETHOD(GetSession)(IDebugSession** ppDebugSession);
	STDMETHOD(LoadDll)(LPCTSTR lpszDllName);
	STDMETHOD(ExecuteCommandSimple)(REFIID iidCommand, VARIANT* vParam, IVariantObject** ppVariantObject);
};

OBJECT_ENTRY_AUTO(__uuidof(DebugSessionService), CDebugSessionService)
