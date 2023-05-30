
// FishTxtDlg.h: 头文件
//

#pragma once
#include <string>
#include "..\src\CMyEdit.h"
#include "CDlgGoto.h"
#include "CDlgCatalog.h"

#define WM_QUIT_FISH			(WM_USER+1)
#define WM_CLOSE_ALL			(WM_USER+2)
#define WM_QUIT_FULL_SCREEN		(WM_USER+3)
#define WM_INTO_FISH			(WM_USER+4)
#define WM_CATALOG				(WM_USER+5)

#define WM_GOTO_CATALOG			(WM_USER+50)
#define TIMER_MOUSE_MOVE		(WM_USER+100)


enum ENUM_CODING_FORM
{
	ECF_OPEN_FAIL = -1,	//打开文件失败
	ECF_ANSI = 0,		//ANSI 格式
	ECF_UTF8,			//UTF-8格式
	ECF_UTF8_BOM,		//UTF-8带BOM格式
	ECF_UNICODE_LITTLE,	//unicode Little Endian格式
	ECF_UNICODE_BIG,	//unicode Big Endian格式
};

// CFishTxtDlg 对话框
class CFishTxtDlg : public CDialogEx
{
// 构造
public:
	CFishTxtDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
//#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FISHTXT_DIALOG };
//#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	HACCEL m_hAccelGoToLine;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()

public:
	CButton m_btnOpenFile;
	CButton m_btnFullScreen;
	CButton m_btnFishing;
	CMyEdit m_ScintillaEdit;
	CString m_strText;

	bool m_bMiniMode;
	bool m_bFullScreen;
	

	//MINI模式宽高
	int m_miniWidth;
	int m_miniHeight;

	CGoToLineDlg* m_pDlgGoto;
	CDlgCatalog* m_pDlgCatalog;

public:
	void SetButtonPosition(bool bVisble);
	void SetButtonVisble(bool bVisble);
	void SetFishMode(bool bMode);
	void SetFishWindowStyle();
	void SetNormalWindowStyle();
	void GotoLine(int iTarDocLine, int iCurVisLine);
	void GotoLineDlg();
	void getCatalog(std::string strText);
	int textEncoding(CString strFile);		//-2:error

public:
	afx_msg void OnBnClickedBtnOpenFile();
	afx_msg void OnBnClickedBtnFullScreen();
	afx_msg void OnBnClickedBtnFishing();
	afx_msg LRESULT OnEditRButtonDown(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGotoCatalog(WPARAM wParam, LPARAM lParam);
};
