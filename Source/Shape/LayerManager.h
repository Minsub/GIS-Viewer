// LayerManager.h: interface for the LayerManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LAYERMANAGER_H__DBDA091B_F668_4F5F_A519_3E3E7CCED0B5__INCLUDED_)
#define AFX_LAYERMANAGER_H__DBDA091B_F668_4F5F_A519_3E3E7CCED0B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Shape/SHPLayer.h"
#include "../scaler.h"

class LayerManager  
{
public:
	void LayerOn(int index);
	void LayerOff(int index);
	void DrawLayer(CDC *pDC, Scaler& sc);
	void MoveNext(int index);
	void MovePrevious(int index);
	void RemoveLayer(int index);
	void AddLayer(char* filename);

	// 10의 레이어만 추가할 수 있음
	SHPLayer *m_Layers[10];
	int m_LayerCount;

	
	
	
	LayerManager();
	virtual ~LayerManager();


};

#endif // !defined(AFX_LAYERMANAGER_H__DBDA091B_F668_4F5F_A519_3E3E7CCED0B5__INCLUDED_)
