#ifndef _CPLAYER
#define _CPLAYER

#include "standard.h"

#include "CBrushMap.h"
#include "CCamera.h"
#include "CCollision.h"
#include "CFrameTime.h"
#include "CGlobalObjects.h"
#include "CHeapArray.h"
#include "CLine.h"
#include "CLinkList.h"
#include "CPacket.h"
#include "CTerrain.h"
#include "CVertexBuffer.h"

class CPlayer
{
public:

	CCamera* camera;
	
	CCollision* collision;

	CVertex2 coordinates;
	
	CVertex position;
	
	CVertex direction;
	
	CVertex pop;

	CVertexBuffer* vertexBuffer;

	CTerrainClip* tc;

	INT x;
	
	INT z;

	CString* name;

	CPlayer();

	CPlayer(CGlobalObjects* g, CBrushMap* b, CTerrain* t, FLOAT r, const CHAR* n);

	~CPlayer();

	VOID PlayerMove(CPacket* p);

	VOID DrawDebug();

private:

	HRESULT hr;

	CGlobalObjects* globalObjects;

	CBrushMap* brushMap;
	
	CTerrain* terrain;

	CVertex c;
	
	CVertex cVect;
	
	CVertex dVect;
	
	CVertex upVect;
	
	CVertex downVect;

	FLOAT d;
		  
	FLOAT distance;
		  
	FLOAT rate;

	BOOL GroundCollision();

	BOOL BrushCollision(CBrush* brush);

#ifdef _DEBUG
	CEffect* clipBlend;
	
	CEffect* lineBlend;

	CVertex vnt[3];
	CVertex lv[5];

	CLine* normalLine;

	CLine* clipLine[3];

	VOID DrawClipABC(CTerrainClip* tc);

	VOID DrawClipDEF(CTerrainClip* tc);
#endif
};
#endif