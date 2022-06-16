#ifndef _CTextureManager
#define _CTextureManager

#include "standard.h"

#include "CErrorLog.h"
#include "CLinkList.h"
#include "CTexture.h"
#include "CVideoDevice.h"

class CTextureManager
{
public:

	CTextureManager();
	
	CTextureManager(CErrorLog* el, CVideoDevice* d);

	~CTextureManager();

	void DeleteTexture(const char* fn);

	CTexture* MakeTexture(const char* fn);
	
	CTexture* GetTexture(const char* tn);

private:

	HRESULT hr;

	CErrorLog* errorLog;
	
	CVideoDevice* videoDevice;
	
	CLinkList* textures;
	
	CTexture* texture;
};
#endif