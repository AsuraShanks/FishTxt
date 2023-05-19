// FishTxtDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "FishTxt.h"
#include "FishTxtDlg.h"
#include "afxdialogex.h"
#include <fstream>
#include <sstream>
#include <thread>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define WM_QUIT_FISH			(WM_USER+1)
#define WM_CLOSE_ALL			(WM_USER+2)
#define WM_QUIT_FULL_SCREEN		(WM_USER+3)
#define WM_INTO_FISH			(WM_USER+4)

#define TIMER_MOUSE_MOVE		(WM_USER+50)

static const int iButtonBarWidth = 180;
static const int iDlgWidth = 650;
static const int iDlgHeight = 500;


////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////钩子相关/////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
HWND gDialog = NULL;
HWND gEdit = NULL;
HHOOK hMouseHook = NULL; //鼠标钩子
HHOOK hKeyBoardHook = NULL; //键盘钩子
HHOOK hCBTHook = NULL; //窗口动作钩子

LRESULT CALLBACK MouseProc(_In_ int    nCode, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	LPMOUSEHOOKSTRUCT pMouseHook = (MOUSEHOOKSTRUCT FAR*) lParam;
	if (nCode >= 0 && gDialog && IsWindow(gDialog) && gEdit && IsWindow(gEdit)) 
	{
		HWND glhTargetWnd = pMouseHook->hwnd; // 取目标窗口句柄
		if (glhTargetWnd == gEdit && wParam == WM_MOUSEWHEEL)
		{
			//PostMessage(gDialog, wParam, pMouseHook->wHitTestCode, (LPARAM) & (pMouseHook->pt));
			//SendMessage(gEdit, SCI_PAGEUPEXTEND, NULL, NULL);
			//return 1;
		}
	}

	// 继续传递消息
	return CallNextHookEx((HHOOK)hMouseHook, nCode, wParam, lParam);
}
LRESULT CALLBACK KeyboardProc(_In_ int    nCode, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	/*if (nCode >= 0)
	{
		if (wParam != VK_LEFT && wParam != VK_RIGHT && wParam != VK_UP && wParam != VK_DOWN)
		{
			return 1;
		}
	}*/
	// 继续传递消息
	return CallNextHookEx((HHOOK)hKeyBoardHook, nCode, wParam, lParam);
}
LRESULT CALLBACK CBTProc(_In_ int    nCode, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	// 继续传递消息
	return CallNextHookEx((HHOOK)hCBTHook, nCode, wParam, lParam);
}

////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////全局函数///////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
BOOL ShowInTaskbar(HWND hWnd, BOOL bShow)
{

	HRESULT hr;
	ITaskbarList* pTaskbarList;
	hr = CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER,
		IID_ITaskbarList, (void**)&pTaskbarList);

	if (SUCCEEDED(hr))
	{

		pTaskbarList->HrInit();
		if (bShow)
			pTaskbarList->AddTab(hWnd);
		else
			pTaskbarList->DeleteTab(hWnd);

		pTaskbarList->Release();
		return TRUE;
	}

	return FALSE;
}


// CFishTxtDlg 对话框

CFishTxtDlg::CFishTxtDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(CFishTxtDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hAccelGoToLine = LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_ACCELERATOR_GO_TO_LINE));
	m_bMiniMode = false;
	m_bFullScreen = false;
	m_miniWidth = 650;
	m_miniHeight = 40;
	pGotoDlg = nullptr;
}

void CFishTxtDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_OPEN_FILE, m_btnOpenFile);
	DDX_Control(pDX, IDC_BTN_FULL_SCREEN, m_btnFullScreen);
	DDX_Control(pDX, IDC_BTN_FISHING, m_btnFishing);
}

BEGIN_MESSAGE_MAP(CFishTxtDlg, CDialogEx)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_OPEN_FILE, &CFishTxtDlg::OnBnClickedBtnOpenFile)
	ON_BN_CLICKED(IDC_BTN_FULL_SCREEN, &CFishTxtDlg::OnBnClickedBtnFullScreen)
	ON_BN_CLICKED(IDC_BTN_FISHING, &CFishTxtDlg::OnBnClickedBtnFishing)
	ON_MESSAGE(WM_EDIT_RBUTTONDOWN, &CFishTxtDlg::OnEditRButtonDown)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CFishTxtDlg 消息处理程序

BOOL CFishTxtDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_ScintillaEdit.SendMessage(SCI_SETMARGINWIDTHN, 1, 0);			//隐藏左侧margin
	m_ScintillaEdit.SendMessage(SCI_SETHSCROLLBAR, 0, 0);			//隐藏水平滚动条
	m_ScintillaEdit.SendMessage(SCI_SETVSCROLLBAR, 0, 0);			//隐藏垂直滚动条
	m_ScintillaEdit.SendMessage(SCI_USEPOPUP, 0);					//隐藏自带弹出菜单
	m_ScintillaEdit.SendMessage(SCI_SETCODEPAGE, SC_CP_UTF8);		//设置编辑框识别编码为utf8
	m_ScintillaEdit.SendMessage(SCI_SETWRAPMODE, 2);				//启用换行

	gDialog = this->GetSafeHwnd();
	gEdit = m_ScintillaEdit.GetSafeHwnd();
//参数1：int idHook 钩子类型，即它处理的消息类型
//       WH_CBT=5(窗口动作消息),在以下事件之前，系统都会调用WH_CBT Hook子程，这些事件包括：
//                1）激活，建立，销毁，最小化，最大化，移动，改变尺寸等窗口事件；
//                2）完成系统指令；
//                3）来自系统消息队列中的移动鼠标，键盘事件；
//                4）设置输入焦点事件；
//                5）同步系统消息队列事件。
//       WH_MOUSE=7   鼠标消息
//       WH_KEYBOARD=2  键盘消息
//参数2：HOOKPROC lpfn  回调函数地址(处理函数)
//       注意：不同钩子类型，处理函数是不同的
//            如果是WH_MOUSE类型   处理函数是MouseProc
//            如果是WH_KEYBOARD类型   处理函数是KeyboardProc
//            如果是WH_CBT类型   处理函数是CBTProc
//参数3：
//       NULL  钩子只关注本进程
//       如果是远程钩子,参数2就得是一个单独的动态库dll
//             参数3就是动态库的句柄，参数4为NULL
//        注意：远程钩子，杀毒软件都会认为是病毒
//参数4：DWORD dwThreadId  线程ID
//返回值：如果函数成功，则返回值是钩子句柄。如果函数失败，则返回值为 NULL
	hMouseHook = SetWindowsHookEx(WH_MOUSE, MouseProc, NULL, GetCurrentThreadId());
	hKeyBoardHook = SetWindowsHookEx(WH_KEYBOARD, KeyboardProc, NULL, GetCurrentThreadId());
	hCBTHook = SetWindowsHookEx(WH_CBT, CBTProc, NULL, GetCurrentThreadId());

	SetButtonPosition(true);

	SetTimer(TIMER_MOUSE_MOVE, 10, NULL);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

BOOL CFishTxtDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
			return TRUE;
	}
	if (pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST)
	{
		if (m_hAccelGoToLine && ::TranslateAccelerator(m_hWnd, m_hAccelGoToLine, pMsg))
		{
			GotoLineDlg();
			return TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

LRESULT CFishTxtDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	switch (message)
	{
	case WM_LBUTTONDOWN:
	{
		if (m_bMiniMode)
		{
			CPoint point = *(CPoint*)lParam;
			PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
		}
	}
		break;
	default:
		break;
	}

	return CDialogEx::WindowProc(message, wParam, lParam);
}

int CFishTxtDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码WS_EX_CLIENTEDGE
	if (!m_ScintillaEdit.Create(NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, lpCreateStruct->cx, lpCreateStruct->cy), this, 10000))
	{
		OutputDebugString("创建SciLexer编辑框失败");
	}

	return 0;
}

void CFishTxtDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

void CFishTxtDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (m_ScintillaEdit.GetSafeHwnd())
	{
		if (m_bFullScreen)
		{
			m_ScintillaEdit.MoveWindow(0, 0, cx, cy);
		}
		else
		{
			m_ScintillaEdit.MoveWindow(iButtonBarWidth, 0, cx - iButtonBarWidth, cy);
		}
	}	
}

void CFishTxtDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	KillTimer(TIMER_MOUSE_MOVE);

	if (hMouseHook != NULL)  //如果钩子句柄不等于NULL
	{
		//卸载钩子
		//参数：要移除的挂钩的句柄
		//返回值:如果该函数成功，则返回值为非零值。如果函数失败，则返回值为零
		UnhookWindowsHookEx(hMouseHook);
	}
	if (hKeyBoardHook != NULL)
	{
		UnhookWindowsHookEx(hKeyBoardHook);
	}
	if (hCBTHook != NULL)
	{
		UnhookWindowsHookEx(hCBTHook);
	}

	CDialogEx::OnClose();
}

void CFishTxtDlg::OnBnClickedBtnOpenFile()
{
	CFileDialog fileDlg(TRUE, _T("txt"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("文本文档(*.txt)|*.txt|测试用(*.cpp)|*.cpp||") , this);

	if (fileDlg.DoModal() == IDOK)
	{
		CString strPath = fileDlg.GetPathName();
		
		std::ifstream file;
		file.open(strPath.GetBuffer(), std::ios::in | std::ios::binary);
		strPath.ReleaseBuffer();
		if (!file.is_open())
		{
			CString msg;
			msg.Format("%s打开失败！", strPath);
			AfxMessageBox(msg);
			return;
		}
		std::stringstream buf;
		buf << file.rdbuf();
		std::string strText(buf.str());
		file.close();
		m_ScintillaEdit.SendMessage(SCI_SETTEXT, 1, (LPARAM)strText.c_str());
	}
}

void CFishTxtDlg::OnBnClickedBtnFullScreen()
{
	SetButtonPosition(false);
	
	CRect rc;
	GetClientRect(&rc);
	m_ScintillaEdit.MoveWindow(rc,FALSE);
	
	m_bFullScreen = true;
}

void CFishTxtDlg::OnBnClickedBtnFishing()
{
	SetFishWindowStyle();

	if(!m_bFullScreen)
		OnBnClickedBtnFullScreen();

	::SetWindowPos(this->GetSafeHwnd(), HWND_TOPMOST, 0, 0, m_miniWidth, m_miniHeight, SWP_NOMOVE | SWP_NOREPOSITION);

	SetFishMode(true);
}

LRESULT CFishTxtDlg::OnEditRButtonDown(WPARAM wParam, LPARAM lParam)
{
	CPoint point = *(CPoint*)lParam;

	CMenu MainMenu, menu;
	MainMenu.CreateMenu();
	menu.CreatePopupMenu();
	if (!m_bMiniMode && !m_bFullScreen)
	{
		return 0;
	}
	else if (m_bMiniMode)
	{
		menu.AppendMenu(MF_STRING, WM_QUIT_FISH, "退出魔域模式");
		menu.AppendMenu(MF_STRING, WM_CLOSE_ALL, "关闭软件");
	}
	else
	{
		menu.AppendMenu(MF_STRING, WM_QUIT_FULL_SCREEN, "退出全屏模式");
		menu.AppendMenu(MF_STRING, WM_INTO_FISH, "进入魔域模式");
	}
	
	UINT nCmd = menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RETURNCMD, point.x, point.y, this);
	if (nCmd == WM_QUIT_FISH)
	{
		SetNormalWindowStyle();

		m_bFullScreen = false;
		::SetWindowPos(this->GetSafeHwnd(), HWND_NOTOPMOST, 0, 0, iDlgWidth, iDlgHeight, SWP_NOMOVE);
		
		SetButtonPosition(true);
		SetFishMode(false);
	}
	else if (nCmd == WM_CLOSE_ALL)
	{
		PostQuitMessage(0);
	}
	else if (nCmd == WM_QUIT_FULL_SCREEN)
	{
		SetButtonPosition(true);

		CRect rc;
		GetClientRect(&rc);
		rc.left += iButtonBarWidth;
		m_ScintillaEdit.MoveWindow(rc);
		m_bFullScreen = false;
	}
	else if (nCmd == WM_INTO_FISH)
	{
		OnBnClickedBtnFishing();
	}

	return 0;
}

void CFishTxtDlg::SetButtonPosition(bool bVisble)
{
	const int iBtnWidth = 100;
	const int iBtnHeight = 33;
	const int iBtnLeft = 40;
	const int iTopMargin = 50;
	const int iMargin = 30;

	CRect rc;
	GetClientRect(&rc);
	int left = rc.left + iBtnLeft;
	int top = rc.top + iTopMargin;

	if (bVisble)
	{
		m_btnOpenFile.SetWindowPos(this, left, top, iBtnWidth, iBtnHeight, SWP_NOZORDER);
		top += iBtnHeight + iMargin;
		m_btnFullScreen.SetWindowPos(this, left, top, iBtnWidth, iBtnHeight, SWP_NOZORDER);
		top += iBtnHeight + iMargin;
		m_btnFishing.SetWindowPos(this, left, top, iBtnWidth, iBtnHeight, SWP_NOZORDER);
	}
	else
	{
		m_btnOpenFile.SetWindowPos(this, 0, 0, 0, 0, SWP_NOZORDER);
		m_btnFullScreen.SetWindowPos(this, 0, 0, 0, 0, SWP_NOZORDER);
		m_btnFishing.SetWindowPos(this, 0, 0, 0, 0, SWP_NOZORDER);
	}
}

void CFishTxtDlg::SetButtonVisble(bool bVisble)
{
	if (bVisble)
	{
		m_btnOpenFile.ShowWindow(SW_SHOW);
		m_btnFullScreen.ShowWindow(SW_SHOW);
		m_btnFishing.ShowWindow(SW_SHOW);
	}
	else
	{
		m_btnOpenFile.ShowWindow(SW_HIDE);
		m_btnFullScreen.ShowWindow(SW_HIDE);
		m_btnFishing.ShowWindow(SW_HIDE);
	}
}

void CFishTxtDlg::SetFishMode(bool bMode)
{
	m_bMiniMode = bMode;
	m_ScintillaEdit.SetFishMode(bMode);
}

void CFishTxtDlg::SetFishWindowStyle()
{
	::SetWindowLong(m_hWnd, GWL_STYLE, GetWindowLong(m_hWnd, GWL_STYLE) & ~WS_CAPTION & ~WS_THICKFRAME & ~WS_BORDER);
	ShowInTaskbar(this->GetSafeHwnd(), FALSE);
}

void CFishTxtDlg::SetNormalWindowStyle()
{
	::SetWindowLong(m_hWnd, GWL_STYLE, GetWindowLong(m_hWnd, GWL_STYLE) | WS_CAPTION | WS_THICKFRAME | WS_BORDER);
	ShowInTaskbar(this->GetSafeHwnd(), TRUE);
}

void CFishTxtDlg::GotoLine(int iTarDocLine, int iCurVisLine)
{
	int iTarVisLine = m_ScintillaEdit.SendMessage(SCI_VISIBLEFROMDOCLINE, iTarDocLine);
	m_ScintillaEdit.SendMessage(SCI_LINESCROLL, 0, iTarVisLine - iCurVisLine);
}

void CFishTxtDlg::GotoLineDlg()
{
	if (pGotoDlg && pGotoDlg->IsWindowVisible())
		return;
	int iLineCount = m_ScintillaEdit.SendMessage(SCI_GETLINECOUNT);
	int iCurVisLine = m_ScintillaEdit.SendMessage(SCI_GETFIRSTVISIBLELINE) + 1;
	int iCurDocLine = m_ScintillaEdit.SendMessage(SCI_DOCLINEFROMVISIBLE, iCurVisLine);
	if (pGotoDlg == nullptr)
	{
		pGotoDlg = new CGoToLineDlg(this);
		pGotoDlg->Create(CGoToLineDlg::IDD, this);
	}
	pGotoDlg->SetInfo(iCurDocLine, iLineCount, iCurVisLine);
	pGotoDlg->Show(TRUE);	
}

void CFishTxtDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case TIMER_MOUSE_MOVE:
	{
		if (m_bMiniMode)
		{
			CRect rc;
			CPoint pt;
			GetWindowRect(&rc);
			GetCursorPos(&pt);
			if (!rc.PtInRect(pt))
			{
				ShowWindow(SW_HIDE);
			}
			else
			{
				ShowWindow(SW_SHOW);
			}
		}
	}
		break;
	default:
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}
