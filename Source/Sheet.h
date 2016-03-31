#if !defined(AFX_SHEET_H__8573D6CD_8939_44A7_9FE3_3F63B3002833__INCLUDED_)
#define AFX_SHEET_H__8573D6CD_8939_44A7_9FE3_3F63B3002833__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Sheet.h : header file
//
#include "SheetLayer.h"


/////////////////////////////////////////////////////////////////////////////
// Sheet

class Sheet : public CPropertySheet
{
	DECLARE_DYNAMIC(Sheet)

// Construction
public:
	Sheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	Sheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:
	CSheetLayer	m_SheetLayer;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Sheet)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~Sheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(Sheet)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHEET_H__8573D6CD_8939_44A7_9FE3_3F63B3002833__INCLUDED_)
