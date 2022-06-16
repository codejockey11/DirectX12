#include "CBoundBox.h"

CBoundBox::CBoundBox()
{
	memset(this, 0x00, sizeof(CBoundBox));
}

CBoundBox::CBoundBox(CVertex* mn, CVertex* mx)
{
	memset(this, 0x00, sizeof(CBoundBox));

	min.p.x = mn->p.x;
	min.p.y = mn->p.y;
	min.p.z = mn->p.z;

	max.p.x = mx->p.x;
	max.p.y = mx->p.y;
	max.p.z = mx->p.z;
}

CBoundBox::CBoundBox(CBoundBox* b)
{
	memset(this, 0x00, sizeof(CBoundBox));

	min.p.x = b->min.p.x;
	min.p.y = b->min.p.y;
	min.p.z = b->min.p.z;

	max.p.x = b->max.p.x;
	max.p.y = b->max.p.y;
	max.p.z = b->max.p.z;
}

CBoundBox::~CBoundBox()
{
}

void CBoundBox::Copy(CBoundBox* b)
{
	min.p.x = b->min.p.x;
	min.p.y = b->min.p.y;
	min.p.z = b->min.p.z;

	max.p.x = b->max.p.x;
	max.p.y = b->max.p.y;
	max.p.z = b->max.p.z;
}

void CBoundBox::Update(CVertex* mn, CVertex* mx)
{
	min.p.x = mn->p.x;
	min.p.y = mn->p.y;
	min.p.z = mn->p.z;

	max.p.x = mx->p.x;
	max.p.y = mx->p.y;
	max.p.z = mx->p.z;
}

BYTE CBoundBox::CheckPointInBox(CVertex* p)
{
	if ((p->p.x >= min.p.x) && (p->p.x <= max.p.x))
	{
		if ((p->p.y >= min.p.y) && (p->p.y <= max.p.y))
		{
			if ((p->p.z >= min.p.z) && (p->p.z <= max.p.z))
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}