#include "CMaterial.h"

CMaterial::CMaterial()
{
	memset(this, 0x00, sizeof(CMaterial));
}

CMaterial::CMaterial(const char* n)
{
	memset(this, 0x00, sizeof(CMaterial));

	name = new CString(n);
}

CMaterial::~CMaterial()
{
	delete name;
}

void CMaterial::Copy(CMaterial* m)
{
	if (m == nullptr)
	{
		return;
	}

	name = new CString(m->name->GetText());

	ambient = m->ambient;
	diffuse = m->diffuse;
	specular = m->specular;
	emissive = m->emissive;

	specularExponent = m->specularExponent;

	opacity = m->opacity;

	illum = m->illum;

	opticalDensity = m->opticalDensity;

	map_Ka = m->map_Ka;
	map_Kd = m->map_Kd;
	map_Ks = m->map_Ks;
	map_Ns = m->map_Ns;

	map_d = m->map_d;
	
	map_Kb = m->map_Kb;

	refl = m->refl;
}
