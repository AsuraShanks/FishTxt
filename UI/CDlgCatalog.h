#pragma once
#include "afxdialogex.h"
#include <vector>
#include <string>

// CDlgCatalog 对话框

class CDlgCatalog : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgCatalog)

public:
	CDlgCatalog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgCatalog();

// 对话框数据
//#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_CATALOG };
//#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	void SetCatalog(std::vector<CString> vcCatalog);

public:
	CListCtrl m_listCatalog;

public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnNMDblclkListCatalog(NMHDR* pNMHDR, LRESULT* pResult);
};
