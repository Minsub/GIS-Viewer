// GDFDoc.cpp : implementation of the CGDFDoc class
//

#include "stdafx.h"
#include "GDF.h"

#include "GDFDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGDFDoc

IMPLEMENT_DYNCREATE(CGDFDoc, CDocument)

BEGIN_MESSAGE_MAP(CGDFDoc, CDocument)
	//{{AFX_MSG_MAP(CGDFDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGDFDoc construction/destruction

CGDFDoc::CGDFDoc()
{
	// TODO: add one-time construction code here

}

CGDFDoc::~CGDFDoc()
{
}

BOOL CGDFDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CGDFDoc serialization

void CGDFDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CGDFDoc diagnostics

#ifdef _DEBUG
void CGDFDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGDFDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGDFDoc commands
