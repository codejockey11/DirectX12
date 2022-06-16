#include "CCollision.h"

CCollision::CCollision()
{
	memset(this, 0x00, sizeof(CCollision));
}

CCollision::~CCollision()
{
}

// n = plane normal
// p0 = plane origin
// l0 = ray origin
// d = ray direction
// l = length from ray start to intersection
BOOL CCollision::IntersectPlane(CVertex* n, CVertex* p0, CVertex* l0, CVertex* d)
{
	// assuming vectors n,d are normalized
	denom = n->Dot(*d);

	// determine if ray parallel or pointing away from the plane
	if (denom > -0.0F)
	{
		return FALSE;
	}

	// p0 - l0;
	p0l0 = *p0 - *l0;

	// to keep translation don't normalize p0l0
	l = p0l0.Dot(*n) / denom;

	// returning true when collision in front of starting point
	// no need to test return value unless it is important to the caller
	if (l >= 0.0F)
	{
		return TRUE;
	}

	return FALSE;
}

// blackpawn.com/texts/pointinpoly/default.html
// realtimecollisiondetection.net
// en.wikipedia.org/wiki/Barycentric_coordinate_system
// mathworld.wolfram.com/BarycentricCoordinates.html
BOOL CCollision::RayTriangleIntersect(CVertex* P, CVertex* A, CVertex* B, CVertex* C)
{
	// Compute vectors
	v0 = *C - *A;
	v1 = *B - *A;
	v2 = *P - *A;

	// Compute dot products
	FLOAT dot00 = v0.Dot(v0);
	FLOAT dot01 = v0.Dot(v1);
	FLOAT dot02 = v0.Dot(v2);
	FLOAT dot11 = v1.Dot(v1);
	FLOAT dot12 = v1.Dot(v2);

	// Compute barycentric coordinates
	FLOAT invDenom = 1 / ((dot00 * dot11) - (dot01 * dot01));

	u = ((dot11 * dot02) - (dot01 * dot12)) * invDenom;
	v = ((dot00 * dot12) - (dot01 * dot02)) * invDenom;

	// Check if point is in triangle
	if ((u >= 0) && (v >= 0) && ((u + v) < 1.0F))
	{
		return TRUE;
	}

	return FALSE;
}