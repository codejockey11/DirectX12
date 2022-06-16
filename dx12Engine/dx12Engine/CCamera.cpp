#include "CCamera.h"

CCamera::CCamera()
{
	memset(this, 0x00, sizeof(CCamera));
}

CCamera::CCamera(float width, float height)
{
	memset(this, 0x00, sizeof(CCamera));

	xmworld = XMMatrixIdentity();

	XMStoreFloat4x4(&xmfworld, xmworld);

	xmproj = XMMatrixPerspectiveFovLH(45.0F * (3.14F / 180.0F), width / height, 1.0F, 50000.0F);

	XMStoreFloat4x4(&xmfproj, xmproj);
}

CCamera::~CCamera()
{
}

void CCamera::UpdateRotation(float x, float y, float z)
{
	rotation.x += x;

	if (rotation.x < 0.0F)
	{
		rotation.x = 359.9F;
	}
	else if (rotation.x > 359.9F)
	{
		rotation.x = 0.0F;
	}

	rotation.y += y;

	if (rotation.y < 0.0F)
	{
		rotation.y = 359.9F;
	}
	else if (rotation.y > 359.9F)
	{
		rotation.y = 0.0F;
	}

	rotation.z += z;

	if (rotation.z < 0.0F)
	{
		rotation.z = 359.9F;
	}
	else if (rotation.z > 359.9F)
	{
		rotation.z = 0.0F;
	}
}

void CCamera::UpdateView()
{
	yaw   = rotation.y * PiDiv180;
	pitch = rotation.x * PiDiv180;
	roll  = rotation.z * PiDiv180;

	xmMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);


	xmflook.x = 0.0F;
	xmflook.y = 0.0F;
	xmflook.z = 1.0F;

	xmvlook = XMLoadFloat3(&xmflook);

	xmvlook = XMVector3TransformCoord(xmvlook, xmMatrix);

	xmflook = XMFLOAT3(XMVectorGetX(xmvlook), XMVectorGetY(xmvlook), XMVectorGetZ(xmvlook));


	xmflook.x = xmflook.x + position.x;
	xmflook.y = xmflook.y + position.y;
	xmflook.z = xmflook.z + position.z;

	xmvlook = XMLoadFloat3(&xmflook);

	
	xmfup.x = 0.0F;
	xmfup.y = 1.0F;
	xmfup.z = 0.0F;

	xmvup = XMLoadFloat3(&xmfup);

	xmvup = XMVector3TransformCoord(xmvup, xmMatrix);

	xmfup = XMFLOAT3(XMVectorGetX(xmvup), XMVectorGetY(xmvup), XMVectorGetZ(xmvup));

	
	xmvposition = XMLoadFloat3(&position);

	
	xmview = XMMatrixLookAtLH(xmvposition, xmvlook, xmvup);

	XMStoreFloat4x4(&xmfview, xmview);


	right = XMFLOAT3(xmfview.m[0][0], xmfview.m[1][0], xmfview.m[2][0]);
	up    = XMFLOAT3(xmfview.m[0][1], xmfview.m[1][1], xmfview.m[2][1]);
	look  = XMFLOAT3(xmfview.m[0][2], xmfview.m[1][2], xmfview.m[2][2]);


	twvpMat = xmworld * xmview * xmproj;

	wvpMat = XMMatrixTranspose(twvpMat);
}

void CCamera::Copy(CCamera* c)
{
	xmworld = c->xmworld;

	xmproj = c->xmproj;

	position = c->position;
	
	rotation = c->rotation;

	CCamera::UpdateView();
}
