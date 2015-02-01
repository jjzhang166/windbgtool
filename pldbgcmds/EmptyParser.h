// EmptyParser.h : Declaration of the CEmptyParser

#pragma once
#include "resource.h"       // main symbols
#include "pldbgcmds_i.h"

using namespace ATL;

// CEmptyParser

class ATL_NO_VTABLE CEmptyParser :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CEmptyParser, &CLSID_EmptyParser>,
	public ICommandResultParser
{
public:
	CEmptyParser()
	{
	}

	DECLARE_NO_REGISTRY()

	BEGIN_COM_MAP(CEmptyParser)
		COM_INTERFACE_ENTRY(ICommandResultParser)
	END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct();
	void FinalRelease(){}

private:
	CComPtr<IPluginManager> m_pPluginManager;

public:

	STDMETHOD(Parse)(VARIANT *v, IVariantObject** ppVariantObject);

};

OBJECT_ENTRY_AUTO(__uuidof(EmptyParser), CEmptyParser)
