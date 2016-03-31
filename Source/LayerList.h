#ifndef __LAYERLIST_H__
#define __LAYERLIST_H__

#include "GDFHeader.h"
#include "Scaler.h"
#include "Layer.h"


class CLayerList
{
	public:
		CLayerList();
		~CLayerList();
		int	 AddLayer(char* pName);
		bool DelLayer(int cNum);
		void DrawLayer(CDC* dc,Scaler &sc);

		CLayer		 *m_Layer[MAX_LAYER_LIST];
		int			 m_Cnt;

		
};

#endif