#ifndef _CShaderManager
#define _CShaderManager

#include "standard.h"

#include "CErrorLog.h"
#include "CLinkList.h"
#include "CShader.h"

class CShaderManager
{
public:

	CLinkList* shaders;

	CShaderManager();

	CShaderManager(CErrorLog* el);

	~CShaderManager();

	CShader* MakeShader(const char* sn, const char* ep, const char* v);

	CShader* GetShader(const char* sn);

	void DeleteShader(const char* sn);

private:

	HRESULT hr;

	CErrorLog* errorLog;

	CShader* shader;
};
#endif