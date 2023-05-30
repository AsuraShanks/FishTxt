// CDlgCatalog.cpp: 实现文件
//

#include "pch.h"
#include "FishTxt.h"
#include "afxdialogex.h"
#include "CDlgCatalog.h"
#include "FishTxtDlg.h"


// CDlgCatalog 对话框

IMPLEMENT_DYNAMIC(CDlgCatalog, CDialogEx)

CDlgCatalog::CDlgCatalog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_CATALOG, pParent)
{

}

CDlgCatalog::~CDlgCatalog()
{
}

void CDlgCatalog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CATALOG, m_listCatalog);
}


BEGIN_MESSAGE_MAP(CDlgCatalog, CDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_CATALOG, &CDlgCatalog::OnNMDblclkListCatalog)
END_MESSAGE_MAP()


// CDlgCatalog 消息处理程序


BOOL CDlgCatalog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_listCatalog.InsertColumn(0, _T("目录"), 0, 250);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BOOL CDlgCatalog::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
	{
		return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CDlgCatalog::OnNMDblclkListCatalog(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	int nIndex = pNMItemActivate->iItem;
	CString strCatalog = m_listCatalog.GetItemText(nIndex, 0);
	::PostMessage(GetParent()->GetSafeHwnd(), WM_GOTO_CATALOG, (WPARAM)(new std::string(strCatalog)), 0);
	ShowWindow(SW_HIDE);
	*pResult = 0;
}

void CDlgCatalog::SetCatalog(std::vector<CString> vcCatalog)
{
	m_listCatalog.DeleteAllItems();

	int nCount = vcCatalog.size();
	for (int i = 0; i < nCount; ++i)
	{
		CString strCatalog = vcCatalog[i];
		m_listCatalog.InsertItem(i, strCatalog);
	}
}
