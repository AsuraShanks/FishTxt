#include "pch.h"
#include "CMyEdit.h"

CMyEdit::CMyEdit()
{
	m_bFishMode = false;
}

CMyEdit::~CMyEdit()
{

}

BEGIN_MESSAGE_MAP(CMyEdit, CWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEWHEEL()
	ON_WM_NCHITTEST()
	ON_WM_NCLBUTTONDOWN()
END_MESSAGE_MAP()

BOOL CMyEdit::Create(DWORD dwExStyle, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CWnd::CreateEx(dwExStyle, "Scintilla", "", dwStyle, rect, pParentWnd, nID);
}

void CMyEdit::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CWnd* pParent = GetParent();
	if (pParent == nullptr)
		return;

	ClientToScreen(&point);

	pParent->SendMessage(WM_EDIT_LBUTTONDOWN, nFlags, (LPARAM)&point);
}

void CMyEdit::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CWnd* pParent = GetParent();
	if (pParent == nullptr)
		return;

	ClientToScreen(&point);

	pParent->SendMessage(WM_EDIT_RBUTTONDOWN, nFlags, (LPARAM)&point);

	//CWnd::OnRButtonDown(nFlags, point);
}

BOOL CMyEdit::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (::GetKeyState(VK_CONTROL) < 0)
	{
		if (zDelta < 0)
			SendMessage(SCI_ZOOMOUT);
		else
			SendMessage(SCI_ZOOMIN);
	}
	else
	{
		if (zDelta < 0)
			SendMessage(SCI_LINESCROLL, 0, 1);
		else
			SendMessage(SCI_LINESCROLL, 0, -1);
	}
	
	return TRUE;
}

LRESULT CMyEdit::OnNcHitTest(CPoint point)
{
	CRect rect;
	GetClientRect(&rect);
	ScreenToClient(&point);

	if (point.x <= rect.left + 3 && point.y <= rect.top + 3) {
		return HTTOPLEFT;
	}
	else if (point.x >= rect.right - 3 && point.y <= rect.top + 3) {
		return HTTOPRIGHT;
	}
	else if (point.x <= rect.left + 3 && point.y >= rect.bottom - 3) {
		return HTBOTTOMLEFT;
	}
	else if (point.x >= rect.right - 3 && point.y >= rect.bottom - 3) {
		return HTBOTTOMRIGHT;
	}
	else if (point.x <= rect.left + 3) {
		return HTLEFT;
	}
	else if (point.x >= rect.right - 3) {
		return HTRIGHT;
	}
	else if (point.y <= rect.top + 3) {
		return HTTOP;
	}
	else if (point.y >= rect.bottom - 3) {
		return HTBOTTOM;
	}

	return CWnd::OnNcHitTest(point);
}

void CMyEdit::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CWnd* pParent = GetParent();
	if (pParent == nullptr)
		return;

	if (nHitTest == HTTOP)
		pParent->SendMessage(WM_SYSCOMMAND, SC_SIZE | WMSZ_TOP, MAKELPARAM(point.x, point.y));
	else if (nHitTest == HTBOTTOM)
		pParent->SendMessage(WM_SYSCOMMAND, SC_SIZE | WMSZ_BOTTOM, MAKELPARAM(point.x, point.y));
	else if (nHitTest == HTLEFT)
		pParent->SendMessage(WM_SYSCOMMAND, SC_SIZE | WMSZ_LEFT, MAKELPARAM(point.x, point.y));
	else if (nHitTest == HTRIGHT)
		pParent->SendMessage(WM_SYSCOMMAND, SC_SIZE | WMSZ_RIGHT, MAKELPARAM(point.x, point.y));
	else if (nHitTest == HTTOPLEFT)
		pParent->SendMessage(WM_SYSCOMMAND, SC_SIZE | WMSZ_TOPLEFT, MAKELPARAM(point.x, point.y));
	else if (nHitTest == HTTOPRIGHT)
		pParent->SendMessage(WM_SYSCOMMAND, SC_SIZE | WMSZ_TOPRIGHT, MAKELPARAM(point.x, point.y));
	else if (nHitTest == HTBOTTOMLEFT)
		pParent->SendMessage(WM_SYSCOMMAND, SC_SIZE | WMSZ_BOTTOMLEFT, MAKELPARAM(point.x, point.y));
	else if (nHitTest == HTBOTTOMRIGHT)
		pParent->SendMessage(WM_SYSCOMMAND, SC_SIZE | WMSZ_BOTTOMRIGHT, MAKELPARAM(point.x, point.y));
	else
		CWnd::OnNcLButtonDown(nHitTest, point);
}
