﻿
// FishTxt.cpp: 定义应用程序的类行为。
//
#include "pch.h"
#include "framework.h"
#include "FishTxt.h"
#include ".\UI\FishTxtDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFishTxtApp

BEGIN_MESSAGE_MAP(CFishTxtApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CFishTxtApp 构造

CFishTxtApp::CFishTxtApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
	m_hUchardet = NULL;
}


// 唯一的 CFishTxtApp 对象

CFishTxtApp theApp;


// CFishTxtApp 初始化

BOOL CFishTxtApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。  否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 激活“Windows Native”视觉管理器，以便在 MFC 控件中启用主题
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	//Scintilla_RegisterClasses(theApp.m_hInstance);
	m_hScintilla = LoadDll(_T("Scintilla.dll"));
	m_hUchardet = LoadDll(_T("uchardet.dll"));
	if (m_hScintilla == NULL || m_hUchardet == NULL)
	{
		return FALSE;
	}

	CFishTxtDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, _T("警告: 对话框创建失败，应用程序将意外终止。\n"));
		TRACE(traceAppMsg, 0, _T("警告: 如果您在对话框上使用 MFC 控件，则无法 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS。\n"));
	}

	// 删除上面创建的 shell 管理器。
	if (pShellManager != nullptr)
	{
		delete pShellManager;
	}

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

int CFishTxtApp::ExitInstance()
{
	// TODO: 在此添加专用代码和/或调用基类
	if (m_hScintilla != NULL)
	{
		FreeLibrary(m_hScintilla);
		m_hScintilla = NULL;
	}

	if (m_hUchardet != NULL)
	{
		FreeLibrary(m_hUchardet);
		m_hUchardet = NULL;
	}

	return CWinApp::ExitInstance();
}

std::string CFishTxtApp::_GetCurFilePath()
{
	TCHAR path[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, path, MAX_PATH);
	std::string sPath(path);
	int pos = sPath.rfind(_T("\\"));
	std::string sRet = sPath.substr(0, pos);
	sRet += _T("\\");
	return sRet;
}

HMODULE CFishTxtApp::LoadDll(std::string strDllName)
{
	std::string strDll = _GetCurFilePath();
	strDll += strDllName;
	HMODULE hModule = LoadLibraryEx(strDll.c_str(), NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
	if (hModule == NULL)
	{
		CString msg;
		msg.Format(_T("load %s fail! error code:%d"), strDllName.c_str(), GetLastError());
		AfxMessageBox(msg);
		return NULL;
	}
	return hModule;
}
