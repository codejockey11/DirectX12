#ifndef _CTERRAIN
#define _CTERRAIN

#include "standard.h"

#include "CBrush.h"
#include "CColor.h"
#include "CEffect.h"
#include "CGlobalObjects.h"
#include "CHeapArray.h"
#include "CLinkList.h"
#include "CPointLight.h"
#include "CTerrainClip.h"
#include "CTerrainTile.h"
#include "CVertexBuffer.h"

class CTerrain
{
public:

	CEffect* effect;

	CHeapArray* clips;

	CLinkList* brushes;

	UINT width;
	
	UINT height;
	
	UINT primSize;

	CTerrain();

	CTerrain(CGlobalObjects* g, const CHAR* filename);

	~CTerrain();

	VOID Draw();

private:

	HRESULT hr;

	CGlobalObjects* globalObjects;

	//CTerrainTile* vertices;

	CVertexNT* vertices;
	
	CTerrainTile* tile;
	
	CVertexBuffer* vertexBuffer;

	CIndexBuffer* indexBuffer;

	INT size;

	FILE* fp;
	
	errno_t err;
	
	size_t br;
};
#endif