#include "CShaderManager.h"

CShaderManager::CShaderManager()
{
	memset(this, 0x00, sizeof(CShaderManager));
}

CShaderManager::CShaderManager(CErrorLog* el)
{
	memset(this, 0x00, sizeof(CShaderManager));

	errorLog = el;

	shaders = new CLinkList();
}

CShaderManager::~CShaderManager()
{
	CLinkListNode* lln = shaders->list;

	while (lln)
	{
		CShader* e = (CShader*)lln->element;

		delete e;

		lln = lln->next;
	}

	shaders = nullptr;
}

CShader* CShaderManager::MakeShader(const char* sn, const char* ep, const char* v)
{
	CShader* s = CShaderManager::GetShader(sn);
	
	if (s)
	{
		return s;
	}

	shader = new CShader(errorLog, sn, ep, v);

	if (shader->shader)
	{
		shaders->Add(shader, shader->filename->GetText());

		return shader;
	}

	errorLog->WriteError("CShaderManager::MakeShader:%s\n", sn);

	delete shader;

	return nullptr;
}

CShader* CShaderManager::GetShader(const char* sn)
{
	CLinkListNode* s = shaders->Search(sn);

	if (s)
	{
		return (CShader*)s->element;
	}

	return nullptr;
}

void CShaderManager::DeleteShader(const char* sn)
{
	CLinkListNode* s = shaders->Search(sn);

	if (s)
	{
		shaders->Delete(s);
	}
}
