// Layer.h: interface for the Layer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LAYER_H__DEA5EFF1_0BC9_405F_80DE_3C222DC3CFA9__INCLUDED_)
#define AFX_LAYER_H__DEA5EFF1_0BC9_405F_80DE_3C222DC3CFA9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Shapefile.h"
#include "Labeler.h"

class SHPLayer  
{
public:

	CShapefile m_shp;	
	Labeler m_label;

	bool m_isOn;		// 레이어 표시 여부 
	bool m_isSelect;	// 레이어 선택 여부 (공간 검색시 사용)
	bool m_isLabelOn;	// 레이어의 라벨 표시 여부 
	
	int m_MaxScale;		// 레이어 표시 최대 스캐일
	int m_MinScale;		// 레이어 표시 최소 스케일

	SHPLayer();
	void Draw(CDC* pDC, Scaler& sc);
	void OpenFile(char* filename);
	virtual ~SHPLayer();

};

#endif // !defined(AFX_LAYER_H__DEA5EFF1_0BC9_405F_80DE_3C222DC3CFA9__INCLUDED_)
