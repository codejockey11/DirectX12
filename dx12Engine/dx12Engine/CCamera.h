#ifndef _CCamera
#define _CCamera

#include "standard.h"

#include "CVertex.h"

class CCamera
{
public:

	XMFLOAT3	position;
	XMFLOAT3	rotation;
		
	XMFLOAT3	right;
	XMFLOAT3	up;
	XMFLOAT3	look;

	float		yaw;
	float		pitch;
	float		roll;

	XMMATRIX	wvpMat;

	CCamera();

	CCamera(float width, float height);
	
	~CCamera();

	void UpdateView();
	
	void UpdateRotation(float x, float y, float z);

	void Copy(CCamera* p);

private:

	XMFLOAT4X4  xmfworld;
	XMFLOAT4X4  xmfview;
	XMFLOAT4X4  xmfproj;

	XMMATRIX	xmworld;
	XMMATRIX	xmview;
	XMMATRIX	xmproj;

	XMFLOAT3	xmfup;
	XMFLOAT3	xmflook;

	XMMATRIX	xmMatrix;

	XMVECTOR	xmvlook;
	XMVECTOR	xmvup;
	XMVECTOR	xmvposition;

	XMMATRIX	twvpMat;
};
#endif