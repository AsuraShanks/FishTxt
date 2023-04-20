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

	CWnd::OnRButtonDown(nFlags, point);
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