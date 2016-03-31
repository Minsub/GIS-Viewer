// Shapefile.cpp: implementation of the CShape class.

#include "../stdafx.h"
#include "../Shape/Shapefile.h"
//#include "Labeler.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CShapefile::CShapefile()
{
	m_bOpen = false;
	
	m_MBR.xmax = m_MBR.ymax = m_MBR.xmin = m_MBR.ymin = 0.;
	
	m_nShapeType = 0;
	
	m_pSHPPoints = NULL;
	m_pSHPPolyObjects = NULL;

//	m_labeler = new Labeler;
//	m_labeler->shp = this;
	
}

CShapefile::~CShapefile()
{
	if(m_bOpen== true)
		Close();
}

void CShapefile::SwapWord(int _length, void* _pWord)   
{
	int		i;
	unsigned char	temp;
	
	for( i=0; i< _length/2; i++ )
	{
		temp = ((unsigned char *) _pWord)[i];
		((unsigned char *) _pWord)[i] = ((unsigned char *) _pWord)[_length-i-1];
		((unsigned char *) _pWord)[_length-i-1] = temp;
	}
}

bool CShapefile::Open(char* _filename)
{
	FILE *fpSHP, *fpSHX;
	int tmp, i, j;
	int lengthfilename;
	
	
	strcpy(m_strSHPPath, _filename);	//SHP file path
	lengthfilename = strlen(m_strSHPPath) -4;	// ".shp"를 빼는 과정
	
	//Get SHX file path
	for(int w=0; w<lengthfilename; w++)
	{ 
		m_strSHXPath[w] = m_strSHPPath[w];
		if(w==lengthfilename-1)
		{
			m_strSHXPath[w+1]=0;
			strcat(m_strSHXPath,".shx" );
		}
	}
	
	
	//open SHP/SHX file
	fpSHP = fopen(m_strSHPPath, "rb");
	fpSHX = fopen(m_strSHXPath, "rb");
	
	if(fpSHP == NULL || fpSHX == NULL)
	{
		// 파일이 열리지 않으면 메시지박스 출력
		::AfxMessageBox("SHP File Error!! \n", MB_OK, -1);
		return false;
	}
	
	/* release memory of exsiting one */
	if(m_bOpen)
		Close();
	
	/* Now read a Header of SHP/SHX */
	// Get File size & Number of records
	fseek(fpSHX, 24L, SEEK_SET);
	fread(&tmp, 4, 1, fpSHX);
	SwapWord(4, &tmp);	// Byte Order Changing
	
	m_nFileSize = tmp * 2;	// get file size
	m_nRecords = (m_nFileSize - 100)/8;	//get a number of records
	
	// get shp type
	fseek(fpSHP, 32L, SEEK_SET);
	fread(&m_nShapeType, 4, 1, fpSHP);
	
	// Check a invalid shp type
	if(!(m_nShapeType == 1 || m_nShapeType == 3 || m_nShapeType == 5 || m_nShapeType == 8)) 
	{
		::AfxMessageBox("Invalid SHP Type.\nThis program only support Point, Arc, Polygon & MultiPoint SHP Type.\n", MB_OK, -1);
		return false;
	}
	
	// get a MBR
	fread(&m_MBR, sizeof(MBR), 1, fpSHP);
	
	// real routine of feature reading(point, arc, polygon, multipoint)
	switch(m_nShapeType)
	{
		// 포인트를 읽어들인다.
	case SHPT_POINT:
		m_pSHPPoints = new GeoPoint[m_nRecords];
		
		for(i = 0; i < m_nRecords; ++i)
		{
			// SHX에서는 레코드 정보의 첫 4바이트에 해당하는 feature의 좌표정보가 시작되는
			// 바이트 수 를 알려 준다.
			fseek(fpSHX, 100 + i * 8, SEEK_SET);
			fread(&tmp, 4, 1, fpSHX);
			SwapWord(4, &tmp);
			tmp=tmp * 2;
			
			fseek(fpSHP, tmp + 12, SEEK_SET);
			fread((GeoPoint*)&m_pSHPPoints[i], 16, 1, fpSHP);
		}
		break;
		
		// 폴리라인과 폴리곤을 읽어들인다.
	case SHPT_ARC :
	case SHPT_POLYGON:
		m_pSHPPolyObjects = new PolyObject[m_nRecords];
		
		for(i=0;i<m_nRecords;++i)
		{
			fseek(fpSHX, 100+i*8, SEEK_SET);
			fread(&tmp, 4, 1, fpSHX);
			SwapWord(4, &tmp);
			tmp=tmp*2;
			
			fseek(fpSHP, tmp+12, SEEK_SET);
			fread((MBR*)&m_pSHPPolyObjects[i].m_MBR, 32, 1, fpSHP);	//get MBR
			fread(&m_pSHPPolyObjects[i].m_nNumParts, 4, 1, fpSHP);	//get parts number
			fread(&m_pSHPPolyObjects[i].m_nNumPoints, 4, 1, fpSHP);  //get points number
			
			m_pSHPPolyObjects[i].m_pParts = new int[m_pSHPPolyObjects[i].m_nNumParts];
			m_pSHPPolyObjects[i].m_pPoints = new GeoPoint[m_pSHPPolyObjects[i].m_nNumPoints];
			
			for(j=0;j<m_pSHPPolyObjects[i].m_nNumParts;++j)
				fread(&m_pSHPPolyObjects[i].m_pParts[j], 4, 1, fpSHP);

				
			for(j=0;j<m_pSHPPolyObjects[i].m_nNumPoints;++j)
				fread(&m_pSHPPolyObjects[i].m_pPoints[j], 16, 1, fpSHP);
		}


		break;
		
		// 다중점을 읽어들인다.
	case SHPT_MULTIPOINT:
		m_pSHPPolyObjects = new PolyObject[m_nRecords];
		
		for(i=0;i<m_nRecords;++i)
		{
			fseek(fpSHX, 100+i*8, SEEK_SET);
			fread(&tmp, 4, 1, fpSHX);
			SwapWord(4, &tmp);
			tmp=tmp*2;
			
			fseek(fpSHP, tmp+12, SEEK_SET);
			fread((MBR*)&m_pSHPPolyObjects[i].m_MBR, 32, 1, fpSHP);	//get  MBR
			fread(&m_pSHPPolyObjects[i].m_nNumPoints, 4, 1, fpSHP);	// get points number
			
			m_pSHPPolyObjects[i].m_pPoints = new GeoPoint[m_pSHPPolyObjects[i].m_nNumPoints];
			
			for(j=0;j<m_pSHPPolyObjects[i].m_nNumPoints;++j)
				fread(&m_pSHPPolyObjects[i].m_pPoints[j], 16, 1, fpSHP);
		}
		break;
	}
	
	fclose(fpSHX);
	fclose(fpSHP);
	
	m_bOpen = true;
	return true;
}


// 할당된 메모리를 제거한다.
bool CShapefile::Close()
{
	if(m_bOpen)
	{
		int i;
		switch(m_nShapeType)
		{
		case SHPT_POINT:
			delete []m_pSHPPoints;
			break;
			
		case SHPT_MULTIPOINT:
			for(i = 0; i < m_nRecords; ++i)
				delete [] m_pSHPPolyObjects[i].m_pPoints;
			break;
			
		case SHPT_ARC:
		case SHPT_POLYGON:
			for(i = 0; i < m_nRecords; ++i)
			{
				delete [] m_pSHPPolyObjects[i].m_pParts;
				delete [] m_pSHPPolyObjects[i].m_pPoints;
			}
			delete[] m_pSHPPolyObjects;
			break;
		}
	}
	m_bOpen = false;
	return true;
}




void CShapefile::Draw(CDC *pDC, Scaler& sc)
{
	double sx, sy;
	int i;
		
	switch(m_nShapeType)
	{
	case SHPT_POINT :
		for(i=0; i<m_nRecords ; i++)
		{
			CPoint pt;	
			sc.WorldToDevice(m_pSHPPoints[i].x,
				m_pSHPPoints[i].y, &sx,&sy);
			pt.x = sx;
			pt.y = sy;
			pDC->Ellipse(pt.x-5, pt.y-5,
				pt.x+5, pt.y+5);
		}

		break;
	case SHPT_ARC :
		
		for(i = 0; i < m_nRecords; ++i)
		{
			// 변수명의 간결화
			int NumPoints = m_pSHPPolyObjects[i].m_nNumPoints;
			int NumParts = m_pSHPPolyObjects[i].m_nNumParts;
			// 하나의 feature에 속하는 모든점들의 공간을 동적 할당
			CPoint* pTempPoint = new CPoint[NumPoints];
			// 각 Part에 속하는 버텍스(점)들의 개수를 저장할 공간을 동적 할당
			int* pVertex = new int[NumParts];
			// 한 feature의 점을 pTempPoint에 저장
			for(int j = 0; j < NumPoints; ++j)
			{
				sc.WorldToDevice(m_pSHPPolyObjects[i].m_pPoints[j].x,
					m_pSHPPolyObjects[i].m_pPoints[j].y, &sx,&sy);
				pTempPoint[j].x = sx;
				pTempPoint[j].y = sy;
			}
			// 버텍스 수를 구하는 과정
			for(j = 0; j < NumParts; j++)	{
				// 마지막 Part의 버텍스 수는 전체 버텍스 수에서 마지막 Part의 인덱스를 빼줌
				if(j == NumParts - 1)	
					pVertex[j] = NumPoints - m_pSHPPolyObjects[i].m_pParts[j];
				// 버텍스는 다음 Part의 인덱스와 현재 Part의 인덱스 차와 같음
				else {
					pVertex[j] = m_pSHPPolyObjects[i].m_pParts[j+1]
						- m_pSHPPolyObjects[i].m_pParts[j];
				}
			}

			pDC->PolyPolyline(pTempPoint, (DWORD*)pVertex, NumParts);
			// 모든 동적 할당을 해제
			delete[] pTempPoint;
			delete[] pVertex;
		}
		break;
	case SHPT_POLYGON :
		
		for(i = 0; i < m_nRecords; ++i)
		{
			// 변수명의 간결화
			int NumPoints = m_pSHPPolyObjects[i].m_nNumPoints;
			int NumParts = m_pSHPPolyObjects[i].m_nNumParts;
			// 하나의 feature에 속하는 모든점들의 공간을 동적 할당
			CPoint* pTempPoint = new CPoint[NumPoints];
			// 각 Part에 속하는 버텍스(점)들의 개수를 저장할 공간을 동적 할당
			int* pVertex = new int[NumParts];
			// 한 feature의 점을 pTempPoint에 저장
			for(int j = 0; j < NumPoints; ++j)
			{
				sc.WorldToDevice(m_pSHPPolyObjects[i].m_pPoints[j].x,
					m_pSHPPolyObjects[i].m_pPoints[j].y, &sx,&sy);
				pTempPoint[j].x = sx;
				pTempPoint[j].y = sy;
			}
			// 버텍스 수를 구하는 과정
			for(j = 0; j < NumParts; j++)	{
				// 마지막 Part의 버텍스 수는 전체 버텍스 수에서 마지막 Part의 인덱스를 빼줌
				if(j == NumParts - 1)	
					pVertex[j] = NumPoints - m_pSHPPolyObjects[i].m_pParts[j];
				// 버텍스는 다음 Part의 인덱스와 현재 Part의 인덱스 차와 같음
				else {
					pVertex[j] = m_pSHPPolyObjects[i].m_pParts[j+1]
						- m_pSHPPolyObjects[i].m_pParts[j];
				}
			}

			pDC->PolyPolygon (pTempPoint, pVertex, NumParts);
			// 모든 동적 할당을 해제
			delete[] pTempPoint;
			delete[] pVertex;
		}
		break;

	}

}
