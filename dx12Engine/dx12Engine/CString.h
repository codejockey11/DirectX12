#ifndef _CString
#define _CString

#include "standard.h"

class CString
{
public:

	CString();

	CString(const CHAR* s);

	CString(UINT l);

	~CString();

	UINT GetLength() { return length; }

	CHAR* GetText() { return text; }

	VOID Clear();

	VOID Format(const CHAR* format, ...);

private:

	UINT length;

	CHAR* text;

	INT l;

	CHAR* t;

	va_list	argptr;
};
#endif