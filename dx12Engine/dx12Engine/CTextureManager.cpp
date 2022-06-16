#include "CTextureManager.h"

CTextureManager::CTextureManager()
{
	memset(this, 0x00, sizeof(CTextureManager));
}

CTextureManager::CTextureManager(CErrorLog* el, CVideoDevice* d)
{
	memset(this, 0x00, sizeof(CTextureManager));

	errorLog = el;
	
	videoDevice = d;

	textures = new CLinkList();
}

CTextureManager::~CTextureManager()
{
	CLinkListNode* lln = textures->list;

	while (lln)
	{
		CTexture* e = (CTexture*)lln->element;

		delete e;

		lln = lln->next;
	}

	textures = nullptr;
}

CTexture* CTextureManager::MakeTexture(const char* fn)
{
	CTexture* t = CTextureManager::GetTexture(fn);
	
	if (t)
	{
		return t;
	}

	texture = new CTexture(errorLog, videoDevice, fn);

	if (texture->isInitialized)
	{
		textures->Add(texture, fn);

		return texture;
	}

	delete texture;

	errorLog->WriteError("CTextureManager::MakeTexture:%s\n", fn);

	return nullptr;
}

void CTextureManager::DeleteTexture(const char* fn)
{
	CLinkListNode* tl = textures->Search(fn);
	
	if (tl)
	{
		textures->Delete(tl);
	}
}

CTexture* CTextureManager::GetTexture(const char* tn)
{
	CLinkListNode* t = textures->Search(tn);

	if (t)
	{
		return (CTexture*)t->element;
	}

	return nullptr;
}
