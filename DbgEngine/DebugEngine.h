// DebugEngine.h : Declaration of the CDebugEngine

#pragma once
#include "resource.h"       // main symbols


#include "DbgEngine_i.h"

using namespace ATL;

// CDebugEngine

class ATL_NO_VTABLE CDebugEngine :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDebugEngine, &CLSID_DebugEngine>,
	public IDebugEngine
{
public:
	CDebugEngine()
	{
	}

	DECLARE_NO_REGISTRY()

	BEGIN_COM_MAP(CDebugEngine)
		COM_INTERFACE_ENTRY(IDebugEngine)
	END_COM_MAP()

public:

	STDMETHOD(OpenSession)(IDebugSession** pSession);

};

OBJECT_ENTRY_AUTO(__uuidof(DebugEngine), CDebugEngine)
