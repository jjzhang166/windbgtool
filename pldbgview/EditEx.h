#pragma once

class CEditEx : public CWindowImpl<CEditEx ,CEdit>
{
public:
	struct NMKEYDOWN
	{
		NMHDR hdr;
		WPARAM wParam;
		LPARAM lParam;
	};

	BEGIN_MSG_MAP(CEditEx)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	END_MSG_MAP()

	CEditEx(){ }
	CEditEx& operator=(HWND hWnd);

	LRESULT OnKeyDown( UINT uMsg,
		WPARAM wParam,
		LPARAM lParam,
		BOOL& /*bHandled*/)
	{
		NMKEYDOWN nm = {{m_hWnd, (UINT_PTR)GetDlgCtrlID(), uMsg}, wParam, lParam};
		::SendMessage(GetParent(), WM_NOTIFY, (WPARAM)GetDlgCtrlID(), (LPARAM)&nm);
		return DefWindowProc(uMsg, wParam, lParam);
	}
};
