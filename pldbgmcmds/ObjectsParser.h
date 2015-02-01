// ObjectsParser.h : Declaration of the CObjectsParser

#pragma once
#include "resource.h"       // main symbols
#include "pldbgmcmds_i.h"

using namespace ATL;

// CObjectsParser

class ATL_NO_VTABLE CObjectsParser :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CObjectsParser, &CLSID_ObjectsParser>,
	public ICommandResultParser
{
public:
	CObjectsParser()
	{
	}

	DECLARE_NO_REGISTRY()

	BEGIN_COM_MAP(CObjectsParser)
		COM_INTERFACE_ENTRY(ICommandResultParser)
	END_COM_MAP()

public:

	STDMETHOD(Parse)(VARIANT *v, IVariantObject** ppVariantObject);

};

OBJECT_ENTRY_AUTO(__uuidof(ObjectsParser), CObjectsParser)
