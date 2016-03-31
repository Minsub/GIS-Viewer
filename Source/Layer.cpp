#include "StdAfx.h"
#include "Layer.h"
#include <stdio.h>


CLayer::CLayer()
{
	tmpcnt = tmpcnt2 = 0;
	m_Hide = true;
}
CLayer::~CLayer()
{
	// LIST  저장 방식
// 	POSITION pos;
// 	pos = DataRecord.GetHeadPosition();
// 	while(pos!=NULL)
// 	{
// 		delete [] DataRecord.GetNext(pos);
// 	}
	
}

void CLayer::FileReader(char* pName)
{
	FILE *fp;
	int i,tmpSize,toPage,baseSize,defaultBaseSize;
	fp = fopen(pName,"rb");

	//GDF파일 헤더 읽기 
	fread(&GDF_Header,sizeof(GDF_HDL),1,fp);		
	strcpy(GDF_Header.GeoName,pName);

	//LINK PART 헤더 읽기
	fread(&LINK_Header,sizeof(LINK_HDL),1,fp);
	
	//L_RECORD 읽기(동적)
	tmpSize = LINK_Header.GmtCnt;
	pL_Record = new LINK_RECORD[tmpSize];
	for(i=0;i<tmpSize;i++){
		fread(&pL_Record[i],sizeof(LINK_RECORD),1,fp);
	}
	
	//N_RECORD 가 있으면 읽기 (동적)
	tmpSize = LINK_Header.NodeCnt;
	if(LINK_Header.NodeCnt!=0)
	{
		pN_Record = new LINK_N_RECORD[tmpSize];
		fread(pN_Record,sizeof(LINK_N_RECORD),tmpSize,fp);
	}
	//SPATIAL PART 헤더 읽기
	fread(&SPA_Header,sizeof(SPATIAL_HDL),1,fp);
	
	//PAGE ALLOCATION HEADER 읽기
	fread(&PAT_Header,sizeof(PAT_HEADER),1,fp);

	
	//PAGE ALLOCATION TABLE 읽기 (동적)
	tmpSize = PAT_Header.AllocNum;
	Pat = new PAT[tmpSize];
	fread(Pat,sizeof(PAT),tmpSize,fp);
	
	//		PAGE 1개 단위 크기 버퍼 생성(동적)
	defaultBaseSize = PAT_Header.BaseSize;
	//char *buff = new char[baseSize];

	//PAGE BODY까지 fseek함수에 사용//
	toPage = sizeof(GDF_HDL)+sizeof(LINK_HDL)+(sizeof(LINK_N_RECORD)*LINK_Header.GmtCnt)
				+sizeof(SPATIAL_HDL)+sizeof(PAT_HEADER)+(sizeof(PAT)*PAT_Header.AllocNum);
	PAGE_OFFSET PAGE_offset;
	PAGE_HEADER PAGE_Header;

	tmpSize = PAT_Header.AllocNum-PAT_Header.FreeNum;
	for(i=0;i<PAT_Header.AllocNum;i++)								//PAGE 검색
	{
		if(Pat[i].Size>0)
		{
			baseSize = defaultBaseSize * Pat[i].Size;
			char *buff = new char[baseSize];	//Pat의 size만큼 버퍼 생성
			fseek(fp,toPage,SEEK_SET);
			fread(buff,baseSize,1,fp);

			memcpy(&PAGE_Header,buff,sizeof(PAGE_HEADER));
			for(int j=1;j<=PAGE_Header.TotalNum;j++)		//해당 PAGE의 옵셋검색 (1번부터 순차검색)
			{
				memcpy(&PAGE_offset,buff+baseSize-(sizeof(PAGE_OFFSET)*j),sizeof(PAGE_OFFSET));
				if(PAGE_offset.Size>0)						//비어 있는지 검색
				{
					TypeReader(buff,&PAGE_offset);
				}
			}
			toPage = toPage + baseSize;
			buff = NULL;
			delete [] buff;
		}
	}
	fclose(fp);

}

void CLayer::TypeReader(char* buff,PAGE_OFFSET* offset)
{
	
	short nType;
	memcpy(&nType,buff+offset->FreeOffset,2);
	
	// R-Tree  //
	OpsFloatExtent ext;
	
	void* pTmp; 

	switch(nType)
	{
		case 33:	//GMT_TYPE_ANNO_POINT
			GMT_ANNO_POINT tmpAnno;
			memcpy(&tmpAnno,buff+offset->FreeOffset,sizeof(GMT_ANNO_POINT));
			GMT_ANNO_POINT *pAnno;
			if(tmpAnno.AnnoteSize>0)
			{
				pAnno = (GMT_ANNO_POINT*)new char[sizeof(GMT_ANNO_POINT)+(sizeof(char)*tmpAnno.AnnoteSize)];
				memcpy(pAnno,buff+offset->FreeOffset,sizeof(GMT_ANNO_POINT)+(sizeof(char)*tmpAnno.AnnoteSize));
				pAnno->Annote = (char*)((char*)pAnno+sizeof(GMT_ANNO_POINT));
			}
			pTmp = (void*)pAnno;
			
			//  LIST에 저장
			//DataRecord.AddTail(pTmp);
			
			//  R-Tree에 저장
			ext.xMin = pAnno->MBR.MinX;
			ext.yMin = pAnno->MBR.MinY;
			ext.xMax = pAnno->MBR.MaxX;
			ext.yMax = pAnno->MBR.MaxY;
			m_RTree.Insert(pTmp,&ext);

			break;
		case 34:
			
			GMT_SYM_POINT tmpSym;
			memcpy(&tmpSym,buff+offset->FreeOffset,sizeof(GMT_SYM_POINT));
			GMT_SYM_POINT *pSym;
			if(tmpSym.Type!=NULL)
			{
				pSym = new GMT_SYM_POINT;
				memcpy(pSym,buff+offset->FreeOffset,sizeof(GMT_SYM_POINT));
			}
			pTmp = (void*)pSym;
			//  LIST저장 
			//DataRecord.AddTail(pTmp);
			
			//  R-Tree에 저장
			ext.xMin = pSym->MBR.MinX;
			ext.yMin = pSym->MBR.MinY;
			ext.xMax = pSym->MBR.MaxX;
			ext.yMax = pSym->MBR.MaxY;
			m_RTree.Insert(pTmp,&ext);

			break;
		case 49:	//GMT_TYPE_PURECHAIN
			GMT_PURECHAIN tmpChain;
			memcpy(&tmpChain,buff+offset->FreeOffset,sizeof(GMT_PURECHAIN));

			GMT_PURECHAIN *pChain;
			if(tmpChain.Attr!=NULL){
				pChain = (GMT_PURECHAIN*)new char[sizeof(GMT_PURECHAIN)+(sizeof(COORD2)*tmpChain.VtxCnt)+sizeof(LINE_ATTR)];
				memcpy(pChain,buff+offset->FreeOffset,sizeof(GMT_PURECHAIN)+(sizeof(COORD2)*tmpChain.VtxCnt)+sizeof(LINE_ATTR));
				pChain->Attr = (LINE_ATTR*)((char*)pChain+sizeof(GMT_PURECHAIN)+(sizeof(COORD2)*tmpChain.VtxCnt));
			}
			else{
				pChain = (GMT_PURECHAIN*)new char[sizeof(GMT_PURECHAIN)+(sizeof(COORD2)*tmpChain.VtxCnt)];
				memcpy(pChain,buff+offset->FreeOffset,sizeof(GMT_PURECHAIN)+(sizeof(COORD2)*tmpChain.VtxCnt));
				pChain->Attr=NULL;
			}
			pChain->VtxList = (COORD2*)((char*)pChain+sizeof(GMT_PURECHAIN));
			pTmp = (void*)pChain;
			//  LIST저장
			//DataRecord.AddTail(pTmp);
			
			//  R-Tree에 저장
			ext.xMin = pChain->MBR.MinX;
			ext.yMin = pChain->MBR.MinY;
			ext.xMax = pChain->MBR.MaxX;
			ext.yMax = pChain->MBR.MaxY;
			m_RTree.Insert(pTmp,&ext);

			break;
		case 65:
			GMT_PUREPOLYGON tmpPolygon;
			memcpy(&tmpPolygon,buff+offset->FreeOffset,sizeof(GMT_PUREPOLYGON));
			
			GMT_PUREPOLYGON *pPolygon;
			
			if(tmpPolygon.Attr!=NULL){
				pPolygon = (GMT_PUREPOLYGON*)new char[sizeof(GMT_PUREPOLYGON)+(sizeof(COORD2)*tmpPolygon.VtxCnt)+sizeof(POLY_ATTR)];
				memcpy(pPolygon,buff+offset->FreeOffset,sizeof(GMT_PUREPOLYGON)+(sizeof(COORD2)*tmpPolygon.VtxCnt)+sizeof(POLY_ATTR));
				pPolygon->Attr = (POLY_ATTR*)((char*)pPolygon+sizeof(GMT_PUREPOLYGON)+(sizeof(COORD2)*tmpPolygon.VtxCnt));
			}
			else{
				pPolygon = (GMT_PUREPOLYGON*)new char[sizeof(GMT_PUREPOLYGON)+(sizeof(COORD2)*tmpPolygon.VtxCnt)];
				memcpy(pPolygon,buff+offset->FreeOffset,sizeof(GMT_PUREPOLYGON)+(sizeof(COORD2)*tmpPolygon.VtxCnt));
				pPolygon->Attr=NULL;
			}
			pPolygon->VtxList = (COORD2*)((char*)pPolygon+sizeof(GMT_PUREPOLYGON));
			pTmp = (void*)pPolygon;

			//  LIST저장
			//DataRecord.AddTail(pTmp);

			//  R-Tree에 저장
			ext.xMin = pPolygon->MBR.MinX;
			ext.yMin = pPolygon->MBR.MinY;
			ext.xMax = pPolygon->MBR.MaxX;
			ext.yMax = pPolygon->MBR.MaxY;

			m_RTree.Insert(pTmp,&ext);

			break;
	}
}



void CLayer::Draw(CDC* dc,Scaler& sc)
{
	short dType;
	//POSITION pos;
	//pos = DataRecord.GetHeadPosition();
	void* pTmp;
	int i;
	double tmpx,tmpy;

	GMT_PUREPOLYGON tmpPoly;
	GMT_PURECHAIN tmpChain;
	GMT_ANNO_POINT tmpAnno;
	GMT_SYM_POINT tmpSym;

	//  R-Tree 공간 검색 
	OpsFloatExtent odExt;
	odExt.xMin = sc.mxMin;
	odExt.yMin = sc.myMin;
	odExt.xMax = sc.mxMax;
	odExt.yMax = sc.myMax;



	m_RTree.BeginSearch(&odExt);


	CPoint*	tmpCPoint;

	//while(pos!=NULL)
	while(1)
	{
		//pTmp = DataRecord.GetNext(pos);
		pTmp = (void*)m_RTree.GetNextItem();
		
		if(pTmp == NULL)
			return;

		memcpy(&dType,pTmp,2);

		switch(dType)
		{
			case 33:			//	GMT_TYPE_ANNO_POINT 	  0x21 // 33
				memcpy(&tmpAnno,pTmp,sizeof(GMT_ANNO_POINT));
				if(tmpAnno.AnnoteSize>0)
				{
					CFont font,*pFont;
					int a,b;
					CPoint point;
					sc.TextSize(tmpAnno.Size,tmpAnno.Width,&a,&b);
					
					if(a<1)				//화면 축소시 글자 안보이게
					{
						break;
					}

					font.CreateFont(a,0,(int)(tmpAnno.Angle*10),0,0,false,false,0,DEFAULT_CHARSET
									,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,tmpAnno.FaceName);
					pFont = dc->SelectObject(&font);
					sc.WorldToDevice(tmpAnno.Pos.X,tmpAnno.Pos.Y,&tmpx,&tmpy);
					point.x = (long)tmpx;
					point.y = (long)tmpy;
					dc->TextOut(point.x,point.y,tmpAnno.Annote);
					dc->SetTextColor(SeletColor(tmpAnno.ColorIdx));
					dc->SetBkMode(TRANSPARENT);						//텍스트 투명
					dc->SelectObject(pFont);
				}

				break;
			case 34:
				
				memcpy(&tmpSym,pTmp,sizeof(GMT_ANNO_POINT));
				if(tmpSym.Type!=NULL)
				
				{
					CDC MemDC;
					MemDC.CreateCompatibleDC(dc);
					
					CBitmap bit;
					bit.LoadBitmap(SelectSym(tmpSym.Idx));		
					CBitmap *pOldBitmap = (CBitmap *)MemDC.SelectObject(&bit);
			
					sc.WorldToDevice(tmpSym.Pos.X,tmpSym.Pos.Y,&tmpx,&tmpy);
					if(tmpSym.Tag==1)
					{
						dc->BitBlt((int)tmpx,(int)tmpy,(int)tmpSym.Size,(int)tmpSym.Size,&MemDC,0,0,SRCCOPY);
						MemDC.SelectObject(pOldBitmap);
					}
					else
					{
						int a,b;
						sc.TextSize(tmpSym.Size*10,tmpSym.Size*10,&a,&b);
						dc->StretchBlt((int)tmpx,(int)tmpy,a,a,&MemDC,0,0,(int)tmpSym.Size,(int)tmpSym.Size,SRCCOPY);
						MemDC.SelectObject(pOldBitmap);
					}
					
					//memdc.DeleteDC();
					//bitmap.DeleteObject();
				}

				break;
			case 49:			//	GMT_TYPE_PURECHAIN		  0x31 // 49
				memcpy(&tmpChain,pTmp,sizeof(GMT_PURECHAIN));
				
				tmpCPoint = new CPoint[tmpChain.VtxCnt];
				
				for(i=0;i<tmpChain.VtxCnt;i++)
				{
					sc.WorldToDevice(tmpChain.VtxList[i].X,tmpChain.VtxList[i].Y,&tmpx,&tmpy);
					tmpCPoint[i].x = (long)tmpx;
					tmpCPoint[i].y = (long)tmpy;
				}
				
				if(tmpChain.Attr!=NULL){
					CPen pen;
					pen.CreatePen(tmpChain.Attr->Style,tmpChain.Attr->Width,SeletColor(tmpChain.Attr->ColorIdx));
					CPen *pPen = dc->SelectObject(&pen);
					dc->Polyline(tmpCPoint,tmpChain.VtxCnt);
					dc->SelectObject(pPen);
				}
				else{				//SPA_HEADER에 있는 기본 속성 따름
					CPen pen;
					pen.CreatePen(SPA_Header.LineAttr.Style,SPA_Header.LineAttr.Width,SeletColor(SPA_Header.LineAttr.ColorIdx));	
					CPen *pPen = dc->SelectObject(&pen);
					dc->Polyline(tmpCPoint,tmpChain.VtxCnt);
					dc->SelectObject(pPen);
				}
				
				delete [] tmpCPoint;
				break;
			
			case 65:			//	GMT_TYPE_PUREPOLYGON	  0x41 // 65
				
				CBrush* pBrOld = (CBrush*)dc->SelectStockObject(NULL_BRUSH);
				memcpy(&tmpPoly,pTmp,sizeof(GMT_PUREPOLYGON));
				
				//겉 테두리 안그려짐
				if(tmpPoly.MBR.MinX==tmpPoly.VtxList->X&&tmpPoly.MBR.MinY==tmpPoly.VtxList->Y)	
				{
					break;
				}
				tmpCPoint = new CPoint[tmpPoly.VtxCnt];

				//TEST
				BOUND tmptmp;
				tmptmp.MinX =  SPA_Header.LeftUpPos.X;
				tmptmp.MinY =  SPA_Header.LeftUpPos.Y;
				tmptmp.MaxX =  SPA_Header.RightDownPos.X;
				tmptmp.MaxY =  SPA_Header.RightDownPos.Y;

				if(tmptmp.MinX > tmpPoly.MBR.MinX)
					tmpcnt2++;
				if(tmptmp.MinY < tmpPoly.MBR.MinY)
					tmpcnt2++;
				if(tmptmp.MaxX < tmpPoly.MBR.MaxX)
					tmpcnt2++;
				if(tmptmp.MaxY > tmpPoly.MBR.MaxY)
					tmpcnt2++;
							
				//tmpcnt++;
								
				//TEST END

				for(i=0;i<tmpPoly.VtxCnt;i++)
				{
					sc.WorldToDevice(tmpPoly.VtxList[i].X,tmpPoly.VtxList[i].Y,&tmpx,&tmpy);
					tmpCPoint[i].x = (long)tmpx;
					tmpCPoint[i].y = (long)tmpy;
				}

				if(tmpPoly.Attr!=NULL)					//PAGE에 속성값을 따를떄
				{
					CPen pen;
					pen.CreatePen(tmpPoly.Attr->Line.Style,tmpPoly.Attr->Line.Width,SeletColor(tmpPoly.Attr->Line.ColorIdx));
					CPen *pPen = dc->SelectObject(&pen);
					dc->Polygon(tmpCPoint,tmpPoly.VtxCnt);
					dc->SelectObject(&pen);
					dc->SelectObject(pBrOld);
				}
				else									//SPA_HEADER의 기본속성값을 따를떄
				{
					CPen pen;
					pen.CreatePen(SPA_Header.LineAttr.Style,SPA_Header.LineAttr.Width,SeletColor(SPA_Header.LineAttr.ColorIdx));
					CPen *pPen = dc->SelectObject(&pen);
					dc->Polygon(tmpCPoint,tmpPoly.VtxCnt);
					dc->SelectObject(&pen);
					dc->SelectObject(pBrOld);
				}
				
				delete [] tmpCPoint;
				break;
			
		}
	}
}


COLORREF CLayer::SeletColor(int cNum)
{
	//개노가다 ㅠㅠ
	switch(cNum)
	{
		case 1:
			return RGB(0,0,0);	
		case 2:
			return RGB(168,168,168);	
		case 8:		//강
			return RGB(219,112,219);
		case 6:
			return RGB(50,205,50);
		case 10:
			return RGB(0,0,255);
		case 38:
			return RGB(0,255,0);
		case 54:
			return RGB(50,205,50);
		case 86:
			return RGB(105,219,147);
		case 118:
			return RGB(95,159,159);
		case 134:
			return RGB(207,181,59);
		case 150:
			return RGB(217,135,25);
		case 166:
			return RGB(219,112,147);
		case 182:
			return RGB(205,127,50);
		case 198:
			return RGB(228,120,51);
		case 214:
			return RGB(228,120,51);
		case 230:
			return RGB(255,28,174);
		case 236:
			return RGB(255,36,0);
		case 240:
			return RGB(255,0,0);
		case 246:
			return RGB(255,0,0);
		default:
			return RGB(0,0,0);
	}
}

int CLayer::SelectSym(int num)
{
	switch(num)
	{
		case 0:
			return IDB_BITMAP3;
			
		case 1:
			return IDB_BITMAP4;
			
		case 2:
			return IDB_BITMAP5;
			
		case 21:
			return IDB_BITMAP6;
		
		case 28:
			return IDB_BITMAP7;
			
		default:
			return IDB_BITMAP2;
	}
}