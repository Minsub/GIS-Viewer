// DigLayerList.cpp : implementation file
//

#include "stdafx.h"
#include "GDF.h"
#include "DigLayerList.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DigLayerList dialog


DigLayerList::DigLayerList(CWnd* pParent /*=NULL*/)
	: CDialog(DigLayerList::IDD, pParent)
{
	//{{AFX_DATA_INIT(DigLayerList)
	//}}AFX_DATA_INIT
}


void DigLayerList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DigLayerList)
	DDX_Control(pDX, IDC_LIST1, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DigLayerList, CDialog)
	//{{AFX_MSG_MAP(DigLayerList)
	ON_BN_CLICKED(IDC_BtnHide, OnBtnHide)
	ON_BN_CLICKED(IDC_BtnDel, OnBtnDel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DigLayerList message handlers



BOOL DigLayerList::OnInitDialog() 
{
	CDialog::OnInitDialog();
		
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
	m_list.InsertColumn(1,"Layer",LVCFMT_LEFT,100,-1);
	m_list.InsertColumn(2,"Path",LVCFMT_LEFT,385,-1);

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
	ListView();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void DigLayerList::OnBtnHide() 
{
	
	for(int i=0;i<m_LayerList->m_Cnt;i++)
	{
		if(m_list.GetCheck(i)==false)
		{
			m_LayerList->m_Layer[i]->m_Hide = false;
		}
		else
		{
			m_LayerList->m_Layer[i]->m_Hide = true;
		}
	}

	
	CDialog::OnCancel();
}

void DigLayerList::OnBtnDel() 
{
	CString str;
	int SelectMark;
	
	SelectMark = m_list.GetSelectionMark();
	str.Format("Layer: %s »èÁ¦!!",m_LayerList->m_Layer[SelectMark]->GDF_Header.LayerName);
	m_LayerList->DelLayer(SelectMark);
	MessageBox(str);
	ListView();
}

void DigLayerList::ListView()
{
	m_list.DeleteAllItems();
	for(int i=0;i<m_LayerList->m_Cnt;i++)
	{
		m_list.InsertItem(i,"");
		if(m_LayerList->m_Layer[i]->m_Hide==true)
		{
			m_list.SetCheck(i,TRUE);
		}
		else
		{
			m_list.SetCheck(i,FALSE);
		}
		m_list.SetItem(i,1,LVIF_TEXT,m_LayerList->m_Layer[i]->GDF_Header.LayerName,0,0,0,NULL);
		m_list.SetItem(i,2,LVIF_TEXT,m_LayerList->m_Layer[i]->GDF_Header.GeoName,0,0,0,NULL);
	}
}
