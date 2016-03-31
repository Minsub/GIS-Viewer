#ifndef __LAYER_H__
#define __LAYER_H__

#include "GDFHeader.h"
#include "Scaler.h"
#include <afxtempl.h>

#include "resource.h"
#include "R_Tree/ortree.h"

class CLayer : public CWnd
{

public:
	CLayer();
	~CLayer();
	void		FileReader(char* pName);
	void		TypeReader(char* buff,PAGE_OFFSET* offset);
	void		Draw(CDC* dc,Scaler& sc);
	COLORREF	SeletColor(int cNum);
	int			SelectSym(int num);
	bool		GetHide()		{	return m_Hide;	};

	//데이터 저장 공간
	CList<void*,void*&> DataRecord;			//리스트
	OpsRTree m_RTree;


	//		GDF File Header
	GDF_HDL			GDF_Header;
	
	//		Link Part		//
	LINK_HDL		LINK_Header;		
	LINK_RECORD		*pL_Record;
	LINK_N_RECORD	*pN_Record;
	
	//		Spatial Part	//
	SPATIAL_HDL		SPA_Header;
	PAT_HEADER		PAT_Header;
	PAT				*Pat;

	bool	m_Hide;
	//TEST
	Scaler *m_pScaler;
	int tmpcnt,tmpcnt2;
	//OpsFloatExtent odExt;
private:
	
};


#endif