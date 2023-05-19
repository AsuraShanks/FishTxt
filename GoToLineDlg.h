#pragma once
#include "afxdialogex.h"


// CGoToLineDlg 对话框

class CGoToLineDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGoToLineDlg)

public:
	CGoToLineDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CGoToLineDlg();

// 对话框数据
//#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_GO_TO_LINE };
//#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	int m_iCurLine;
	int m_iLineCount;
	int m_iTargetLine;
	int m_iCurVisLine;
	CEdit m_editTarget;

private:
	void GotoCenter();

public:
	void SetInfo(int iCurLine, int iLineCount, int iCurVisLine); //iCurVisLine用于跳转时的计算跳转行数
	void Show(bool bShow);

public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedOk();
};
