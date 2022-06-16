#ifndef _CLinkList
#define _CLinkList

#include "standard.h"

#include "CLinkListNode.h"
#include "CString.h"

class CLinkList
{
public:

	CLinkListNode* list;

	INT count;

	CLinkList();

	~CLinkList();

	VOID Clear();

	VOID Add(void* n, const CHAR* k);

	VOID Add(void* n, INT k);

	CLinkListNode* Delete(CLinkListNode* n);

	CLinkListNode* Search(const CHAR* k);

	CLinkListNode* Search(INT k);
};
#endif