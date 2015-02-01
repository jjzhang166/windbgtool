// ObjectValuesParser.h : Declaration of the CObjectValuesParser

#pragma once
#include "resource.h"       // main symbols
#include "pldbgmcmds_i.h"

using namespace ATL;


// CObjectValuesParser

class ATL_NO_VTABLE CObjectValuesParser :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CObjectValuesParser, &CLSID_ObjectValuesParser>,
	public ICommandResultParser
{
public:
	CObjectValuesParser()
	{
	}

	DECLARE_NO_REGISTRY()

	BEGIN_COM_MAP(CObjectValuesParser)
		COM_INTERFACE_ENTRY(ICommandResultParser)
	END_COM_MAP()

public:

	STDMETHOD(Parse)(VARIANT *v, IVariantObject** ppVariantObject);

};

OBJECT_ENTRY_AUTO(__uuidof(ObjectValuesParser), CObjectValuesParser)
