#pragma once
#include <afxwin.h>
#include ".\include\Scintilla\SciLexer.h"
#include ".\include\Scintilla\Scintilla.h"

#define WM_EDIT_RBUTTONDOWN (WM_USER+1)
#define WM_EDIT_LBUTTONDOWN (WM_USER+2)
#define WM_EDIT_NCHITTEST   (WM_USER+3)

class CMyEdit : public CWnd
{
public:
    CMyEdit();
    virtual ~CMyEdit();
    virtual BOOL Create(DWORD dwExStyle, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

    DECLARE_MESSAGE_MAP()

public:
    bool m_bFishMode;

public:
    void SetFishMode(bool bMode) { m_bFishMode = bMode; }
  
public:
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg LRESULT OnNcHitTest(CPoint point);
    afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
};

