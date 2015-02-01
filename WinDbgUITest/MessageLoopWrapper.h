#pragma once

#include "WinDbgUITest_i.h"
#include "viewmdl_contract_i.h"

class ATL_NO_VTABLE CMessageLoopWrapper :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMessageLoopWrapper, &CLSID_MessageLoopWrapper>,
	public IMessageLoop,
	public CMessageFilter,
	public CIdleHandler
{
private:
	CMessageLoop* m_pMessageLoop;
	std::list< CAdapt<CComPtr<IMsgFilter> > > m_pFilters;
	std::list< CAdapt<CComPtr<IIdleHandler> > > m_pIdleHandlers;

public:
	CMessageLoopWrapper()
	{
	}

	~CMessageLoopWrapper()
	{
		m_pMessageLoop->RemoveIdleHandler(this);
		m_pMessageLoop->RemoveMessageFilter(this);
	}

	DECLARE_NO_REGISTRY()

	BEGIN_COM_MAP(CMessageLoopWrapper)
		COM_INTERFACE_ENTRY(IMessageLoop)
	END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	STDMETHOD(AddMessageFilter)(IMsgFilter* pMsgFilter);
	STDMETHOD(AddIdleHandler)(IIdleHandler* pIdleHandler);
	STDMETHOD(RemoveMessageFilter)(IMsgFilter* pMsgFilter);
	STDMETHOD(RemoveIdleHandler)(IIdleHandler* pIdleHandler);
	
	void SetMessageLoop(CMessageLoop* messageLoop);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();
};

OBJECT_ENTRY_AUTO(__uuidof(MessageLoopWrapper), CMessageLoopWrapper)