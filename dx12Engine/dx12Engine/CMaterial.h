#ifndef _CMaterial
#define _CMaterial

#include "standard.h"

#include "CString.h"
#include "CTexture.h"

class CMaterial
{
public:

	enum
	{
		MATERIAL_COUNT = 8,
		MATERIAL_LENGTH = 32
	};

	CString* name;

	XMFLOAT3 ambient;           // Ka
	XMFLOAT3 diffuse;           // Kd
	XMFLOAT3 specular;          // Ks
	XMFLOAT3 emissive;          // Ke
	
	float    specularExponent;  // Ns
	
	float    opacity;           // d or inverted: Tr = 1 - d
	
	UINT     illum;

	float	 opticalDensity;    // Specifies the optical density for the surface.This is also known as index of refraction.

	CTexture* map_Ka;	// the ambient texture map
	CTexture* map_Kd;	// the diffuse texture map(most of the time, it will be the same as the ambient texture map
	CTexture* map_Ks;   // specular color texture map
	CTexture* map_Ns;   // specular highlight component
	CTexture* map_d;    // the alpha texture map
	CTexture* map_Kb;   // some implementations use 'map_bump' instead of 'bump' below

	CTexture* refl;     // spherical reflection map

	CMaterial();

	CMaterial(const char* n);

	~CMaterial();

	void Copy(CMaterial* m);
};
#endif