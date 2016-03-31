#include "stdafx.h"
#include "LayerList.h"



CLayerList::CLayerList()
{
	m_Cnt=0;
	for(int i=0; i< MAX_LAYER_LIST ; i++ )
	{
		m_Layer[i] = NULL;
	}
};

CLayerList::~CLayerList()
{
	for(int i=0; i< m_Cnt ; i++ )
	{
		if(m_Layer[i] != NULL)
			delete m_Layer[i];
	}
}


int CLayerList::AddLayer(char* pName)
{
	if(m_Cnt==MAX_LAYER_LIST)
	{
		return F_MAXLAYER;
	}
	for(int i=0; i< m_Cnt ; i++ )
	{
		if(!(strcmp(m_Layer[i]->GDF_Header.GeoName,pName)))
		{
			return F_REOPEN;
		}
	}

	m_Layer[m_Cnt] = new CLayer;
	m_Layer[m_Cnt]->FileReader(pName);
	m_Cnt++;
	
	return F_SUCCESS;
}

bool CLayerList::DelLayer(int cNum)
{
	delete m_Layer[cNum];
	m_Layer[cNum]=NULL;
	for(int i=cNum;i<m_Cnt-1;i++)
	{
		m_Layer[i] = m_Layer[i+1];
	}
	m_Layer[m_Cnt] = NULL; 
	m_Cnt--;

	return true;
}

void CLayerList::DrawLayer(CDC* dc,Scaler &sc)
{
	
	for(int i=0;i<m_Cnt;i++)
	{
		if(m_Layer[i]->GetHide()==true)
		{
			m_Layer[i]->Draw(dc,sc);
		}
		
	}
}