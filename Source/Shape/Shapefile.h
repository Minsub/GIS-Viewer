// Shapefile.h: interface for the CShape class.
//


#ifndef _SHAPEFILE_H
#define _SHAPEFILE_H


#include "../Scaler.h"
#include "../Shape/objtype.h"

class Labeler;

class CShapefile  
{
public:	// for variables
	char		m_strSHPPath[500];			//Name of SHP file
	char		m_strSHXPath[500];			//Name of SHX file
	
	int		m_nRecords;					// Number of Record
	int		m_nShapeType;				/* SHPT_* */
	MBR		m_MBR;					// Minimub Bouding Rectangle
		
	// 공간데이터 멤버 
	GeoPoint*	m_pSHPPoints;				// Real MapPoint
	PolyObject*	m_pSHPPolyObjects;			// Real PolyObject
	
	bool		m_bOpen;					// To check whether SHP is Opened or not
	int		m_nFileSize;

	
public: // for functions

	void Draw(CDC* pDC, Scaler& sc);
	CShapefile();
	~CShapefile();
	
	void		SwapWord(int _length, void* _pWord);	// Byte order changing
	bool		Open(char*	_filename);				// SHP file open
	bool		Close();
	
	
};

#endif