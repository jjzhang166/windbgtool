// DebugSession.h : Declaration of the CDebugSession

#pragma once
#include "resource.h"       // main symbols
#include "DbgEngine_i.h"

using namespace ATL;

// CDebugSession

class ATL_NO_VTABLE CDebugSession :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDebugSession, &CLSID_DebugSession>,
	public IDebugSession
{
public:
	CDebugSession()
	{
		m_bBulkOutput = FALSE;
		m_Mask = 0;
	}

	DECLARE_NO_REGISTRY()
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(CDebugSession)
		COM_INTERFACE_ENTRY(IDebugSession)
		COM_INTERFACE_ENTRY_AGGREGATE(__uuidof(IDebugOutputCallbacks), m_pDebugOutputCallbacks)
		COM_INTERFACE_ENTRY_AGGREGATE(__uuidof(IDebugControl), m_pDebugClient)
		COM_INTERFACE_ENTRY_AGGREGATE(__uuidof(IDebugControl2), m_pDebugClient)
		COM_INTERFACE_ENTRY_AGGREGATE(__uuidof(IDebugControl3), m_pDebugClient)
		COM_INTERFACE_ENTRY_AGGREGATE(__uuidof(IDebugControl4), m_pDebugClient)
	END_COM_MAP()

	void FinalRelease();
	HRESULT FinalConstruct();

private:
	CComPtr<IPluginManager> m_pPluginManager;
	CComPtr<IDebugClient4> m_pDebugClient;
	std::set< CAdapt< CComPtr<IDebugOutputCallback> > > m_pOutputCallbacks;
	BOOL m_bBulkOutput;
	CString m_strBulkBuffer;
	ULONG m_Mask;

	STDMETHOD(FlushOutput)();
public:
	STDMETHOD(Open)();
	STDMETHOD(Close)();
	STDMETHOD(OpenDumpFile)(LPCTSTR dumpFilePath);
	STDMETHOD(CloseDumpFile)();
	STDMETHOD(CreateCommand)(IDebugCommand** pCmd);
	STDMETHOD(AddOutputCallback)(IDebugOutputCallback* pCallback);
	STDMETHOD(RemoveOutputCallback)(IDebugOutputCallback* pCallback);

	STDMETHOD(Output)(ULONG Mask, LPCSTR Text);
	STDMETHOD(Execute)(ULONG OutputControl, LPCTSTR Command, ULONG Flags);

private:
	class CBulkOutput
	{
	public:
		CDebugSession* m_pDebugSession;
		CBulkOutput(CDebugSession* pDebugSession)
		{
			m_pDebugSession = pDebugSession;
			m_pDebugSession->m_bBulkOutput = TRUE;
		}

		~CBulkOutput()
		{
			m_pDebugSession->FlushOutput();
			m_pDebugSession->m_bBulkOutput = FALSE;
			m_pDebugSession->m_strBulkBuffer.Empty();
		}
	};

	//There is bug is DbgEng.dll - double AddRef while outputing - custom reference tracking to perform correct cleanup
	class COutputCallbacks : public IDebugOutputCallbacks
	{
	private:
		CDebugSession* m_pDebugSession;
	public:
		COutputCallbacks(CDebugSession* pDebugSession)
		{
			m_pDebugSession = pDebugSession;
		}

		STDMETHOD(QueryInterface)(REFIID InterfaceId, PVOID* Interface)
		{
			*Interface = NULL;
			if (IsEqualIID(InterfaceId, __uuidof(IUnknown)) ||
				IsEqualIID(InterfaceId, __uuidof(IDebugOutputCallbacks)))
			{
				*Interface = (IDebugOutputCallbacks *)this;
				AddRef();
				return S_OK;
			}
			else
			{
				return E_NOINTERFACE;
			}
		}
		STDMETHOD_(ULONG, AddRef)()
		{
			return 1;
		}
		STDMETHOD_(ULONG, Release)()
		{
			return 1;
		}

		STDMETHOD(Output)(ULONG Mask, PCSTR Text)
		{
			return m_pDebugSession->Output(Mask, Text);
		}
	};

	std::shared_ptr<COutputCallbacks> m_pDebugOutputCallbacks;
};

OBJECT_ENTRY_AUTO(__uuidof(DebugSession), CDebugSession)
