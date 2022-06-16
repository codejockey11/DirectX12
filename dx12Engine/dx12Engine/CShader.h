#ifndef _CShader
#define _CShader

#include "standard.h"

#include "CErrorLog.h"
#include "CString.h"

class CShader
{
public:

	CString* filename;
	CString* version;
	CString* entryPoint;

	ID3DBlob* shader;

	CShader();

	CShader(CErrorLog* el, const char* sn, const char* ep, const char* v);

	~CShader();

private:

	HRESULT hr;

	CErrorLog* errorLog;

	UINT flags;

	ID3DBlob* errors;
};
#endif