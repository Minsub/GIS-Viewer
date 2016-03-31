// GDFView.cpp : implementation of the CGDFView class
//

#include "stdafx.h"
#include "GDF.h"

#include "GDFDoc.h"
#include "GDFView.h"
#include <math.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGDFView

IMPLEMENT_DYNCREATE(CGDFView, CView)

BEGIN_MESSAGE_MAP(CGDFView, CView)
	//{{AFX_MSG_MAP(CGDFView)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_SIZE()
	ON_COMMAND(ID_ZOOMOUT, OnZoomout)
	ON_COMMAND(ID_ZOONIN, OnZoonin)
	ON_COMMAND(ID_MOVE, OnMove)
	ON_COMMAND(ID_AREA, OnArea)
	ON_UPDATE_COMMAND_UI(ID_ZOONIN, OnUpdateZoonin)
	ON_UPDATE_COMMAND_UI(ID_ZOOMOUT, OnUpdateZoomout)
	ON_UPDATE_COMMAND_UI(ID_MOVE, OnUpdateMove)
	ON_UPDATE_COMMAND_UI(ID_AREA, OnUpdateArea)
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_EXTENT, OnExtent)
	ON_UPDATE_COMMAND_UI(ID_EXTENT, OnUpdateExtent)
	ON_COMMAND(ID_BtnLayer, OnBtnLayer)
	ON_COMMAND(ID_SHPOpen, OnSHPOpen)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGDFView construction/destruction

CGDFView::CGDFView()
{
	m_isFileType = 0;
	m_isOpenFile = false;
	m_TypeInfo = 0;
	// TODO: add construction code here

}

CGDFView::~CGDFView()
{
}

BOOL CGDFView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CGDFView drawing

void CGDFView::OnDraw(CDC* pDC)
{
	CGDFDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here

	if(m_isOpenFile==false)		//파일이 하나도 안열렸으면 리턴
		return;

	CRect rect;
	GetClientRect(&rect);
	// 화면의 크기
	m_scaler.sxMin = rect.left;
	m_scaler.syMin = rect.top;
	m_scaler.sxMax = rect.right;
	m_scaler.syMax = rect.bottom;
	// 화면의 원점 
	m_scaler.sox = (m_scaler.sxMax + m_scaler.sxMin) / 2;
	m_scaler.soy = (m_scaler.syMax + m_scaler.syMin) / 2;
	
	switch(m_isFileType)
	{
		case GDFFILE:
			g_CLayerList.DrawLayer(pDC,m_scaler);
			break;
		case SHPFILE:
			g_CLayerListSHP.DrawLayer(pDC,m_scaler);
			break;
	}
	

	m_pIndexMapSheet->m_SheetIndexMap.Invalidate();
}
/////////////////////////////////////////////////////////////////////////////
// CGDFView printing

BOOL CGDFView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGDFView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CGDFView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CGDFView diagnostics

#ifdef _DEBUG
void CGDFView::AssertValid() const
{
	CView::AssertValid();
}

void CGDFView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGDFDoc* CGDFView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGDFDoc)));
	return (CGDFDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGDFView message handlers

void CGDFView::OnFileOpen() 
{
	if(m_isFileType==SHPFILE)
	{
		MessageBox("SHP을 열었기 때문에 GDF을 사용 할 수 없습니다");
		return;
	}
	char	fileBuff[50000] = {0};	
	char	*pFile;
	CString str;
	int		fType;

	//-----경로 찾기-------//
	char szFileter[] = "GDF_File(*.GDF) | *.GDF; | All Files(*.*)|*.*||";
	CFileDialog dlg(true,NULL,NULL,OFN_FILEMUSTEXIST|OFN_ALLOWMULTISELECT,szFileter);

	dlg.m_ofn.lpstrFile = fileBuff;
	dlg.m_ofn.nMaxFile = sizeof(fileBuff);

	if(IDOK == dlg.DoModal())
	{
		POSITION pos = dlg.GetStartPosition();

		while(pos!=NULL)
		{
			CString filepath = dlg.GetNextPathName(pos);
			AfxGetApp()->OpenDocumentFile(filepath);
			pFile = LPSTR(LPCTSTR(filepath));
			fType = g_CLayerList.AddLayer(pFile);
			switch(fType)
			{
				case F_MAXLAYER:
					str.Format("Max Layer : %d 를 %s파일부터 초과되었습니다!!",MAX_LAYER_LIST,pFile);
					MessageBox(str);
					//return;
					break;
				case F_REOPEN:
					str.Format("%s 파일은 이미 읽은 파일 입니다!!",pFile);
					MessageBox(str);
					break;
				default:
					break;
			}
		}

		if(m_isOpenFile == false)
		{
			m_scaler.mxMin = g_CLayerList.m_Layer[0]->SPA_Header.LeftUpPos.X;		
			m_scaler.myMin = g_CLayerList.m_Layer[0]->SPA_Header.LeftUpPos.Y;	
			m_scaler.mxMax = g_CLayerList.m_Layer[0]->SPA_Header.RightDownPos.X;	
			m_scaler.myMax = g_CLayerList.m_Layer[0]->SPA_Header.RightDownPos.Y;		
			m_scaler.mox = (m_scaler.mxMax + m_scaler.mxMin) / 2;
			m_scaler.moy = (m_scaler.myMax + m_scaler.myMin) / 2;

			RECT rect;
			GetClientRect(&rect);
			// 화면의 크기
			double x1,y1,x2,y2;
			m_scaler.DeviceToWorld(rect.left,rect.top,&x1,&y1);
			m_scaler.DeviceToWorld(rect.right,rect.bottom,&x2,&y2);

			m_scaler.mxMin = x1;	
			m_scaler.myMin = y2;
			m_scaler.mxMax = x2;
			m_scaler.myMax = y1;
			m_scaler.mox = (m_scaler.mxMax + m_scaler.mxMin) / 2;
			m_scaler.moy = (m_scaler.myMax + m_scaler.myMin) / 2;

			//TEST END
			
			//Sheet Init
			CMainFrame* pFrm = ((CMainFrame*)(AfxGetApp()->m_pMainWnd));
			m_pSheet = pFrm->GetSheet();
			m_pSheet->m_SheetLayer.m_pLayerList = &g_CLayerList;
			m_pSheet->m_SheetLayer.m_pLayerListSHP = NULL;
			m_pSheet->m_SheetLayer.m_pParentScaler	= &m_scaler;
			
			m_pIndexMapSheet = pFrm->GetIndexMapSheet();
			m_pIndexMapSheet->m_SheetIndexMap.m_pLayerList = &g_CLayerList;
			m_pIndexMapSheet->m_SheetIndexMap.m_pParentScaler = &m_scaler;
			
			//mini map
			m_pIndexMapSheet->m_SheetIndexMap.m_Scaler.mxMin = g_CLayerList.m_Layer[0]->SPA_Header.LeftUpPos.X;		
			m_pIndexMapSheet->m_SheetIndexMap.m_Scaler.myMin = g_CLayerList.m_Layer[0]->SPA_Header.LeftUpPos.Y;	
			m_pIndexMapSheet->m_SheetIndexMap.m_Scaler.mxMax = g_CLayerList.m_Layer[0]->SPA_Header.RightDownPos.X;	
			m_pIndexMapSheet->m_SheetIndexMap.m_Scaler.myMax = g_CLayerList.m_Layer[0]->SPA_Header.RightDownPos.Y;		
			m_pIndexMapSheet->m_SheetIndexMap.m_Scaler.mox = (m_scaler.mxMax + m_scaler.mxMin) / 2;
			m_pIndexMapSheet->m_SheetIndexMap.m_Scaler.moy = (m_scaler.myMax + m_scaler.myMin) / 2;

			m_pIndexMapSheet->m_SheetIndexMap.m_mDC = &m_indexMapCDC;
			m_pIndexMapSheet->m_SheetIndexMap.m_Bitmap = &m_indexMapBitmap;			
		}

		m_isFileType = GDFFILE;
		m_isOpenFile = true;
		m_pSheet->m_SheetLayer.ListView();
		
		Invalidate();
	}
}

void CGDFView::OnLButtonDown(UINT nFlags, CPoint point) 
{

	
	if(m_isOpenFile==TRUE)
	{
		double mWidth ;
		double mHeight ;

		if(m_TypeInfo==ZOOMIN)
		{
			

			mWidth = m_scaler.mxMax - m_scaler.mxMin;
			mHeight = m_scaler.myMax - m_scaler.myMin;
			 
			 // 20% 축소
			 mWidth *= 1.2;
			 mHeight *= 1.2;

			 m_scaler.DeviceToWorld(point.x, point.y,
				 &m_scaler.mox, &m_scaler.moy);
			 
			 m_scaler.mxMin = m_scaler.mox - mWidth / 2;
			 m_scaler.mxMax = m_scaler.mox + mWidth / 2;
			 m_scaler.myMin = m_scaler.moy - mHeight / 2;
			 m_scaler.myMax = m_scaler.moy + mHeight / 2;
			Invalidate();
		}
		else if(m_TypeInfo==ZOOMOUT)
		{
			mWidth = m_scaler.mxMax - m_scaler.mxMin;
			mHeight = m_scaler.myMax - m_scaler.myMin;
			
			// 20% 확대
			mWidth *= 0.8;
			mHeight *= 0.8;

			m_scaler.DeviceToWorld(point.x, point.y,
				&m_scaler.mox, &m_scaler.moy);
			
			m_scaler.mxMin = m_scaler.mox - mWidth / 2;
			m_scaler.mxMax = m_scaler.mox + mWidth / 2;
			m_scaler.myMin = m_scaler.moy - mHeight / 2;
			m_scaler.myMax = m_scaler.moy + mHeight / 2;
			Invalidate();
		}
		else if(m_TypeInfo==MOVE)
		{
			m_ptZoomWindowLT = m_ptZoomWindowBR = point;
			SetCapture();
		}
		else if(m_TypeInfo==AREA)
		{
			m_ptZoomWindowLT = m_ptZoomWindowBR = point;
			SetCapture();
		}
	}		 
	
	CView::OnLButtonDown(nFlags, point);
}

void CGDFView::OnRButtonDown(UINT nFlags, CPoint point) 
{

	CView::OnRButtonDown(nFlags, point);
}

void CGDFView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	// 화면 크기 얻기
	CRect	tmpRect;
	GetClientRect(tmpRect);
	
	m_scaler.sxMin = 0;
	m_scaler.sxMax = tmpRect.right;
	m_scaler.syMin = 0;
	m_scaler.syMax = tmpRect.bottom;
	
	// 화면의 원점 
	m_scaler.sox = (m_scaler.sxMax + m_scaler.sxMin) / 2;
	m_scaler.soy = (m_scaler.syMax + m_scaler.syMin) / 2;
	// TODO: Add your message handler code here
	
}

void CGDFView::OnZoomout() 
{
	m_TypeInfo = ZOOMOUT;	
}

void CGDFView::OnZoonin() 
{
	m_TypeInfo = ZOOMIN;	
}

void CGDFView::OnMove() 
{
	m_TypeInfo = MOVE;
}

void CGDFView::OnArea() 
{
	m_TypeInfo = AREA;
}

void CGDFView::OnUpdateZoonin(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_TypeInfo==ZOOMIN);	
}

void CGDFView::OnUpdateZoomout(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_TypeInfo==ZOOMOUT);
}

void CGDFView::OnUpdateMove(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_TypeInfo==MOVE);
}

void CGDFView::OnUpdateArea(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_TypeInfo==AREA);
}

void CGDFView::OnExtent() 
{
	
	BOUND FullExtent;
	int i;

	switch(m_isFileType)
	{
	case GDFFILE:
		if(g_CLayerList.m_Cnt< 1)
			return;
		
		
		FullExtent.MinX =  g_CLayerList.m_Layer[0]->SPA_Header.LeftUpPos.X;
		FullExtent.MinY =  g_CLayerList.m_Layer[0]->SPA_Header.LeftUpPos.Y;
		FullExtent.MaxX =  g_CLayerList.m_Layer[0]->SPA_Header.RightDownPos.X;
		FullExtent.MaxY =  g_CLayerList.m_Layer[0]->SPA_Header.RightDownPos.Y;
		
		
		for(i=1; i< g_CLayerList.m_Cnt; i++)
		{
			FullExtent.MinX =  (FullExtent.MinX < g_CLayerList.m_Layer[i]->SPA_Header.LeftUpPos.X) ?
				FullExtent.MinX : g_CLayerList.m_Layer[i]->SPA_Header.LeftUpPos.X;
			FullExtent.MinY =  (FullExtent.MinY < g_CLayerList.m_Layer[i]->SPA_Header.LeftUpPos.Y) ?
				FullExtent.MinY : g_CLayerList.m_Layer[i]->SPA_Header.LeftUpPos.Y;
			FullExtent.MaxX =  (FullExtent.MaxX > g_CLayerList.m_Layer[i]->SPA_Header.RightDownPos.X) ?
				FullExtent.MaxX : g_CLayerList.m_Layer[i]->SPA_Header.RightDownPos.X;
			FullExtent.MaxY =  (FullExtent.MaxY > g_CLayerList.m_Layer[i]->SPA_Header.RightDownPos.Y) ?
				FullExtent.MaxY : g_CLayerList.m_Layer[i]->SPA_Header.RightDownPos.Y;
		}
		
		break;
	

	case SHPFILE:
		if(g_CLayerListSHP.m_LayerCount < 1)
			return;
		
		
		FullExtent.MinX =  g_CLayerListSHP.m_Layers[0]->m_shp.m_MBR.xmin;
		FullExtent.MinY =  g_CLayerListSHP.m_Layers[0]->m_shp.m_MBR.ymin;
		FullExtent.MaxX =  g_CLayerListSHP.m_Layers[0]->m_shp.m_MBR.xmax;
		FullExtent.MaxY =  g_CLayerListSHP.m_Layers[0]->m_shp.m_MBR.ymax;

		for(i=1; i< g_CLayerListSHP.m_LayerCount; i++)
		{
			FullExtent.MinX =  (FullExtent.MinX < g_CLayerListSHP.m_Layers[i]->m_shp.m_MBR.xmin) ?
				FullExtent.MinX : g_CLayerListSHP.m_Layers[i]->m_shp.m_MBR.xmin;
			FullExtent.MinY =  (FullExtent.MinY < g_CLayerListSHP.m_Layers[i]->m_shp.m_MBR.ymin) ?
				FullExtent.MinY : g_CLayerListSHP.m_Layers[i]->m_shp.m_MBR.ymin;
			FullExtent.MaxX =  (FullExtent.MaxX > g_CLayerListSHP.m_Layers[i]->m_shp.m_MBR.xmax) ?
				FullExtent.MaxX : g_CLayerListSHP.m_Layers[i]->m_shp.m_MBR.xmax;
			FullExtent.MaxY =  (FullExtent.MaxY > g_CLayerListSHP.m_Layers[i]->m_shp.m_MBR.ymax) ?
				FullExtent.MaxY : g_CLayerListSHP.m_Layers[i]->m_shp.m_MBR.ymax;
		}
		//임시 지워여함!!//
		m_scaler.mxMin = FullExtent.MinX;
		m_scaler.myMin = FullExtent.MinY;
		m_scaler.mxMax = FullExtent.MaxX;
		m_scaler.myMax = FullExtent.MaxY;
		m_scaler.mox = (m_scaler.mxMax + m_scaler.mxMin) / 2;
		m_scaler.moy = (m_scaler.myMax + m_scaler.myMin) / 2;
		// 임시
		break;
	}
	m_scaler.mxMin = FullExtent.MinX;
	m_scaler.myMin = FullExtent.MinY;
	m_scaler.mxMax = FullExtent.MaxX;
	m_scaler.myMax = FullExtent.MaxY;
	m_scaler.mox = (m_scaler.mxMax + m_scaler.mxMin) / 2;
	m_scaler.moy = (m_scaler.myMax + m_scaler.myMin) / 2;

	// 임시
	if(m_isFileType == GDFFILE)
	{
		RECT rect;
		GetClientRect(&rect);
		// 화면의 크기
		double x1,y1,x2,y2;
		m_scaler.DeviceToWorld(rect.left,rect.top,&x1,&y1);
		m_scaler.DeviceToWorld(rect.right,rect.bottom,&x2,&y2);
		
		m_scaler.mxMin = x1;	
		m_scaler.myMin = y2;
		m_scaler.mxMax = x2;
		m_scaler.myMax = y1;
		m_scaler.mox = (m_scaler.mxMax + m_scaler.mxMin) / 2;
		m_scaler.moy = (m_scaler.myMax + m_scaler.myMin) / 2;
	}
	//  임시 끝
	
	
	Invalidate();
}

void CGDFView::OnUpdateExtent(CCmdUI* pCmdUI) 
{
}


void CGDFView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	COORD2	tmpPoint1, tmpPoint2;
	
	if(m_TypeInfo == AREA && m_isOpenFile == true)
	{
		ReleaseCapture();  
		
		BOUND newMBR;
		
		if ((fabs(m_ptZoomWindowLT.x - m_ptZoomWindowBR.x) < 20) || 
			(fabs(m_ptZoomWindowLT.y - m_ptZoomWindowBR.y) < 20))
			return;
		
		m_ptZoomWindowBR = point;
		m_scaler.DeviceToWorld(m_ptZoomWindowLT.x, m_ptZoomWindowLT.y,
			&tmpPoint1.X, &tmpPoint1.Y);
		m_scaler.DeviceToWorld(m_ptZoomWindowBR.x, m_ptZoomWindowBR.y,
			&tmpPoint2.X, &tmpPoint2.Y);
		
		
		if(tmpPoint1.X > tmpPoint2.X)
		{
			newMBR.MaxX = tmpPoint1.X;
			newMBR.MinX = tmpPoint2.X;
		}
		else
		{
			newMBR.MaxX = tmpPoint2.X;
			newMBR.MinX = tmpPoint1.X;
		}
		
		if(tmpPoint1.Y > tmpPoint2.Y)
		{
			newMBR.MaxY = tmpPoint1.Y;
			newMBR.MinY = tmpPoint2.Y;
		}
		else
		{
			newMBR.MaxY = tmpPoint2.Y;
			newMBR.MinY = tmpPoint1.Y;
		}
		
		
		// GetClientRect(&rect);  // 화면의 크기를 받아온다.
		m_scaler.mxMin = newMBR.MinX;
		m_scaler.myMin = newMBR.MinY;
		m_scaler.mxMax = newMBR.MaxX;
		m_scaler.myMax = newMBR.MaxY;
		m_scaler.mox = (m_scaler.mxMax + m_scaler.mxMin) / 2;
		m_scaler.moy = (m_scaler.myMax + m_scaler.myMin) / 2;
		Invalidate();
	}
	
	else if(m_TypeInfo == MOVE && m_isOpenFile == true)
	{
		ReleaseCapture();  
	}
		
	
	CView::OnLButtonUp(nFlags, point);
}

void CGDFView::OnMouseMove(UINT nFlags, CPoint point) 
{
	CMainFrame* pWnd = (CMainFrame*) AfxGetMainWnd();
	
	if(m_isOpenFile)
	{
		double dx, dy;
		
		// 마우스 포인트의 화면좌표를 실좌표 값으로 변환
		m_scaler.DeviceToWorld(point.x, point.y, &dx, &dy);
		
		// 변환된 실좌표 값을 DisplayMessage함수에 인자로 넘긴다
		pWnd->DisplayMessage(dx, dy);
	}
	if(m_TypeInfo==AREA && m_isOpenFile == true)
	{
		if(nFlags & MK_LBUTTON)
		{
			CClientDC	dc(this);
			CPen	cPen, *oldPen;
			
			cPen.CreatePen(PS_DOT, 1, RGB(0,255,255));
			oldPen = (CPen*)dc.SelectObject(&cPen);
			dc.SetROP2(R2_XORPEN);
			
			//기존의 BOX을 지운다.
			dc.MoveTo(m_ptZoomWindowLT);
			dc.LineTo(m_ptZoomWindowLT.x, m_ptZoomWindowBR.y);
			dc.LineTo(m_ptZoomWindowBR);
			dc.LineTo(m_ptZoomWindowBR.x, m_ptZoomWindowLT.y);
			dc.LineTo(m_ptZoomWindowLT);
			// 새로운 BOX를 그린다.
			dc.MoveTo(m_ptZoomWindowLT);
			dc.LineTo(m_ptZoomWindowLT.x, point.y);
			dc.LineTo(point);
			dc.LineTo(point.x, m_ptZoomWindowLT.y);
			dc.LineTo(m_ptZoomWindowLT);
			
			m_ptZoomWindowBR = point;
			dc.SelectObject(oldPen);
		}
		
	}
	else if(m_TypeInfo == MOVE && m_isOpenFile == true)
	{
		if(nFlags & MK_LBUTTON)
		{
			//////////////////////////////////////
			// 마우스가 움직이는 동안 지도가 따라 움직이도록  한다.
			m_ptZoomWindowBR = point;
			double mx1, mx2, my1, my2;
			m_scaler.DeviceToWorld(m_ptZoomWindowLT.x, m_ptZoomWindowLT.y, &mx1, &my1);
			m_scaler.DeviceToWorld(m_ptZoomWindowBR.x, m_ptZoomWindowBR.y, &mx2, &my2);
			
			m_scaler.mox += (mx1-mx2);
			m_scaler.moy += (my1-my2);
			
			double mWidth = m_scaler.mxMax - m_scaler.mxMin;
			double mHeight = m_scaler.myMax - m_scaler.myMin;
			
			m_scaler.mxMin = m_scaler.mox - (mWidth / 2);
			m_scaler.mxMax = m_scaler.mox + (mWidth / 2);
			m_scaler.myMin = m_scaler.moy - (mHeight / 2);
			m_scaler.myMax = m_scaler.moy + (mHeight / 2);
			
			Invalidate();
			
			m_ptZoomWindowLT = point;	
		}
	}
	CView::OnMouseMove(nFlags, point);
}


void CGDFView::OnBtnLayer() 
{
	m_DigLayerList.m_LayerList = &g_CLayerList;
	m_DigLayerList.DoModal();
	Invalidate();
	//g_CLayerList.DrawLayer(pDC,m_scaler);
}

void CGDFView::OnSHPOpen() 
{
	if(m_isFileType==GDFFILE)
	{
		MessageBox("GDF을 열었기 때문에 SHP을 사용 할 수 없습니다");
		return;
	}
	CFileDialog dlg(TRUE, NULL, NULL, OFN_READONLY, 
		"SHP Files (*.shp)|*.shp|All Files (*.*)|*.*|", NULL);
	
	// SHP Loading
	if(dlg.DoModal() != IDOK)
		return;
	
	CString filepath;
	char fname[500];
	filepath = dlg.GetPathName();
	strcpy(fname, (LPSTR)(LPCTSTR)filepath);
	
	BeginWaitCursor();
	//m_shp.Open(fname);
	
	g_CLayerListSHP.AddLayer(fname);
	EndWaitCursor();
	
	if(m_isOpenFile == false)
	{
		m_scaler.mxMin = g_CLayerListSHP.m_Layers[0]->m_shp.m_MBR.xmin;
		m_scaler.myMin = g_CLayerListSHP.m_Layers[0]->m_shp.m_MBR.ymin;
		m_scaler.mxMax = g_CLayerListSHP.m_Layers[0]->m_shp.m_MBR.xmax;
		m_scaler.myMax = g_CLayerListSHP.m_Layers[0]->m_shp.m_MBR.ymax;
		m_scaler.mox = (m_scaler.mxMax + m_scaler.mxMin) / 2;
		m_scaler.moy = (m_scaler.myMax + m_scaler.myMin) / 2;

		//Sheet Init
		CMainFrame* pFrm = ((CMainFrame*)(AfxGetApp()->m_pMainWnd));
		m_pSheet = pFrm->GetSheet();
		m_pSheet->m_SheetLayer.m_pLayerListSHP = &g_CLayerListSHP;
		m_pSheet->m_SheetLayer.m_pLayerList = NULL;
		m_pSheet->m_SheetLayer.m_pParentScaler	= &m_scaler;
		
		m_pIndexMapSheet = pFrm->GetIndexMapSheet();
		m_pIndexMapSheet->m_SheetIndexMap.m_pLayerListSHP = &g_CLayerListSHP;
		m_pIndexMapSheet->m_SheetIndexMap.m_pParentScaler = &m_scaler;
		
		//mini map
		m_pIndexMapSheet->m_SheetIndexMap.m_Scaler.mxMin = g_CLayerListSHP.m_Layers[0]->m_shp.m_MBR.xmin;	
		m_pIndexMapSheet->m_SheetIndexMap.m_Scaler.myMin = g_CLayerListSHP.m_Layers[0]->m_shp.m_MBR.ymin;;	
		m_pIndexMapSheet->m_SheetIndexMap.m_Scaler.mxMax = g_CLayerListSHP.m_Layers[0]->m_shp.m_MBR.xmax;
		m_pIndexMapSheet->m_SheetIndexMap.m_Scaler.myMax = g_CLayerListSHP.m_Layers[0]->m_shp.m_MBR.ymax;		
		m_pIndexMapSheet->m_SheetIndexMap.m_Scaler.mox = (m_scaler.mxMax + m_scaler.mxMin) / 2;
		m_pIndexMapSheet->m_SheetIndexMap.m_Scaler.moy = (m_scaler.myMax + m_scaler.myMin) / 2;
		
	}

	m_isFileType = SHPFILE;
	m_isOpenFile = true;
	m_pSheet->m_SheetLayer.ListView();
	Invalidate();
}
