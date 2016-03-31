// SheetLayer.cpp : implementation file
//

#include "stdafx.h"
#include "GDF.h"
#include "SheetLayer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSheetLayer property page

IMPLEMENT_DYNCREATE(CSheetLayer, CPropertyPage)

CSheetLayer::CSheetLayer() : CPropertyPage(CSheetLayer::IDD)
{
	//{{AFX_DATA_INIT(CSheetLayer)
	//}}AFX_DATA_INIT
}

CSheetLayer::~CSheetLayer()
{
	m_pLayerList = NULL;
	m_pLayerListSHP = NULL;
}

void CSheetLayer::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSheetLayer)
	DDX_Control(pDX, IDC_SheetLayer, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSheetLayer, CPropertyPage)
	//{{AFX_MSG_MAP(CSheetLayer)
	ON_BN_CLICKED(IDC_BtnSheetDel, OnBtnSheetDel)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_SheetLayer, OnItemchangedSheetLayer)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSheetLayer message handlers

BOOL CSheetLayer::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
		/*
		char*	NameList[2] = {"Hide","Layer"};
		int		width[2]	= {80,200};
		
		LV_COLUMN col;
		for(int i=0;i<2;i++)
		{
			col.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
			col.fmt = LVCFMT_LEFT;
			col.pszText = NameList[i];
			col.iSubItem = i;
			col.cx = width[i];
			col.cchTextMax = 20;
			
			m_list.InsertColumn(i,&col);
		}*/
	m_list.InsertColumn(0,"Hide",LVCFMT_LEFT,50,-1);
	m_list.InsertColumn(1,"Layer",LVCFMT_LEFT,173,-1);

	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES | LVS_EX_GRIDLINES );  
	
/*
	LV_ITEM item;
	for (int j=0;j<10;j++)
	{
		for(int cn=0;cn<2;cn++)
		{
			item.mask = LVIF_TEXT;
			item.iItem = j;
			item.iSubItem = cn;
			item.pszText = NameList[cn];
			if(cn==0){
				m_list.InsertItem(&item);	
			}
			else
			{
				m_list.SetItem(&item);;
			}
		}
	}
*/	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CSheetLayer::ListView()
{
	m_isView = false;
	m_list.DeleteAllItems();

	//GDF
	if(m_pLayerList != NULL)
	{
		for(int i=0;i<m_pLayerList->m_Cnt;i++)
		{
			m_list.InsertItem(i,"");
			if(m_pLayerList->m_Layer[i]->m_Hide==true)
			{
				m_list.SetCheck(i,TRUE);
			}
			else
			{
				m_list.SetCheck(i,FALSE);
			}
			m_list.SetItem(i,1,LVIF_TEXT,m_pLayerList->m_Layer[i]->GDF_Header.LayerName,0,0,0,NULL);
			//m_list.SetItem(i,2,LVIF_TEXT,m_pLayerList->m_Layer[i]->GDF_Header.GeoName,0,0,0,NULL);
		}
	}

	//SHP
	if(m_pLayerListSHP != NULL)
	{
		for(int i=0;i<m_pLayerListSHP->m_LayerCount;i++)
		{
			m_list.InsertItem(i,"");
			if(m_pLayerListSHP->m_Layers[i]->m_isOn==true)
			{
				m_list.SetCheck(i,TRUE);
			}
			else
			{
				m_list.SetCheck(i,FALSE);
			}
			m_list.SetItem(i,1,LVIF_TEXT,m_pLayerListSHP->m_Layers[i]->m_shp.m_strSHPPath,0,0,0,NULL);
		}

	}
	m_isView = true;
}

void CSheetLayer::OnBtnSheetDel() 
{
	CString str;
	int SelectMark;
	POSITION pos;
	pos = m_list.GetFirstSelectedItemPosition();
	SelectMark = m_list.GetNextSelectedItem(pos);

	if(SelectMark ==-1)
	{
		return;
	}
	if(m_pLayerList != NULL)
	{
		str.Format("Layer: %s 삭제!!",m_pLayerList->m_Layer[SelectMark]->GDF_Header.LayerName);
		m_pLayerList->DelLayer(SelectMark);
	}
	if(m_pLayerListSHP != NULL)
	{
		str.Format("Layer: %s 삭제!!",m_pLayerListSHP->m_Layers[SelectMark]->m_shp.m_strSHPPath);
		m_pLayerListSHP->RemoveLayer(SelectMark);
	}

	MessageBox(str);
	ListView();
	AfxGetApp()->m_pMainWnd->Invalidate();
}

void CSheetLayer::OnItemchangedSheetLayer(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	
	int iItem = pNMListView->iItem;

	BOOL bCur = m_list.GetCheck(iItem);
	int layerID = m_list.GetItemCount()-iItem-1;
	//호출될때마다 화면을 갱신하면 안되기 때문에 다소 확인하는 절차가 번거롭더라도 확인해서 갱신한다.
	if(m_isView==true)
	{
		if(m_pLayerList != NULL)
		{
			if(m_pLayerList->m_Layer[iItem]->m_Hide != bCur)
			{
				m_pLayerList->m_Layer[iItem]->m_Hide = bCur;
				AfxGetApp()->m_pMainWnd->Invalidate();
				Invalidate();
			}
		}
		if(m_pLayerListSHP != NULL)
		{
			if(m_pLayerListSHP->m_Layers[iItem]->m_isOn != bCur)
			{
				m_pLayerListSHP->m_Layers[iItem]->m_isOn = bCur;
				AfxGetApp()->m_pMainWnd->Invalidate();
				Invalidate();
			}
		}
		
	}
	
	*pResult = 0;
}

void CSheetLayer::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
//	m_pLayerList->DrawLayer(dc,);
	
	// Do not call CPropertyPage::OnPaint() for painting messages
}
