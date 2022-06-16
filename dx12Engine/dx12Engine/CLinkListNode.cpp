#include "CLinkListNode.h"

CLinkListNode::CLinkListNode()
{
	memset(this, 0x00, sizeof(CLinkListNode));
}

CLinkListNode::~CLinkListNode()
{
	delete element;
}
