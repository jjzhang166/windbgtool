// HeapStatParser.h : Declaration of the CHeapStatParser

#pragma once
#include "resource.h"       // main symbols
#include "pldbgmcmds_i.h"

using namespace ATL;

// CHeapStatParser

class ATL_NO_VTABLE CHeapStatParser :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CHeapStatParser, &CLSID_HeapStatParser>,
	public ICommandResultParser
{
public:
	CHeapStatParser()
	{
	}

	DECLARE_NO_REGISTRY()

	BEGIN_COM_MAP(CHeapStatParser)
		COM_INTERFACE_ENTRY(ICommandResultParser)
	END_COM_MAP()

public:

	STDMETHOD(Parse)(VARIANT *v, IVariantObject** ppVariantObject);

};

OBJECT_ENTRY_AUTO(__uuidof(HeapStatParser), CHeapStatParser)
