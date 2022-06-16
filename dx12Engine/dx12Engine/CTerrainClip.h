#ifndef _CTerrainClip
#define _CTerrainClip

#include "standard.h"

#include "CVertex.h"

class CTerrainClip
{
public:

	CVertex	a;
	CVertex	b;
	CVertex	c;
	CVertex	d;

	CVertex	n1;
	CVertex	n2;

	WORD sp1;
	WORD sp2;

	CTerrainClip();

	~CTerrainClip();
};
#endif