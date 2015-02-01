#pragma once

#include "..\ObjMdl\PluginTableBase.h"

using namespace ATL;

#include "pldbgview_i.h"

class ATL_NO_VTABLE CPluginTable :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CPluginTable, &CLSID_PluginTable>,
	public CPluginTableBase
{
public:
	DECLARE_NO_REGISTRY()

	BEGIN_COM_MAP(CPluginTable)
		COM_INTERFACE_ENTRY(IPluginTable)
	END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct();

	void FinalRelease()
	{
	}

	CPluginTable()
	{
	}
};

OBJECT_ENTRY_AUTO(__uuidof(PluginTable), CPluginTable)
