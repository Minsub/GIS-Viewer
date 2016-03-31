// Labeler.cpp: implementation of the Labeler class.
//
//////////////////////////////////////////////////////////////////////

#include "../stdafx.h"
#include "../Shape/Labeler.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


Labeler::Labeler()
{
	m_isLoad= false;
	pField = NULL;
	labels = NULL;
	
}

Labeler::~Labeler()
{
	if(pField!= NULL)
		delete[] pField;

	FreeAllLabelObject();

}

#define DEFFIELD 0
void Labeler::readDBF()
{
    FILE *fp;
    char ver;
    
    char *record;

    if( (fp = fopen( m_strDBFPath, "rb" )) == NULL ){
        AfxMessageBox( "파일 열기 실패(읽기)" );
        return ;
    }

    fread( &ver, 1, 1, fp );

    if(ver != 3){
        fclose(fp);
        return ;
    }

	// 처음 파일을 읽을 때에만 파일 헤더와 필드 헤더 읽기 
	if(pField== NULL)
	{
		// 파일 헤더 읽어오기
		fseek(fp, 0, SEEK_SET);
		fread(&fHeader, sizeof(DBF_FILEHEADER), 1, fp);
		
		// 필드의 개수 구하기
		// (전체 헤더 크기에서 File Header와 0x0D(1) 크기를 빼고 필드헤더 크기로 나눔) 
		nfield = (fHeader.header_length - sizeof(DBF_FILEHEADER)-1)
			/ sizeof(DBF_FILEDDESCRIPTOR);
		
		//필드의 개수만큼 Field descriptor 생성하기
		//필드 정보 읽어내기
		pField = new DBF_FILEDDESCRIPTOR[nfield];
		fread(pField, sizeof(DBF_FILEDDESCRIPTOR)*nfield, 1, fp);
		
		//File Header와 Field Descriptor , 0x0D 건너띄기
		int offset = fHeader.header_length ;
		fseek(fp, offset, SEEK_SET);
		
		labels = new LabelObject[fHeader.recordn];  
	}
	
     //Read Field record
    for(int j=0; j < (int)fHeader.recordn ; j++){
 	    // 첫 삭제 바이트 건너띄기
		  fseek(fp, 1, SEEK_CUR);
        for(int i=0; i<nfield; i++){
            record = new char[pField[i].field_length+1];
            fread(record, pField[i].field_length, 1, fp);

	   // NULL 문자 추가 첫 삭제 바이트 건너띄기
            record[pField[i].field_length] = '\0'; 
	   //공백제거
            removeSpace(record,pField[i].field_length); 
			
			/////////////////////////////////////
			//
			if(i==2)
				labels[j].label = record;
			/////////////////////////////////////

            delete[] record;
        }
       
   }
    fclose(fp);


	// 처음 읽었을 때만 라벨 위치 다시 계산 
	if(m_isLoad== false)
		CalculateLabelPoint();

	m_isLoad= true;
}

void Labeler::removeSpace(char *rec, int length)
{
    for(short int i=length-1; i>0; i--){
        if(rec[i] == ' ')
            rec[i] = 0;
        else
            break;
    }
}

void Labeler::Draw(CDC *pDC, Scaler &sc)
{
	if(m_isLoad == false)
		return;
	int i;
	double sx, sy;
	CPoint pt;
	//	DrawText(pDC, str, tmpPoint.x, tmpPoint.y-10);
		//    pDC->TextOut(tmpPoint.x, tmpPoint.y-10, str);
	///////////////////////////////
	// 라벨링
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextAlign(TA_CENTER);

		for(i=0; i<shp->m_nRecords ; i++)
		{
			sc.WorldToDevice(labels[i].x,
				labels[i].y, &sx,&sy);
			pt.x = (long)sx;
			pt.y = (long)sy;

			pDC->TextOut(pt.x, pt.y,labels[i].label);
			//DrawText(pDC, label[i], pt.x, pt.y-10);
		}

}

void Labeler::DrawText(CDC *pDC, CString str, long x, long y)
{
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextAlign(TA_CENTER);
	pDC->SetTextColor(RGB(255,255,255));
	pDC->TextOut(x-1,y-1, str);
	pDC->TextOut(x,y-1, str);
	pDC->TextOut(x+1,y-1, str);
	pDC->TextOut(x-1,y, str);
	pDC->TextOut(x,y, str);
	pDC->TextOut(x+1,y, str);
	pDC->TextOut(x-1,y+1, str);
	pDC->TextOut(x,y+1, str);
	pDC->TextOut(x+1,y+1, str);
	pDC->SetTextColor(RGB(0,0,0));
	pDC->TextOut(x,y, str);
}

bool Labeler::CalculateLabelPoint()
{
	int i;
	double lx, ly;

	
	switch(shp->m_nShapeType)
	{
	case SHPT_POINT :
		for(i=0; i<shp->m_nRecords ; i++)
		{
			labels[i].x = shp->m_pSHPPoints[i].x;
			labels[i].y = shp->m_pSHPPoints[i].y;
			
		}

		break;
	case SHPT_ARC :
	case SHPT_POLYGON :		
		for(i = 0; i < shp->m_nRecords; ++i)
		{
			lx = (shp->m_pSHPPolyObjects[i].m_MBR.xmax + shp->m_pSHPPolyObjects[i].m_MBR.xmin) / 2;
			ly = (shp->m_pSHPPolyObjects[i].m_MBR.ymax + shp->m_pSHPPolyObjects[i].m_MBR.ymin) / 2;

			labels[i].x = lx;
			labels[i].y = ly;

		}
		break;

	}
	return true;
}

void Labeler::Open(char *filename, int index)
{

	if(m_isLoad== false)
	{
		int lengthfilename;
		lengthfilename = strlen(filename) -4;	// ".shp"를 빼는 과정
		
		//Get DBF file path
		for(int w=0; w<lengthfilename; w++)
		{ 
			m_strDBFPath[w] = filename[w];
			if(w==lengthfilename-1)
			{
				m_strDBFPath[w+1]=0;
				strcat(m_strDBFPath, ".dbf");
			}
		}	

	}

	dbf_index = index;
	readDBF();
}

void Labeler::FreeAllLabelObject()
{
	if(labels != NULL)
	{
		delete[] labels;
		labels = NULL;
	}
}
