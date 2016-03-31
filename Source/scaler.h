// Scaler.h: interface for the Scaler class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(SCALER_H)
#define SCALER_H
#include "GDFHeader.h"
class Scaler  
{
	
public:
	Scaler();
	void DeviceToWorld(double sx, double sy, double *mx, double *my);
	void WorldToDevice(double mx, double my, double *sx, double *sy);
	void TextSize(double mx, double my, int *x, int *y);

	void test1(SPATIAL_HDL *header ,double mx, double my, double *sx, double *sy);

	//////////////////////////////
	// 지도그리기 관련 멤버 
	double mox , moy ;					// 지도 기준점
	double mxMin, myMin, mxMax, myMax;	// 지도 Extent
	double sox , soy ;					// 화면 기준점
	double sxMin, syMin, sxMax, syMax;	// 화면 Extent
	

	// 캔버스 1단위당 실제길이 값 - 거리비(실제길이/캔버스길이)
	double nowRatio;		
};

#endif

