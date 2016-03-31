// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "GDF.h"

#include "MainFrm.h"
#include "Layer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_BtnLayer, OnBtnLayer)
	ON_COMMAND(ID_SHPOpen, OnSHPOpen)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);


	if(!m_wndPannel.Create(this,IDD_PANNEL,CBRS_LEFT|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC|CBRS_GRIPPER,IDD_PANNEL))
	{
		TRACE("Fail to create DialogBar.");
		return -1;
	}
	
	m_wndPannel.SetWindowText("GIS Mamager");
	m_wndPannel.EnableDocking(CBRS_ALIGN_LEFT|CBRS_ALIGN_RIGHT);
	DockControlBar(&m_wndPannel);

	//Layer Sheet ADD
	CWnd* pSheetHolder = m_wndPannel.GetDlgItem(IDC_SheetHolder);
	m_pSheet = new Sheet("GIS Control sheet",pSheetHolder);
	if(!m_pSheet->Create(pSheetHolder, WS_CHILD|WS_VISIBLE,0))
	{
		delete m_pSheet;
		m_pSheet = NULL;
		return -1;
	}
	
	CRect rct;
	pSheetHolder->GetWindowRect(rct);
	m_pSheet->SetWindowPos(NULL,0,0,rct.Width(),rct.Height(),SWP_NOZORDER|SWP_NOACTIVATE);

	//IndexMap Sheet ADD
	CWnd* pIndexMapSheetHolder = m_wndPannel.GetDlgItem(IDC_IndexMapHolder);
	m_pIndexMapSheet = new CIndexMapSheet("GIS Control sheet",pIndexMapSheetHolder);
	if(!m_pIndexMapSheet->Create(pIndexMapSheetHolder, WS_CHILD|WS_VISIBLE,0))
	{
		delete m_pIndexMapSheet;
		m_pIndexMapSheet = NULL;
		return -1;
	}
	
	m_pIndexMapSheet->SetWindowPos(NULL,0,0,rct.Width(),rct.Height(),SWP_NOZORDER|SWP_NOACTIVATE);
	
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

// 윈도우 창의 Status Bar에 msg(지도좌표 값)내용을 출력
void CMainFrame::DisplayMessage(double x, double y)
{
	CString msg;
	msg.Format("실좌표 값 : (%.3f, %.3f)", x, y);
	m_wndStatusBar.SetPaneText(0, msg);
}

void CMainFrame::OnFileOpen() 
{
	/*
	char *cstr;
	CString strPathName;
	

	//임시//
	CLayer g_CLayer;
	CClientDC dc(this);
	//g_CLayer.FileReader("강.gdf",&dc);
	
	g_CLayer.test1(&dc);
	*/
	/*
	//-----경로 찾기-------//
	char szFileter[] = "GDF_File(*.GDF) | *.GDF | All Files(*.*)|*.*||";
	CFileDialog dlg(true,NULL,NULL,OFN_HIDEREADONLY,szFileter);
	if(IDOK == dlg.DoModal())
	{
		strPathName = dlg.GetFileName();
	}
	//cstring변환--//
	cstr = LPSTR(LPCTSTR(strPathName));

	*/
}

void CMainFrame::OnBtnLayer() 
{
	
	
}

void CMainFrame::OnSHPOpen() 
{
	// TODO: Add your command handler code here
	
}
