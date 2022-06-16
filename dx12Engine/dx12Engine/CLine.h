#ifndef _CLINE
#define _CLINE

#include "standard.h"

#include "CCustomVertexLine.h"
#include "CErrorLog.h"
#include "CLinkList.h"
#include "CTexture.h"
#include "CTextureManager.h"
#include "CVertexBuffer.h"
#include "CVideoDevice.h"

class CLine
{
	public:

		CCustomVertexLine line[6];

		CLine();
		CLine::CLine(CErrorLog* el, CVideoDevice* d, CTextureManager* tm, CVec3* f, CVec3* t);

		~CLine();

		void CLine::Draw();

	private:

		CErrorLog*			errorLog;
		CVideoDevice*		videoDevice;
		CTextureManager*	textureMgr;

		CVertexBuffer*	vertexBuffer;
		CTexture*		texture;

};

#endif