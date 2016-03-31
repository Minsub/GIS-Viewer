// GDFDoc.h : interface of the CGDFDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GDFDOC_H__C0D4AFF0_4DA8_48DC_BA72_6DB75C146F3C__INCLUDED_)
#define AFX_GDFDOC_H__C0D4AFF0_4DA8_48DC_BA72_6DB75C146F3C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CGDFDoc : public CDocument
{
protected: // create from serialization only
	CGDFDoc();
	DECLARE_DYNCREATE(CGDFDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGDFDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGDFDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CGDFDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GDFDOC_H__C0D4AFF0_4DA8_48DC_BA72_6DB75C146F3C__INCLUDED_)
