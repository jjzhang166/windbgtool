// KFramesParser.h : Declaration of the CKFramesParser

#pragma once
#include "resource.h"       // main symbols
#include "pldbgmcmds_i.h"

using namespace ATL;

// CKFramesParser

class ATL_NO_VTABLE CDotNetInfoParser :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDotNetInfoParser, &CLSID_DotNetInfoParser>,
	public ICommandResultParser
{
public:
	CDotNetInfoParser()
	{
	}

	DECLARE_NO_REGISTRY()

	BEGIN_COM_MAP(CDotNetInfoParser)
		COM_INTERFACE_ENTRY(ICommandResultParser)
	END_COM_MAP()

public:

	STDMETHOD(Parse)(VARIANT *v, IVariantObject** ppVariantObject);

};

OBJECT_ENTRY_AUTO(__uuidof(DotNetInfoParser), CDotNetInfoParser)
