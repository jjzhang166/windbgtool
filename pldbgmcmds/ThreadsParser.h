// ThreadsParser.h : Declaration of the CThreadsParser

#pragma once
#include "resource.h"       // main symbols
#include "pldbgmcmds_i.h"

using namespace ATL;

// CThreadsParser

class ATL_NO_VTABLE CThreadsParser :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CThreadsParser, &CLSID_ThreadsParser>,
	public ICommandResultParser
{
public:
	CThreadsParser()
	{
	}

	DECLARE_NO_REGISTRY()

	BEGIN_COM_MAP(CThreadsParser)
		COM_INTERFACE_ENTRY(ICommandResultParser)
	END_COM_MAP()

public:

	STDMETHOD(Parse)(VARIANT *v, IVariantObject** ppVariantObject);

};

OBJECT_ENTRY_AUTO(__uuidof(ThreadsParser), CThreadsParser)
