#ifndef _CLIGHT
#define _CLIGHT

#include "standard.h"

#include "CVideoDevice.h"

class CLight
{
	public:
		
		D3DLIGHT9	light;

		int			number;

		CLight();
		CLight::CLight(CVideoDevice* d, int n, D3DLIGHTTYPE Type, D3DCOLORVALUE Diffuse, D3DCOLORVALUE Specular, D3DCOLORVALUE Ambient,
						D3DVECTOR Position, D3DVECTOR Direction, FLOAT Range, FLOAT Falloff, FLOAT Attenuation0,
						FLOAT Attenuation1, FLOAT Attenuation2, FLOAT Theta, FLOAT Phi);

		~CLight();

	private:

		CVideoDevice*	videoDevice;

};

#endif