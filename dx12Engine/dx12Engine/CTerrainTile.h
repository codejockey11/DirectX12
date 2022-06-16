#ifndef _CTerrainTile
#define _CTerrainTile

#include "standard.h"

#include "CVertex.h"

class CTerrainTile
{
public:

	CVertexNT	a;
	CVertexNT	b;
	CVertexNT	c;

	CVertexNT	d;
	CVertexNT	e;
	CVertexNT	f;

	CTerrainTile();

	~CTerrainTile();
};
#endif