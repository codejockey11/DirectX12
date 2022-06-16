#include "CButton.h"

CButton::CButton()
{
	memset(this, 0x00, sizeof(CButton));
}

CButton::CButton(CGlobalObjects* g, const CHAR* imageOff, const CHAR* imageOn, CVertex2* s, CVertex2* p, BYTE a, BYTE t)
{
	memset(this, 0x00, sizeof(CButton));

	globalObjects = g;

	size = *s;
	position = *p;

	textureOff = new CHudImage(globalObjects->errorLog, globalObjects->videoDevice, globalObjects->textureMgr, globalObjects->frametime, imageOff, s, p, NULL);
	textureOn = new CHudImage(globalObjects->errorLog, globalObjects->videoDevice, globalObjects->textureMgr, globalObjects->frametime, imageOn, s, p, NULL);
	packet = new CPacket(0, 0, a, t, t, 1);
}

CButton::~CButton()
{
	delete textureOff;
	delete textureOn;
	delete packet;
}

VOID CButton::Draw()
{
	if ((globalObjects->mouse->position.X >= textureOff->vertices[0].X) && (globalObjects->mouse->position.X <= textureOff->vertices[1].X))
	{
		if ((globalObjects->mouse->position.Y >= textureOff->vertices[0].Y) && (globalObjects->mouse->position.Y <= textureOff->vertices[2].Y))
		{
			if (soundCount == 0)
			{
				globalObjects->soundMgr->GetSound("button2.wav")->StartSound();
			}

			textureDraw = textureOn;
			soundCount++;
			isMouseOver = TRUE;
		}
		else
		{
			textureDraw = textureOff;
			soundCount = 0;
			isMouseOver = FALSE;
		}
	}
	else
	{
		textureDraw = textureOff;
		soundCount = 0;
		isMouseOver = FALSE;
	}

	if (!textureDraw)
	{
		return;
	}

	textureDraw->Draw();
}