#include "CShader.h"

CShader::CShader()
{
	memset(this, 0x00, sizeof(CShader));
}

CShader::CShader(CErrorLog* el, const char* sn, const char* ep, const char* v)
{
	memset(this, 0x00, sizeof(CShader));

	errorLog = el;

	filename = new CString(sn);
	
	entryPoint = new CString(ep);
	
	version = new CString(v);

	flags = D3DCOMPILE_ENABLE_UNBOUNDED_DESCRIPTOR_TABLES;

#ifdef _DEBUG
	flags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	hr = D3DCompileFromFile(MBSToWide(filename->GetText()), nullptr, nullptr, ep, v, flags, 0, &shader, &errors);

	if (hr < 0)
	{
		errorLog->WriteError("CShader::CShader::D3DCompileFromFile:%s:%s\n", filename->GetText(), errorLog->GetComErrorMessage(hr));

		shader = nullptr;

		if (errors)
		{
			errorLog->WriteError("Compile Errors\n%s", (char*)errors->GetBufferPointer());
		}
	}
}

CShader::~CShader()
{
	delete filename;
	
	delete entryPoint;
	
	delete version;

	if (shader)
	{
		shader->Release();
		shader = 0;
	}

	if (errors)
	{
		errors->Release();
		errors = 0;
	}
}
