// Scaler.cpp: implementation of the Scaler class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Scaler.h"
#include <math.h>


Scaler::Scaler()
{
}

void Scaler::WorldToDevice(double mx, double my, double *sx, double *sy)
{
	double m_lfnowXRatio = (mxMax-mxMin) / (sxMax-sxMin) ;  // X√‡√¥
	double m_lfnowYRatio = (myMax-myMin) / (syMax-syMin) ;  // Y√‡√¥
	
	// ¡æ»æ∫Ò ∫∏¡§
	if(m_lfnowXRatio > m_lfnowYRatio)
		nowRatio  =  m_lfnowXRatio;
	else
		nowRatio  =  m_lfnowYRatio;
	
	// ¡ﬂΩ…¡°¿ª ±‚¡ÿ¡°¿∏∑Œ ¿‚¿Ω
	mox = (mxMax+mxMin) /2;
	moy = (myMax+myMin) /2;
	sox = (sxMax+sxMin) /2;
	soy = (syMax+syMin) /2;
	
	*sx=(mx-mox)/nowRatio+sox;
	*sy=(moy-my)/nowRatio+soy;
}

void Scaler::DeviceToWorld(double sx, double sy, double *mx, double *my)
{
	double m_lfnowXRatio = (mxMax-mxMin) / (sxMax-sxMin) ; // X√‡√¥
	double m_lfnowYRatio = (myMax-myMin) / (syMax-syMin) ; // Y√‡√¥
	
	// ¡æ»æ∫Ò ∫∏¡§
	if(m_lfnowXRatio > m_lfnowYRatio)
		nowRatio  =  m_lfnowXRatio;
	else
		nowRatio  =  m_lfnowYRatio;
	
	
	// ¡ﬂΩ…¡°¿ª ±‚¡ÿ¡°¿∏∑Œ ¿‚¿Ω
	mox = (mxMax+mxMin) /2;
	moy = (myMax+myMin) /2;
	sox = (sxMax+sxMin) /2;
	soy = (syMax+syMin) /2;
	
	
	*mx = mox + (sx - sox) * nowRatio;
	*my = moy - (sy - soy) *  nowRatio;
}

void Scaler::TextSize(double mx, double my, int *x, int *y)
{
	double m_XRatio = (mxMax-mxMin) / (sxMax-sxMin) ;  // X√‡√¥
	double m_YRatio = (myMax-myMin) / (syMax-syMin) ;  // Y√‡√¥
	
	if(mx<1)
	{
		nowRatio = nowRatio *1200;
		mx = mx *1200;
		*x = (int)(mx / nowRatio);
		*y = (int)(my / nowRatio);
		
	}
	else
	{
		*x = (int)(mx / nowRatio);
		*y = (int)(my / nowRatio);
	}

	if(*x>1000)
	{
		*x = (int)(*x * nowRatio);
	}
	
}


void Scaler::test1(SPATIAL_HDL *header ,double mx, double my, double *sx, double *sy)
{
	double m_XRatio = (mxMax-mxMin) / (header->RightDownPos.X-header->LeftUpPos.X);  // X√‡√¥
	double m_YRatio = (myMax-myMin) / (header->RightDownPos.Y-header->LeftUpPos.Y);  // Y√‡√¥
	
	*sx = mx * m_XRatio;
	*sy = my * m_YRatio;
}
