#include "CModel.h"

CModel::CModel()
{
	memset(this, 0x00, sizeof(CModel));
}

CModel::CModel(CGlobalObjects* g, FILE* file, ComPtr<ID3D12GraphicsCommandList> cl, CLinkList* vb, CMaterial* material)
{
	memset(this, 0x00, sizeof(CModel));

	globalObjects = g;

	fread(&len, sizeof(SHORT), 1, file);

	fread(materialName, sizeof(char), len, file);

	material->name = new CString(materialName);

	fread(&material->ambient, sizeof(float), 3, file);
	fread(&material->diffuse, sizeof(float), 3, file);
	fread(&material->specular, sizeof(float), 3, file);
	fread(&material->emissive, sizeof(float), 3, file);

	fread(&material->specularExponent, sizeof(float), 1, file);

	fread(&material->opacity, sizeof(float), 1, file);

	fread(&material->illum, sizeof(UINT), 1, file);

	fread(&material->opticalDensity, sizeof(float), 1, file);

	fread(&len, sizeof(SHORT), 1, file);
	if (len > 0)
	{
		fread(map_Ka, sizeof(char), len, file);

		material->map_Ka = globalObjects->textureMgr->MakeTexture(map_Ka);
	}

	fread(&len, sizeof(SHORT), 1, file);
	if (len > 0)
	{
		fread(map_Kd, sizeof(char), len, file);

		material->map_Kd = globalObjects->textureMgr->MakeTexture(map_Kd);
	}

	fread(&len, sizeof(SHORT), 1, file);
	if (len > 0)
	{
		fread(map_Ks, sizeof(char), len, file);

		material->map_Ks = globalObjects->textureMgr->MakeTexture(map_Ks);
	}

	fread(&len, sizeof(SHORT), 1, file);
	if (len > 0)
	{
		fread(map_Ns, sizeof(char), len, file);

		material->map_Ns = globalObjects->textureMgr->MakeTexture(map_Ns);
	}

	fread(&len, sizeof(SHORT), 1, file);
	if (len > 0)
	{
		fread(map_d, sizeof(char), len, file);

		material->map_d = globalObjects->textureMgr->MakeTexture(map_d);
	}

	fread(&len, sizeof(SHORT), 1, file);
	if (len > 0)
	{
		fread(map_Kb, sizeof(char), len, file);

		material->map_Kb = globalObjects->textureMgr->MakeTexture(map_Kb);
	}

	fread(&len, sizeof(SHORT), 1, file);
	if (len > 0)
	{
		fread(refl, sizeof(char), len, file);

		material->refl = globalObjects->textureMgr->MakeTexture(refl);
	}

	fread(&vcount, sizeof(UINT), 1, file);

	BYTE* v = new BYTE[vcount * sizeof(CVertexNT)];

	fread(v, sizeof(BYTE), vcount * sizeof(CVertexNT), file);

	CVertexBuffer* b = new CVertexBuffer(globalObjects, cl, CVertex::VT_VERTEXNT, vcount, v);

	vb->Add(b, material->name->GetText());

	delete[] v;
}

CModel::~CModel()
{
}
