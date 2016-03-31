// GDFView.h : interface of the CGDFView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GDFVIEW_H__63D22ECA_036A_4F40_8610_A4E88E692B3C__INCLUDED_)
#define AFX_GDFVIEW_H__63D22ECA_036A_4F40_8610_A4E88E692B3C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Scaler.h"
#include "Layer.h"
#include "LayerList.h"
#include "MainFrm.h"
#include "DigLayerList.h"
#include "Shape/LayerManager.h"

#define SHPFILE 1
#define GDFFILE 2

class CGDFView : public CView
{
protected: // create from serialization only
	CGDFView();
	DECLARE_DYNCREATE(CGDFView)

// Attributes
public:
	CGDFDoc* GetDocument();
	//	GDF
	int				m_TypeInfo;
	CLayerList		g_CLayerList;
	//  SHP

	LayerManager	g_CLayerListSHP;
	//INTERFACE
	DigLayerList	m_DigLayerList;
	Sheet*			m_pSheet;
	CIndexMapSheet* m_pIndexMapSheet;	

	//공통
	Scaler			m_scaler;
	int				m_isFileType;
	bool			m_isOpenFile;
	CPoint	 m_ptZoomWindowLT ;  // 임시 시작점
	CPoint	 m_ptZoomWindowBR;   //임시 끝점.(좌상단 점과 우하단 점..)

	CDC		 m_indexMapCDC;
	CBitmap  m_indexMapBitmap;
	
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGDFView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGDFView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CGDFView)
	afx_msg void OnFileOpen();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnZoomout();
	afx_msg void OnZoonin();
	afx_msg void OnMove();
	afx_msg void OnArea();
	afx_msg void OnUpdateZoonin(CCmdUI* pCmdUI);
	afx_msg void OnUpdateZoomout(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMove(CCmdUI* pCmdUI);
	afx_msg void OnUpdateArea(CCmdUI* pCmdUI);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnExtent();
	afx_msg void OnUpdateExtent(CCmdUI* pCmdUI);
	afx_msg void OnBtnLayer();
	afx_msg void OnSHPOpen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in GDFView.cpp
inline CGDFDoc* CGDFView::GetDocument()
   { return (CGDFDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GDFVIEW_H__63D22ECA_036A_4F40_8610_A4E88E692B3C__INCLUDED_)
