#include "LayerList.h"

#if !defined(AFX_DIGLAYERLIST_H__CE0E14B2_5132_45A6_86F1_C2C5989EBC53__INCLUDED_)
#define AFX_DIGLAYERLIST_H__CE0E14B2_5132_45A6_86F1_C2C5989EBC53__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DigLayerList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DigLayerList dialog

class DigLayerList : public CDialog
{
// Construction
public:
	DigLayerList(CWnd* pParent = NULL);   // standard constructor
	void		ListView();
	CLayerList*		m_LayerList;

// Dialog Data
	//{{AFX_DATA(DigLayerList)
	enum { IDD = IDD_DIALOG1 };
	CListCtrl	m_list;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DigLayerList)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DigLayerList)
	afx_msg void OnBtnHide();
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnDel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIGLAYERLIST_H__CE0E14B2_5132_45A6_86F1_C2C5989EBC53__INCLUDED_)
