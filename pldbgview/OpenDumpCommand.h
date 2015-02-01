// OpenDumpCommand.h : Declaration of the COpenDumpCommand

#pragma once
#include "resource.h"       // main symbols
#include "pldbgview_i.h"

using namespace ATL;

// COpenDumpCommand

class ATL_NO_VTABLE COpenDumpCommand :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<COpenDumpCommand, &CLSID_OpenDumpCommand>,
	public ICommand2,
	public IInitializeWithControlImpl,
	public IIdleHandler
{
private:
	CBitmap m_Bitmap;
public:
	COpenDumpCommand()
	{
		m_Bitmap.LoadBitmap(IDB_BITMAP2);
	}

	DECLARE_NO_REGISTRY()

	BEGIN_COM_MAP(COpenDumpCommand)
		COM_INTERFACE_ENTRY(ICommand)
		COM_INTERFACE_ENTRY(ICommand2)
		COM_INTERFACE_ENTRY(IIdleHandler)
		COM_INTERFACE_ENTRY(IInitializeWithControl)
	END_COM_MAP()

public:
	STDMETHOD(GetHBITMAP)(REFGUID guidId, HBITMAP* pHBITMAP);
	STDMETHOD(GetCommandText)(REFGUID guidId, BSTR* bstrText);
	STDMETHOD(Invoke)(REFGUID guidId);
	STDMETHOD(InstallToolbar)(IToolbar* pToolbar);
	STDMETHOD(InstallMenu)(IMenu* pMenu);
	STDMETHOD(OnIdle)(BOOL *bResult);
	STDMETHOD(GetEnabled)(REFGUID guidId, BOOL *pbEnabled);
};

OBJECT_ENTRY_AUTO(__uuidof(OpenDumpCommand), COpenDumpCommand)
