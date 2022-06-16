#ifndef _CModel
#define _CModel

#include "standard.h"

#include "CGlobalObjects.h"
#include "CMaterial.h"
#include "CString.h"
#include "CVertexBuffer.h"

class CModel
{
public:

	enum
	{
		MATERIAL_LENGTH = 32
	};

	CModel();

	CModel(CGlobalObjects* g, FILE* file, ComPtr<ID3D12GraphicsCommandList> cl, CLinkList* vb, CMaterial* material);

	~CModel();

private:

	CGlobalObjects* globalObjects;

	size_t br;

	SHORT len;

	UINT vcount;

	char materialName[MATERIAL_LENGTH];

	char map_Ka[MATERIAL_LENGTH];
	char map_Kd[MATERIAL_LENGTH];
	char map_Ks[MATERIAL_LENGTH];
	char map_Ns[MATERIAL_LENGTH];
	char map_d[MATERIAL_LENGTH];
	char map_Kb[MATERIAL_LENGTH];
	char refl[MATERIAL_LENGTH];
};
#endif