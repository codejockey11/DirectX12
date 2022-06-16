#include "CLinkList.h"

CLinkList::CLinkList()
{
	memset(this, 0x00, sizeof(CLinkList));

	list = new CLinkListNode();
}

CLinkList::~CLinkList()
{
	CLinkListNode* lln = list;

	while (lln)
	{
		CLinkListNode* n = lln->next;

		delete lln;

		lln = n;
	}

	list = nullptr;
}

VOID CLinkList::Clear()
{
	CLinkListNode* p = list;

	while (p->element)
	{
		p = CLinkList::Delete(p);
	}
}

VOID CLinkList::Add(void* n, const CHAR* k)
{
	if (list->element == 0)
	{
		list->element = n;
		list->key = new CString(k);
		list->next = new CLinkListNode();

		count++;

		return;
	}

	CLinkListNode* previous = 0;
	CLinkListNode* traverser = list;

	while (traverser->element)
	{
		previous = traverser;
		traverser = traverser->next;
	}

	traverser->previous = previous;
	traverser->element = n;
	traverser->key = new CString(k);
	traverser->next = new CLinkListNode();

	count++;
}

VOID CLinkList::Add(void* n, INT k)
{
	if (list->element == 0)
	{
		list->element = n;
		list->ikey = k;
		list->next = new CLinkListNode();

		count++;

		return;
	}

	CLinkListNode* previous = 0;
	CLinkListNode* traverser = list;

	while (traverser->element)
	{
		previous = traverser;
		traverser = traverser->next;
	}

	traverser->previous = previous;
	traverser->element = n;
	traverser->ikey = k;
	traverser->next = new CLinkListNode();

	count++;
}

CLinkListNode* CLinkList::Delete(CLinkListNode* n)
{
	if (list == n)
	{
		list->next->previous = 0;

		CLinkListNode* next = list->next;

		delete list;

		list = next;

		count--;

		if (count == 0)
		{
			list->next = new CLinkListNode();
		}

		return list;
	}

	CLinkListNode* traverser = list;

	while (traverser->element)
	{
		if (traverser == n)
		{
			CLinkListNode* next = traverser->next;

			traverser->previous->next = traverser->next;
			traverser->next->previous = traverser->previous;

			delete traverser;

			count--;

			return next;
		}

		traverser = traverser->next;
	}

	return NULL;
}

CLinkListNode* CLinkList::Search(const CHAR* k)
{
	CLinkListNode* traverser = list;

	while (traverser->element)
	{
		if (strcmp(traverser->key->GetText(), k) == 0)
		{
			return traverser;
		}

		traverser = traverser->next;
	}

	return NULL;
}

CLinkListNode* CLinkList::Search(INT k)
{
	CLinkListNode* traverser = list;

	while (traverser->element)
	{
		if (traverser->ikey == k)
		{
			return traverser;
		}

		traverser = traverser->next;
	}

	return NULL;
}
