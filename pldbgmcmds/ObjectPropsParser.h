// ObjectPropsParser.h : Declaration of the CObjectPropsParser

#pragma once
#include "resource.h"       // main symbols
#include "pldbgmcmds_i.h"

using namespace ATL;


// CObjectPropsParser

class ATL_NO_VTABLE CObjectPropsParser :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CObjectPropsParser, &CLSID_ObjectPropsParser>,
	public ICommandResultParser
{
public:
	CObjectPropsParser()
	{
	}

	DECLARE_NO_REGISTRY()

	BEGIN_COM_MAP(CObjectPropsParser)
		COM_INTERFACE_ENTRY(ICommandResultParser)
	END_COM_MAP()

public:

	STDMETHOD(Parse)(VARIANT *v, IVariantObject** ppVariantObject);

};

OBJECT_ENTRY_AUTO(__uuidof(ObjectPropsParser), CObjectPropsParser)
