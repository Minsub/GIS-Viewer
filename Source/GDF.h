// GDF.h : main header file for the GDF application
//

#if !defined(AFX_GDF_H__B463878F_8CCD_4916_8E99_D32067AFDBDD__INCLUDED_)
#define AFX_GDF_H__B463878F_8CCD_4916_8E99_D32067AFDBDD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CGDFApp:
// See GDF.cpp for the implementation of this class
//

class CGDFApp : public CWinApp
{
public:
	CGDFApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGDFApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CGDFApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GDF_H__B463878F_8CCD_4916_8E99_D32067AFDBDD__INCLUDED_)
