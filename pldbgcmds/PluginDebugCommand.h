// PluginDebugCommand.h : Declaration of the CPluginDebugCommand

#pragma once
#include "resource.h"       // main symbols
#include "pldbgcmds_i.h"

using namespace ATL;

// CPluginDebugCommand

class ATL_NO_VTABLE CPluginDebugCommand :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CPluginDebugCommand, &CLSID_PluginDebugCommand>,
	public IPluginDebugCommand,
	public IInitializeWithPluginInfo
{
public:
	CPluginDebugCommand()
	{
	}

	DECLARE_NO_REGISTRY()

	BEGIN_COM_MAP(CPluginDebugCommand)
		COM_INTERFACE_ENTRY(IInitializeWithPluginInfo)
		COM_INTERFACE_ENTRY(IPluginDebugCommand)
		COM_INTERFACE_ENTRY_AGGREGATE(IID_IDebugCommand, m_pDebugCommand)
	END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct();
	void FinalRelease(){}

private:
	CComPtr<IPluginManager> m_pPluginManager;
	CComPtr<IDebugCommand> m_pDebugCommand;
	CComPtr<IPluginInfo> m_pPluginInfo;

public:
	STDMETHOD(GetPluginInfo)(IPluginInfo** ppPluginInfo);
	STDMETHOD(SetPluginInfo)(IPluginInfo* pPluginInfo);
	STDMETHOD(Execute)(VARIANT* vParam, IVariantObject** ppVariantObject);
	STDMETHOD(SetDebugSession)(IDebugSession* pDebugSession);
};

OBJECT_ENTRY_AUTO(__uuidof(PluginDebugCommand), CPluginDebugCommand)
