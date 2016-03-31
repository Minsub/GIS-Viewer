// IndexMapSheet.cpp : implementation file
//

#include "stdafx.h"
#include "GDF.h"
#include "IndexMapSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIndexMapSheet

IMPLEMENT_DYNAMIC(CIndexMapSheet, CPropertySheet)

CIndexMapSheet::CIndexMapSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_SheetIndexMap);
}

CIndexMapSheet::CIndexMapSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_SheetIndexMap);
}

CIndexMapSheet::~CIndexMapSheet()
{
}


BEGIN_MESSAGE_MAP(CIndexMapSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CIndexMapSheet)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIndexMapSheet message handlers
