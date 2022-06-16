#include "CVertex.h"

CVertex::CVertex()
{
	memset(this, 0x00, sizeof(CVertex));
}

CVertex::CVertex(float p1, float p2, float p3)
{
	memset(this, 0x00, sizeof(CVertex));

	p.x = p1;
	p.y = p2;
	p.z = p3;
}

CVertex::~CVertex()
{
}

void CVertex::operator += (const CVertex& v)
{
	p.x += v.p.x;
	p.y += v.p.y;
	p.z += v.p.z;
};

void CVertex::operator -= (const CVertex& v)
{
	p.x -= v.p.x;
	p.y -= v.p.y;
	p.z -= v.p.z;
};

void CVertex::operator *= (float v)
{
	p.x *= v;
	p.y *= v;
	p.z *= v;
};

void CVertex::operator /= (float v)
{
	p.x /= v;
	p.y /= v;
	p.z /= v;
};

CVertex CVertex::operator + (const CVertex& v)
{
	CVertex t;

	t.p.x = p.x + v.p.x;
	t.p.y = p.y + v.p.y;
	t.p.z = p.z + v.p.z;

	return t;
};

CVertex CVertex::operator - (const CVertex& v)
{
	CVertex t;

	t.p.x = p.x - v.p.x;
	t.p.y = p.y - v.p.y;
	t.p.z = p.z - v.p.z;

	return t;
};

CVertex CVertex::operator * (float v)
{
	CVertex t;

	t.p.x = p.x * v;
	t.p.y = p.y * v;
	t.p.z = p.z * v;

	return t;
};

CVertex CVertex::operator / (float v)
{
	CVertex t;

	t.p.x = p.x / v;
	t.p.y = p.y / v;
	t.p.z = p.z / v;

	return t;
};

bool CVertex::operator == (const CVertex v)
{
	return (p.x == v.p.x) && (p.y == v.p.y) && (p.z == v.p.z);
};

bool CVertex::operator != (const CVertex v)
{
	return !((p.x == v.p.x) && (p.y == v.p.y) && (p.z == v.p.z));
};

float CVertex::Length()
{
	return (float)sqrtf((p.x * p.x) + (p.y * p.y) + (p.z * p.z));
}

void CVertex::Normalize()
{
	float d = CVertex::Length();

	p.x = p.x / d;
	p.y = p.y / d;
	p.z = p.z / d;
}

void CVertex::RadiusNormalize()
{
	float d = (float)fabs((DOUBLE)p.x) + (float)fabs((DOUBLE)p.y) + (float)fabs((DOUBLE)p.z);

	p.x = p.x / d;
	p.y = p.y / d;
	p.z = p.z / d;
}

float CVertex::Dot(CVertex v)
{
	return (p.x * v.p.x) + (p.y * v.p.y) + (p.z * v.p.z);
};

CVertex CVertex::Cross(CVertex v)
{
	CVertex t;

	t.p.x = (p.y * v.p.z) - (p.z * v.p.y);
	t.p.y = (p.z * v.p.x) - (p.x * v.p.z);
	t.p.z = (p.x * v.p.y) - (p.y * v.p.x);

	return t;
}

CVertex CVertex::CalculateNormal(CVertex* a, CVertex* b, CVertex* c)
{
	CVertex edge1 = *c - *b;
	CVertex edge2 = *c - *a;

	edge1.Normalize();

	edge2.Normalize();

	CVertex N1 = edge1.Cross(edge2);

	N1.Normalize();

	return N1;
}

CVertexRGBA::CVertexRGBA()
{
	memset(this, 0x00, sizeof(CVertexRGBA));
}

CVertexRGBA::~CVertexRGBA()
{
}

CVertexT::CVertexT()
{
	memset(this, 0x00, sizeof(CVertexT));
}

CVertexT::~CVertexT()
{
}

CVertexNT::CVertexNT()
{
	memset(this, 0x00, sizeof(CVertexNT));
}

CVertexNT::~CVertexNT()
{
}

CVertex2d::CVertex2d()
{
	memset(this, 0x00, sizeof(CVertex2d));
}

CVertex2d::~CVertex2d()
{
}

CVertex2::CVertex2()
{
	memset(this, 0x00, sizeof(CVertex2));
}

CVertex2::CVertex2(float p1, float p2)
{
	p.x = p1;
	p.y = p2;
}

CVertex2::~CVertex2()
{
}
