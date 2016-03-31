// Labeler.h: interface for the Labeler class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LABELER_H__53063506_6376_4254_AC74_DEA4CFC3C488__INCLUDED_)
#define AFX_LABELER_H__53063506_6376_4254_AC74_DEA4CFC3C488__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#include "../Scaler.h"
#include "Shapefile.h"
class CShapefile;


struct LabelObject
{
	double x;
	double y;
	CString label;
};

class Labeler  
{
public:
	void FreeAllLabelObject();


	CShapefile *shp;
	char		m_strDBFPath[500];

	// DBF파일 헤더
	DBF_FILEHEADER fHeader;
	// DBF 필드 헤더 배열
	DBF_FILEDDESCRIPTOR* pField ;
	// 필드의 갯수 
	int nfield;

	bool m_isLoad;
	// 라벨 데이터 
	LabelObject * labels;
	// DBF 상에서 라벨로 사용하는 속성 인덱스 
	int dbf_index;


	Labeler();
	~Labeler();

	void Open(char * filename, int index);
	void removeSpace(char *rec, int length);
	void readDBF();
	bool CalculateLabelPoint();
	void Draw(CDC *pDC, Scaler &scaler);
	void DrawText(CDC *pDC, CString str, long x, long y);
};

#endif // !defined(AFX_LABELER_H__53063506_6376_4254_AC74_DEA4CFC3C488__INCLUDED_)
