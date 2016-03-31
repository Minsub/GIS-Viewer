// Sheet.cpp : implementation file
//

#include "stdafx.h"
#include "GDF.h"
#include "Sheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Sheet

IMPLEMENT_DYNAMIC(Sheet, CPropertySheet)

Sheet::Sheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_SheetLayer);

}

Sheet::Sheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_SheetLayer);

}

Sheet::~Sheet()
{
}


BEGIN_MESSAGE_MAP(Sheet, CPropertySheet)
	//{{AFX_MSG_MAP(Sheet)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Sheet message handlers
