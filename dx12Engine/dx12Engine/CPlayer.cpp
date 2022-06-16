#include "CPlayer.h"

CPlayer::CPlayer()
{
	memset(this, 0x00, sizeof(CPlayer));
}

CPlayer::CPlayer(CGlobalObjects* g, CBrushMap* b, CTerrain* t, FLOAT r, const CHAR* n)
{
	memset(this, 0x00, sizeof(CPlayer));

	globalObjects = g;

	brushMap = b;
	terrain = t;

	rate = r;

	name = new CString(n);

	camera = new CCamera();
	collision = new CCollision();

	direction = camera->look;

	upVect = CVertex(0.0F, 1.0F, 0.0F);
	downVect = CVertex(0.0F, -1.0F, 0.0F);

	vertexBuffer = new CVertexBuffer(globalObjects->errorLog, globalObjects->videoDevice, CVertex::VT_VERTEX, 3, NULL);

#ifdef _DEBUG
	clipBlend = globalObjects->effectMgr->GetEffect("blend.fx");

	lineBlend = globalObjects->effectMgr->GetEffect("line.fx");

	normalLine = new CLine(globalObjects);

	for (UINT i = 0; i < 3; i++)
	{
		clipLine[i] = new CLine(globalObjects);
	}
#endif
}

CPlayer::~CPlayer()
{
	delete name;
	delete camera;
	delete collision;
	delete vertexBuffer;

#ifdef _DEBUG
	delete normalLine;

	for (UINT i = 0; i < 3; i++)
	{
		delete clipLine[i];
	}
#endif
}

VOID CPlayer::PlayerMove(CPacket* p)
{
	distance = rate * globalObjects->frametime->frametime;

	direction = camera->look;

	direction.Normalize();

	switch (p->type)
	{
	case CPacket::PT_FORWARD:
	{
		break;
	}

	case CPacket::PT_BACKWARD:
	{
		direction = camera->look * -1.0F;

		direction.Normalize();

		break;
	}

	case CPacket::PT_STRAFELEFT:
	{
		FLOAT yaw = -90.0F * PiDiv180;

		direction = direction.RotateAngleByAxis(yaw, CVertex(0.0F, 1.0F, 0.0F));

		direction.Normalize();

		break;
	}

	case CPacket::PT_STRAFERIGHT:
	{
		FLOAT yaw = 90.0F * PiDiv180;

		direction = direction.RotateAngleByAxis(yaw, CVertex(0.0F, 1.0F, 0.0F));

		direction.Normalize();

		break;
	}
	}

	if (brushMap)
	{
		for (UINT h = 0; h < brushMap->height; h++)
		{
			for (UINT w = 0; w < brushMap->width; w++)
			{
				CPlayer::BrushCollision((CBrush*)brushMap->brushes->GetElement(2, w, h));
			}
		}
	}

	if (terrain)
	{
		CPlayer::GroundCollision();
	
		if (terrain->brushes)
		{
			CLinkListNode* lln = terrain->brushes->list;
	
			while (lln->element)
			{
				CDBrush* br = (CDBrush*)lln->element;

				CPlayer::BrushCollision(br);
	
				lln = lln->next;
			}
		}
	}

	position += (direction * distance);

	camera->position = position;
}

BOOL CPlayer::BrushCollision(CBrush* brush)
{
	dVect = direction;

	for (UINT i = 0; i < 6; i++)
	{
		if ((i == CBrushSide::BBS_BOTTOM) || (brush->side[i]->clip == CBrushSide::SC_NOCLIP))
		{
			continue;
		}

		cVect = brush->side[i]->vertices[0].n * -1.0F;

		CVertex pn = brush->side[i]->vertices[0].n;

		CVertex A = brush->side[i]->vertices[0];
		CVertex B = brush->side[i]->vertices[1];
		CVertex C = brush->side[i]->vertices[2];
		CVertex D = brush->side[i]->vertices[3];

		/*
		A-B
		| |
		D-C
		*/
		if (i == CBrushSide::BBS_TOP)
		{
			collision->IntersectPlane(&pn, &A, &position, &cVect);

			if (collision->l <= 8.0F)
			{
				pop.X = position.X;
				pop.Y = position.Y - collision->l;
				pop.Z = position.Z;

				if (collision->RayTriangleIntersect(&pop, &A, &C, &D))
				{
					position = pop;
					position.Y += 8;

					direction.Y -= cVect.Dot(direction);

					direction.RadiusNormalize();

					return TRUE;
				}

				if (collision->RayTriangleIntersect(&pop, &A, &B, &C))
				{
					position = pop;
					position.Y += 8;

					direction.Y -= cVect.Dot(direction);

					direction.RadiusNormalize();

					return TRUE;
				}
			}
		}

		if (collision->IntersectPlane(&pn, &A, &position, &cVect))
		{
			if (collision->l <= 8.0F)
			{
				pop = position + (cVect * collision->l);

				if (collision->RayTriangleIntersect(&pop, &A, &C, &D))
				{
					direction.X = (pn.X + dVect.X) / 2;
					direction.Z = (pn.Z + dVect.Z) / 2;

					direction.RadiusNormalize();

					position.X = pop.X + (pn.X * collision->l);
					position.Z = pop.Z + (pn.Z * collision->l);

					return TRUE;
				}

				if (collision->RayTriangleIntersect(&pop, &A, &B, &C))
				{
					direction.X = (pn.X + dVect.X) / 2;
					direction.Z = (pn.Z + dVect.Z) / 2;

					direction.RadiusNormalize();

					position.X = pop.X + (pn.X * collision->l);
					position.Z = pop.Z + (pn.Z * collision->l);

					return TRUE;
				}
			}
		}
	}

	return FALSE;
}

/*
Ground Clip

B-C
| |
A-D

ABD has N1
DBC has N2
*/
BOOL CPlayer::GroundCollision()
{
	x = ((INT)position.X + ((terrain->width + 1) * terrain->primSize) / 2) / terrain->primSize;
	z = ((INT)position.Z + ((terrain->height + 1) * terrain->primSize) / 2) / terrain->primSize;

	if ((x > (INT)terrain->width - 1) || (z > (INT)terrain->height - 1))
	{
		return FALSE;
	}

	if ((x < 0) || (z < 0))
	{
		return FALSE;
	}

	tc = (CTerrainClip*)terrain->clips->GetElement(2, x, z);

	if (tc == NULL)
	{
		return FALSE;
	}

	direction.Normalize();

	collision->IntersectPlane(&tc->n1, &tc->a, &position, &downVect);

	if (collision->l <= 8.0F)
	{
		pop.X = position.X;
		pop.Y = position.Y - collision->l;
		pop.Z = position.Z;

		if (collision->RayTriangleIntersect(&pop, &tc->a, &tc->b, &tc->d))
		{
			direction.Y -= tc->n1.Dot(direction);

			direction.RadiusNormalize();
#ifdef _DEBUG
			CPlayer::DrawClipABC(tc);
#endif
			position = pop;
			position.Y = pop.Y + 8;

			return TRUE;
		}
	}


	collision->IntersectPlane(&tc->n2, &tc->c, &position, &downVect);

	if (collision->l <= 8.0F)
	{
		pop.X = position.X;
		pop.Y = position.Y - collision->l;
		pop.Z = position.Z;

		if (collision->RayTriangleIntersect(&pop, &tc->d, &tc->b, &tc->c))
		{
			direction.Y -= tc->n2.Dot(direction);

			direction.RadiusNormalize();
#ifdef _DEBUG
			CPlayer::DrawClipDEF(tc);
#endif
			position = pop;
			position.Y = pop.Y + 8;

			return TRUE;
		}
	}

	return FALSE;
}
#ifdef _DEBUG
VOID CPlayer::DrawClipABC(CTerrainClip* tc)
{
	memcpy(&vnt[0], &tc->a, sizeof(CVertex));
	memcpy(&vnt[1], &tc->b, sizeof(CVertex));
	memcpy(&vnt[2], &tc->d, sizeof(CVertex));

	vertexBuffer->LoadBuffer(vnt);

	memcpy(&lv[0], &tc->a, sizeof(CVertex));
	memcpy(&lv[1], &tc->b, sizeof(CVertex));
	memcpy(&lv[2], &tc->d, sizeof(CVertex));

	clipLine[0]->Load(&lv[0], &lv[1]);
	clipLine[1]->Load(&lv[1], &lv[2]);
	clipLine[2]->Load(&lv[2], &lv[0]);

	lv[3].X = (tc->a.X + tc->b.X + tc->d.X) / 3;
	lv[3].Y = (tc->a.Y + tc->b.Y + tc->d.Y) / 3;
	lv[3].Z = (tc->a.Z + tc->b.Z + tc->d.Z) / 3;

	lv[4].X = lv[3].X + (tc->n1.X * 32.0F);
	lv[4].Y = lv[3].Y + (tc->n1.Y * 32.0F);
	lv[4].Z = lv[3].Z + (tc->n1.Z * 32.0F);

	normalLine->Load(&lv[3], &lv[4]);
}

VOID CPlayer::DrawClipDEF(CTerrainClip* tc)
{
	memcpy(&vnt[0], &tc->d, sizeof(CVertex));
	memcpy(&vnt[1], &tc->b, sizeof(CVertex));
	memcpy(&vnt[2], &tc->c, sizeof(CVertex));

	vertexBuffer->LoadBuffer(vnt);

	memcpy(&lv[0], &tc->d, sizeof(CVertex));
	memcpy(&lv[1], &tc->b, sizeof(CVertex));
	memcpy(&lv[2], &tc->c, sizeof(CVertex));

	clipLine[0]->Load(&lv[0], &lv[1]);
	clipLine[1]->Load(&lv[1], &lv[2]);
	clipLine[2]->Load(&lv[2], &lv[0]);

	lv[3].X = (tc->d.X + tc->b.X + tc->c.X) / 3;
	lv[3].Y = (tc->d.Y + tc->b.Y + tc->c.Y) / 3;
	lv[3].Z = (tc->d.Z + tc->b.Z + tc->c.Z) / 3;

	lv[4].X = lv[3].X + (tc->n2.X * 32.0F);
	lv[4].Y = lv[3].Y + (tc->n2.Y * 32.0F);
	lv[4].Z = lv[3].Z + (tc->n2.Z * 32.0F);

	normalLine->Load(&lv[3], &lv[4]);
}

VOID CPlayer::DrawDebug()
{
	if (clipBlend->effect)
	{
		hr = clipBlend->effect->Begin(&clipBlend->cPasses, 0);

		for (UINT i = 0; i < clipBlend->cPasses; i++)
		{
			hr = clipBlend->effect->BeginPass(i);

			hr = clipBlend->effect->CommitChanges();

			vertexBuffer->Draw();

			hr = clipBlend->effect->EndPass();
		}

		hr = clipBlend->effect->End();
	}

	if (lineBlend)
	{
		hr = lineBlend->effect->Begin(&lineBlend->cPasses, 0);

		for (UINT i = 0; i < lineBlend->cPasses; i++)
		{
			hr = lineBlend->effect->BeginPass(i);

			hr = lineBlend->effect->CommitChanges();

			normalLine->Draw();

			clipLine[0]->Draw();
			clipLine[1]->Draw();
			clipLine[2]->Draw();

			hr = lineBlend->effect->EndPass();
		}

		hr = lineBlend->effect->End();
	}
}
#endif