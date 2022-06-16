#ifndef _CCOLLISION
#define _CCOLLISION

#include "standard.h"

#include "CVertex.h"

class CCollision
{
public:

	FLOAT l;

	FLOAT u;
	FLOAT v;

	CCollision();

	~CCollision();

	BOOL RayTriangleIntersect(CVertex* P, CVertex* A, CVertex* B, CVertex* C);

	BOOL IntersectPlane(CVertex* n, CVertex* p0, CVertex* l0, CVertex* d);

private:

	FLOAT denom;

	CVertex p0l0;

	CVertex p;
	CVertex pp;

	FLOAT d2;
	FLOAT radius2;

	CVertex v0v1;
	CVertex v0v2;

	CVertex v0;
	CVertex v1;
	CVertex v2;
};
#endif