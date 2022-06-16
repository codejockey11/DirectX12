#include "CLight.h"


CLight::CLight()
{
	memset(this, 0x00, sizeof(CLight));

}


CLight::CLight(CVideoDevice* d, int n, D3DLIGHTTYPE Type, D3DCOLORVALUE Diffuse, D3DCOLORVALUE Specular, D3DCOLORVALUE Ambient,
	D3DVECTOR Position, D3DVECTOR Direction, FLOAT Range, FLOAT Falloff, FLOAT Attenuation0,
	FLOAT Attenuation1, FLOAT Attenuation2, FLOAT Theta, FLOAT Phi)
{
	memset(this, 0x00, sizeof(CLight));

	videoDevice			= d;

	number				= n;

	light.Type			= Type;
	light.Diffuse		= Diffuse;
	light.Specular		= Specular;
	light.Ambient		= Ambient;
	light.Position		= Position;
	light.Direction		= Direction;
	light.Range			= Range;
	light.Falloff		= Falloff;
	light.Attenuation0	= Attenuation0;
	light.Attenuation1	= Attenuation1;
	light.Attenuation2	= Attenuation2;
	light.Theta			= Theta;
	light.Phi			= Phi;

}


CLight::~CLight()
{
}
