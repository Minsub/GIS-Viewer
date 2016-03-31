#if !defined(AFX_INDEXMAPSHEET_H__AFCF0DD4_22D4_4E40_B23A_18D57755A9B8__INCLUDED_)
#define AFX_INDEXMAPSHEET_H__AFCF0DD4_22D4_4E40_B23A_18D57755A9B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IndexMapSheet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CIndexMapSheet

#include "SheetIndexMap.h"

class CIndexMapSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CIndexMapSheet)

// Construction
public:
	CIndexMapSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CIndexMapSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:
	CSheetIndexMap		m_SheetIndexMap;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIndexMapSheet)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CIndexMapSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CIndexMapSheet)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INDEXMAPSHEET_H__AFCF0DD4_22D4_4E40_B23A_18D57755A9B8__INCLUDED_)
