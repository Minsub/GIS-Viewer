#if !defined(AFX_SHEETLAYER_H__C749DDF2_B438_4C0E_AB9B_EB7B73BE1FEF__INCLUDED_)
#define AFX_SHEETLAYER_H__C749DDF2_B438_4C0E_AB9B_EB7B73BE1FEF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SheetLayer.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSheetLayer dialog

#include "LayerList.h"
#include "Shape/LayerManager.h"

class CSheetLayer : public CPropertyPage
{
	DECLARE_DYNCREATE(CSheetLayer)

// Construction
public:
	CSheetLayer();
	~CSheetLayer();
	void ListView();
	
	bool		m_isView;
	Scaler*		m_pParentScaler;
	Scaler		m_Scaler;

	CLayerList*		m_pLayerList;
	LayerManager*   m_pLayerListSHP;


// Dialog Data
	//{{AFX_DATA(CSheetLayer)
	enum { IDD = IDD_DigSheetLayer };
	CListCtrl	m_list;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSheetLayer)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSheetLayer)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnSheetDel();
	afx_msg void OnItemchangedSheetLayer(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHEETLAYER_H__C749DDF2_B438_4C0E_AB9B_EB7B73BE1FEF__INCLUDED_)
