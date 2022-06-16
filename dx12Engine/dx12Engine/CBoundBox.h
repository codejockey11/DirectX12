#ifndef _CBoundBox
#define _CBoundBox

#include "standard.h"

#include "CVertex.h"

class CBoundBox
{
public:

	CVertex min;
	
	CVertex max;
	
	CBoundBox();

	CBoundBox(CBoundBox* b);

	CBoundBox(CVertex* mn, CVertex* mx);
	
	~CBoundBox();

	void Copy(CBoundBox* b);

	void Update(CVertex* mn, CVertex* mx);

	BYTE CheckPointInBox(CVertex* p);
};
#endif