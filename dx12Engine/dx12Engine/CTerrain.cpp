#include "CTerrain.h"

CTerrain::CTerrain()
{
	memset(this, 0x00, sizeof(CTerrain));
}

CTerrain::CTerrain(CGlobalObjects* g, const CHAR* filename)
{
	memset(this, 0x00, sizeof(CTerrain));

	globalObjects = g;

	err = fopen_s(&fp, filename, "rb");

	if (err)
	{
		globalObjects->errorLog->WriteError("CTerrain::CTerrain:Terrain not found:%s\n", filename);

		fp = 0;

		return;
	}

	br = fread_s(&width, sizeof(UINT), sizeof(UINT), 1, fp);
	br = fread_s(&height, sizeof(UINT), sizeof(UINT), 1, fp);
	br = fread_s(&primSize, sizeof(UINT), sizeof(UINT), 1, fp);


	vertices = (CVertexNT*)malloc(sizeof(CVertexNT) * width * height);


	if (vertices)
	{
		br = fread_s(vertices, sizeof(CVertexNT) * width * height, sizeof(CVertexNT), (size_t)width * height, fp);
	}

	if (fp)
	{
		fclose(fp);
	}

	vertexBuffer = new CVertexBuffer(globalObjects->errorLog, globalObjects->videoDevice, CVertex::VT_VERTEXNT, width * height, (VOID*)vertices);

	free(vertices);

	width -= 1;
	height -= 1;

	DWORD* indices = (DWORD*)malloc(sizeof(DWORD) * width * height * 6);

	DWORD* indicesh = indices;

	CHeapArray* iha = new CHeapArray(sizeof(BYTE), FALSE, TRUE, 2, width + 1, height + 1);
	
	for (UINT h = 0; h < height;h++)
	{
		for (UINT w = 0; w < width; w++)
		{
			*indices = iha->GetOffset(2, w, h);
			indices++;
	
			*indices = iha->GetOffset(2, w, h + 1);
			indices++;
	
			*indices = iha->GetOffset(2, w + 1, h);
			indices++;
	
	
	
			*indices = iha->GetOffset(2, w + 1, h);
			indices++;
	
			*indices = iha->GetOffset(2, w, h + 1);
			indices++;
	
			*indices = iha->GetOffset(2, w + 1, h + 1);
			indices++;
		}
	}
	
	delete iha;
	
	indexBuffer = new CIndexBuffer(globalObjects->errorLog, globalObjects->videoDevice, width * height * 6, width * height * 2, indicesh);

	free(indicesh);




	err = fopen_s(&fp, "clips.txt", "rb");

	if (fp)
	{
		clips = new CHeapArray(sizeof(CTerrainClip), TRUE, TRUE, 2, width, height);

		br = fread_s(clips->heap, sizeof(CTerrainClip) * width * height, sizeof(CTerrainClip), width * height, fp);

		fclose(fp);
	}

	//effect = globalObjects->effectMgr->GetEffect("normalMap.fx");
	effect = globalObjects->effectMgr->GetEffect("terrain.fx");

	if (effect)
	{
		hr = effect->effect->SetFloatArray(effect->hmaterial, White.GetFloatArray(), 4);

		hr = effect->effect->SetTexture(effect->htexture00, globalObjects->textureMgr->GetTexture("grass_01_d.tga")->image);
		hr = effect->effect->SetTexture(effect->htexture01, globalObjects->textureMgr->GetTexture("dgrass_01_d.tga")->image);
		hr = effect->effect->SetTexture(effect->htexture02, globalObjects->textureMgr->GetTexture("rock_01_d.tga")->image);
		hr = effect->effect->SetTexture(effect->htexture03, globalObjects->textureMgr->GetTexture("mask1.bmp")->image);
		hr = effect->effect->SetTexture(effect->htexture04, globalObjects->textureMgr->GetTexture("mask2.bmp")->image);
		hr = effect->effect->SetTexture(effect->htexture05, globalObjects->textureMgr->GetTexture("lightmap.bmp")->image);
	}

	CVertex v1(0.0F, 16.0F, 0.0F);

	CVertex v2(32.0F, 32.0F, 32.0F);

	CVertex2 v3(1.0F, 1.0F);

	CVertex2 v4(0.0F, 0.0F);

	CDBrush* tb = new CDBrush(globalObjects, &v1, &v2, &v3, &v4, NULL);

	tb->side[CBrushSide::BBS_LEFT]->texture = globalObjects->textureMgr->GetTexture("rocks.jpg");
	tb->side[CBrushSide::BBS_RIGHT]->texture = globalObjects->textureMgr->GetTexture("rocks.jpg");
	tb->side[CBrushSide::BBS_FRONT]->texture = globalObjects->textureMgr->GetTexture("rocks.jpg");
	tb->side[CBrushSide::BBS_BACK]->texture = globalObjects->textureMgr->GetTexture("rocks.jpg");
	tb->side[CBrushSide::BBS_TOP]->texture = globalObjects->textureMgr->GetTexture("stoneWall.tga");
	tb->side[CBrushSide::BBS_BOTTOM]->texture = globalObjects->textureMgr->GetTexture("stoneWall.tga");

	tb->side[CBrushSide::BBS_LEFT]->effect = globalObjects->effectMgr->GetEffect("brush.fx");
	tb->side[CBrushSide::BBS_RIGHT]->effect = globalObjects->effectMgr->GetEffect("brush.fx");
	tb->side[CBrushSide::BBS_FRONT]->effect = globalObjects->effectMgr->GetEffect("brush.fx");
	tb->side[CBrushSide::BBS_BACK]->effect = globalObjects->effectMgr->GetEffect("brush.fx");
	tb->side[CBrushSide::BBS_TOP]->effect = globalObjects->effectMgr->GetEffect("normalMap.fx");
	tb->side[CBrushSide::BBS_BOTTOM]->effect = globalObjects->effectMgr->GetEffect("brush.fx");

	tb->rotX =  0.0F;
	tb->rotY = 90.0F;
	tb->rotZ =  0.0F;

	brushes = new CLinkList();

	brushes->Add(tb, NULL);
}

CTerrain::~CTerrain()
{
	delete brushes;
	delete clips;
	delete vertexBuffer;
	delete indexBuffer;
}

VOID CTerrain::Draw()
{
	if (effect)
	{
		hr = effect->effect->Begin(&effect->cPasses, 0);

		for (UINT i = 0; i < effect->cPasses; i++)
		{
			hr = effect->effect->BeginPass(i);

			hr = effect->effect->CommitChanges();

			//vertexBuffer->Draw();
			vertexBuffer->DrawIndexed(indexBuffer);

			hr = effect->effect->EndPass();
		}

		hr = effect->effect->End();
	}
	else
	{
		hr = globalObjects->videoDevice->device->SetTexture(0, globalObjects->textureMgr->GetTexture("grass_03_d.tga")->image);

		//vertexBuffer->Draw();
		vertexBuffer->DrawIndexed(indexBuffer);
	}

	if (brushes)
	{
		CLinkListNode* lln = brushes->list;

		while (lln->element)
		{
			CDBrush* br = (CDBrush*)lln->element;

			br->UpdateRotation();

			br->Rotation();

			br->Draw();

			lln = lln->next;
		}
	}
}