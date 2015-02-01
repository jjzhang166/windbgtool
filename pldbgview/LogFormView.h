// LogFormView.h : Declaration of the CLogFormView

#pragma once
#include "resource.h"       // main symbols
#include "EditEx.h"
#include "pldbgview_i.h"
#include <vector>

using namespace ATL;

// CLogFormView

class ATL_NO_VTABLE CLogFormView :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CLogFormView, &CLSID_LogFormView>,
	public CAxDialogImpl<CLogFormView>,
	public CDialogResize<CLogFormView>,
	public ILogFormView,
	public IControl2,
	public IInitializeWithControlImpl,
	public IDebugOutputCallback,
	public IViewDebugEngineServiceEvents
{
public:
	enum {IDD = IDD_DIALOG1};

	CLogFormView()
	{
		m_bInitialized = FALSE;
		m_dwAdvice = 0;
		m_bIsInProgress = FALSE;
		m_commandHistoryIndex = 0;
	}

	DECLARE_NO_REGISTRY()

	BEGIN_COM_MAP(CLogFormView)
		COM_INTERFACE_ENTRY(ILogFormView)
		COM_INTERFACE_ENTRY(IControl)
		COM_INTERFACE_ENTRY(IControl2)
		COM_INTERFACE_ENTRY(IDebugOutputCallback)
		COM_INTERFACE_ENTRY(IInitializeWithControl)
		COM_INTERFACE_ENTRY(IViewDebugEngineServiceEvents)
	END_COM_MAP()

	BEGIN_DLGRESIZE_MAP(CLogFormView)
		DLGRESIZE_CONTROL(IDC_EDIT1, DLSZ_SIZE_X | DLSZ_SIZE_Y)
		DLGRESIZE_CONTROL(IDC_EDIT2, DLSZ_SIZE_X | DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_STATIC1, DLSZ_MOVE_Y)
    END_DLGRESIZE_MAP()

	BEGIN_MSG_MAP(CLogFormView)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		CHAIN_MSG_MAP(CDialogResize<CLogFormView>)
		CHAIN_MSG_MAP(CAxDialogImpl<CLogFormView>)
	END_MSG_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease();

private:
	BOOL m_bInitialized;
	CEdit m_edit;
	CEditEx m_editCmd;
	CFont m_editFont;
	DWORD m_dwAdvice;
	bool m_bIsInProgress;

	std::vector<CString> m_vCommandsHistory;
	UINT_PTR m_commandHistoryIndex;

	CComPtr<IViewDebugEngineService> m_pViewDebugEngineService;

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnEditKeyDown(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/);
	STDMETHOD(GetDebugSession)(IDebugSession** ppDebugSession);
public:

	STDMETHOD(GetHWND)(HWND *hWnd);
	STDMETHOD(CreateEx)(HWND hWndParent, HWND *hWnd);
	STDMETHOD(PreTranslateMessage)(MSG *pMsg, BOOL *pbResult);

	STDMETHOD(CreateEx2)(HWND hWndParent, RECT rect, HWND* hWnd);
	STDMETHOD(GetText)(BSTR* pbstr);
	STDMETHOD(OnActivate)();
	STDMETHOD(OnDeactivate)();
	STDMETHOD(OnClose)();

	STDMETHOD(Output)(ULONG Mask, LPCTSTR Text);

	STDMETHOD(OnBeforeCommandExecute)(REFGUID guidCommandId, VARIANT* vParam);
	STDMETHOD(OnAfterCommandExecute)(REFGUID guidCommandId, VARIANT* vParam, IVariantObject* pResult);

	STDMETHOD(WriteLine)(BSTR bstr);
};

OBJECT_ENTRY_AUTO(__uuidof(LogFormView), CLogFormView)
