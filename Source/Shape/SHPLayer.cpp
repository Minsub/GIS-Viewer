// Layer.cpp: implementation of the Layer class.
//
//////////////////////////////////////////////////////////////////////

#include "../stdafx.h"
//#include "GISVIewer.h"
#include "../Shape/SHPLayer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SHPLayer::SHPLayer()
{
	m_isOn = true;
	m_isLabelOn = false;
	m_MinScale = 0;
	m_MaxScale = 100000;
}

SHPLayer::~SHPLayer()
{

}

void SHPLayer::Draw(CDC* pDC, Scaler& sc)
{

	if(m_isOn == false || sc.nowRatio < m_MinScale || sc.nowRatio > m_MaxScale)
		return;

	m_shp.Draw(pDC, sc);

	if(m_isLabelOn == true)
		m_label.Draw(pDC, sc);
}

void SHPLayer::OpenFile(char* filename)
{
	if(m_shp.m_bOpen == false)
	{
		m_label.shp = &m_shp;
		m_shp.Open(filename);
		m_label.Open(filename,0);
		m_isLabelOn = true;
	}
}
