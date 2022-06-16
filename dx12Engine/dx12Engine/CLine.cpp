#include "CLine.h"


CLine::CLine()
{
	memset(this, 0x00, sizeof(CLine));

}


CLine::CLine(CErrorLog* el, CVideoDevice* d, CTextureManager* tm, CVec3* f, CVec3* t)
{
	memset(this, 0x00, sizeof(CLine));

	errorLog = el;
	videoDevice = d;
	textureMgr = tm;

	texture = textureMgr->MakeTexture("red.bmp", D3DCOLOR_XRGB(255, 0, 255));

	vertexBuffer = new CVertexBuffer(errorLog, videoDevice, _CUSTOMVERTEX::CUSTOMVERTEXLINE, 6);

	line[0].p1 = *f;
	line[1].p1 = *t;

	vertexBuffer->LoadBuffer(line);

}


CLine::~CLine()
{
	SAFE_DELETE(vertexBuffer);

}


void CLine::Draw()
{
	videoDevice->device->SetTexture(0, texture->image);

	vertexBuffer->Draw();

}
