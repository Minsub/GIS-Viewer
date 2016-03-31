#if !defined(AFX_SHEETINDEXMAP_H__AE3D7CCF_327F_46B4_B4BE_9139D2E7F33E__INCLUDED_)
#define AFX_SHEETINDEXMAP_H__AE3D7CCF_327F_46B4_B4BE_9139D2E7F33E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SheetIndexMap.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSheetIndexMap dialog

#include "LayerList.h"
#include "scaler.h"
#include "Shape/LayerManager.h"

#define INDEXMAP_POS_MIN 1
#define INDEXMAP_POS_MAX 231

class CSheetIndexMap : public CPropertyPage
{
	DECLARE_DYNCREATE(CSheetIndexMap)

// Construction
public:
	CSheetIndexMap();
	~CSheetIndexMap();

	//임시
	bool			m_Render;
	int				LayerCnt;

	CLayerList*		m_pLayerList;
	LayerManager*   m_pLayerListSHP;

	Scaler*			m_pParentScaler;
	Scaler			m_Scaler;
	
	
	//더블 버퍼링
	CDC				*m_mDC;
	CBitmap			*m_Bitmap;
	
	void DrawRectangle(CDC &pDC);



// Dialog Data
	//{{AFX_DATA(CSheetIndexMap)
	enum { IDD = IDD_DigSheetIndexMap };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSheetIndexMap)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSheetIndexMap)
	afx_msg void OnPaint();
	afx_msg void OnButton1();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHEETINDEXMAP_H__AE3D7CCF_327F_46B4_B4BE_9139D2E7F33E__INCLUDED_)
