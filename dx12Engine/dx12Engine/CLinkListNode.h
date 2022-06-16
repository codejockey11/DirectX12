#ifndef _CLinkListNode
#define _CLinkListNode

#include "standard.h"

#include "CString.h"

class CLinkListNode
{
public:

	CLinkListNode* previous;
	CLinkListNode* next;

	CString* key;

	INT ikey;

	BOOL deleteMe;

	void* element;

	CLinkListNode();

	~CLinkListNode();
};
#endif