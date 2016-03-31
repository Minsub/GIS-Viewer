// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__83A3025D_B45E_4A7F_9270_2845BD8A1BB7__INCLUDED_)
#define AFX_MAINFRM_H__83A3025D_B45E_4A7F_9270_2845BD8A1BB7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Sheet.h"
#include "IndexMapSheet.h"

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
	void DisplayMessage(double x, double y);
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar			m_wndStatusBar;
	CToolBar			m_wndToolBar;
	CDialogBar			m_wndPannel;
	Sheet				*m_pSheet;
	CIndexMapSheet		*m_pIndexMapSheet;
public:
	
	Sheet*				GetSheet()			{	return m_pSheet;	};
	CIndexMapSheet*		GetIndexMapSheet()	{	return m_pIndexMapSheet;	};
// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnFileOpen();
	afx_msg void OnMove();
	afx_msg void OnArea();
	afx_msg void OnBtnLayer();
	afx_msg void OnSHPOpen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__83A3025D_B45E_4A7F_9270_2845BD8A1BB7__INCLUDED_)
