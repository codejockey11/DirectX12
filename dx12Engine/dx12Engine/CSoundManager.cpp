#include "CSoundManager.h"

CSoundManager::CSoundManager()
{
	memset(this, 0x00, sizeof(CSoundManager));
}

CSoundManager::CSoundManager(CErrorLog* el, CSoundDevice* d)
{
	memset(this, 0x00, sizeof(CSoundManager));

	errorLog = el;
	
	soundDevice = d;

	sounds = new CLinkList();
}

CSoundManager::~CSoundManager()
{
	CLinkListNode* lln = sounds->list;

	while (lln)
	{
		CSound* e = (CSound*)lln->element;

		delete e;

		lln = lln->next;
	}

	sounds = nullptr;
}

CSound* CSoundManager::MakeSound(const char* fn, BYTE loop)
{
	CSound* sc = CSoundManager::GetSound(fn);

	if (sc)
	{
		return sc;
	}

	sound = new CSound(errorLog, soundDevice, fn, loop);

	if (sound->initialized)
	{
		sounds->Add(sound, sound->filename->GetText());

		return sound;
	}

	errorLog->WriteError("CSoundManager::MakeSound:Failed:%s\n", fn);

	delete sound;

	return nullptr;
}

CSound* CSoundManager::GetSound(const char* sn)
{
	CLinkListNode* sll = sounds->Search(sn);

	if (sll)
	{
		return (CSound*)sll->element;
	}

	return nullptr;
}
