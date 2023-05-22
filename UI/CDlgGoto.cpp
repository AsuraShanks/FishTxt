// GoToLineDlg.cpp: 实现文件
//

#include "pch.h"
#include "FishTxt.h"
#include "afxdialogex.h"
#include "FishTxtDlg.h"
#include "CDlgGoto.h"


// CGoToLineDlg 对话框

IMPLEMENT_DYNAMIC(CGoToLineDlg, CDialogEx)

CGoToLineDlg::CGoToLineDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(CGoToLineDlg::IDD, pParent)
	, m_iCurLine(1)
	, m_iLineCount(0)
	, m_iTargetLine(0)
	, m_iCurVisLine(0)
{

}

CGoToLineDlg::~CGoToLineDlg()
{
}

void CGoToLineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_CUR_LINE, m_iCurLine);
	DDV_MinMaxInt(pDX, m_iCurLine, 1, INT_MAX);
	DDX_Text(pDX, IDC_STATIC_LINE_COUNT, m_iLineCount);
	DDV_MinMaxInt(pDX, m_iLineCount, 0, INT_MAX);
	DDX_Control(pDX, IDC_EDIT_TARGET, m_editTarget);
}


BEGIN_MESSAGE_MAP(CGoToLineDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CGoToLineDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CGoToLineDlg 消息处理程序


BOOL CGoToLineDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_editTarget.SetWindowText("");
	GotoCenter();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BOOL CGoToLineDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CGoToLineDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strLine;
	m_editTarget.GetWindowText(strLine);
	m_iTargetLine = atoi(strLine);
	m_editTarget.SetWindowText("");

	CFishTxtDlg* pParent = (CFishTxtDlg*)GetParent();

	pParent->GotoLine(m_iTargetLine, m_iCurVisLine);
	Show(FALSE);
}

void CGoToLineDlg::GotoCenter()
{
	CRect rcParent, rc;
	::GetWindowRect(this->GetParent()->GetSafeHwnd(), &rcParent);
	GetWindowRect(&rc);
	CPoint ptCenter;
	ptCenter.x = rcParent.left + (rcParent.right - rcParent.left) / 2;
	ptCenter.y = rcParent.top + (rcParent.bottom - rcParent.top) / 2;

	::SetWindowPos(this->GetSafeHwnd(), HWND_TOP, ptCenter.x - rc.Width() / 2, ptCenter.y - rc.Height() / 2, rc.Width(), rc.Height(), SWP_HIDEWINDOW);
}

void CGoToLineDlg::SetInfo(int iCurLine, int iLineCount, int iCurVisLine)
{
	m_iCurLine = iCurLine;
	m_iLineCount = iLineCount;
	m_iCurVisLine = iCurVisLine;
	UpdateData(FALSE);
}

void CGoToLineDlg::Show(bool bShow)
{
	ShowWindow(bShow);
	if (bShow)
		GetDlgItem(IDC_EDIT_TARGET)->SetFocus();
}