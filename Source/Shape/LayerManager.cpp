// LayerManager.cpp: implementation of the LayerManager class.
//
//////////////////////////////////////////////////////////////////////

#include "../stdafx.h"
//#include "GISVIewer.h"
#include "LayerManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

LayerManager::LayerManager()
{
	m_LayerCount = 0;
	for(int i=0; i< m_LayerCount ; i++ )
	{
		m_Layers[i] = NULL;
	}
}

LayerManager::~LayerManager()
{
	for(int i=0; i< m_LayerCount ; i++ )
	{
		if(m_Layers[i] != NULL)
			delete m_Layers[i];
	}
}

void LayerManager::AddLayer(char *filename)
{
	m_Layers[m_LayerCount] = new SHPLayer;
	m_Layers[m_LayerCount]->OpenFile(filename);
	m_LayerCount++;
}

void LayerManager::RemoveLayer(int index)
{
	if(index >= m_LayerCount)
		return;
	
	delete m_Layers[index] ;

	// 뒤에 있는 레이어 당겨오기 
	for(int i=index ; i< 10 ; i++)
	{
		if(i>=m_LayerCount-1)
		{
			m_Layers[i] = NULL;
		}
		else
		{
			m_Layers[i] = m_Layers[i+1];
		}
	}
	
	m_LayerCount--;

}

void LayerManager::MovePrevious(int index)
{
	if(index >= m_LayerCount || index == 0 )
		return;
	
	SHPLayer *tmp;
	tmp = m_Layers[index-1];
	m_Layers[index-1] = m_Layers[index];
	m_Layers[index] = tmp;

	
}

void LayerManager::MoveNext(int index)
{
	if(index >= m_LayerCount-1 )
		return;
	
	SHPLayer *tmp;
	tmp = m_Layers[index+1];
	m_Layers[index+1] = m_Layers[index];
	m_Layers[index] = tmp;

}

void LayerManager::DrawLayer(CDC *pDC, Scaler &sc)
{
	for(int i=0; i< m_LayerCount ; i++ )
	{
		m_Layers[i]->Draw(pDC, sc);
	}
}

void LayerManager::LayerOff(int index)
{
	if(index >= m_LayerCount )
		return;
	m_Layers[index]->m_isOn = false;
}

void LayerManager::LayerOn(int index)
{
	if(index >= m_LayerCount )
		return;
	m_Layers[index]->m_isOn = true;
}
