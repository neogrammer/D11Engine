#include "Camera.h"

Camera::Camera()
{
	this->pos = XMFLOAT3(0.f, 0.f, 0.f);
	this->posVector = XMLoadFloat3(&this->pos);
	this->rot = XMFLOAT3(0.f, 0.f, 0.f);
	this->rotVector = XMLoadFloat3(&this->rot);
	UpdateViewMatrix();
}

void Camera::SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ)
{
	float fovRadians = (fovDegrees / 360.f) * XM_2PI;
	this->projectionMatrix = XMMatrixPerspectiveFovLH(fovRadians, aspectRatio, nearZ, farZ);
}

const XMMATRIX& Camera::GetViewMatrix() const
{
	return this->viewMatrix;
}

const XMMATRIX& Camera::GetProjectionMatrix() const
{
	// TODO: insert return statement here
	return this->projectionMatrix;
}

const XMVECTOR& Camera::GetPositionVector() const
{
	// TODO: insert return statement here
	return this->posVector;
}

const XMFLOAT3& Camera::GetPositionFloat3() const
{
	// TODO: insert return statement here}
	return this->pos;
}

const XMVECTOR& Camera::GetRotationVector() const
{
	// TODO: insert return statement here
	return this->rotVector;
}

const XMFLOAT3& Camera::GetRotationFloat3() const
{
	// TODO: insert return statement here
	return this->rot;
}

void Camera::SetPosition(const XMVECTOR& pos)
{
	XMStoreFloat3(&this->pos, pos);
	this->posVector = pos;
	UpdateViewMatrix();
}

void Camera::SetPosition(float x, float y, float z)
{
	this->pos = XMFLOAT3(x, y, z);
	this->posVector = XMLoadFloat3(&this->pos);
	UpdateViewMatrix();
}

void Camera::AdjustPosition(const XMVECTOR& pos)
{
	this->posVector += pos;
	XMStoreFloat3(&this->pos, this->posVector);
	UpdateViewMatrix();
}

void Camera::AdjustPosition(float x, float y, float z)
{
	this->pos.x += x;
	this->pos.y += y;
	this->pos.z += z;
	this->posVector = XMLoadFloat3(&this->pos);
	this->UpdateViewMatrix();
}

void Camera::SetRotation(const XMVECTOR& rot)
{
	this->rotVector = rot;
	XMStoreFloat3(&this->rot, rot);
	UpdateViewMatrix();
}

void Camera::SetRotation(float x, float y, float z)
{
	this->rot = XMFLOAT3(x, y, z);
	this->rotVector = XMLoadFloat3(&this->rot);
	this->UpdateViewMatrix();
}

void Camera::AdjustRotation(const XMVECTOR& rot)
{
	this->rotVector += rot;
	XMStoreFloat3(&this->rot, this->rotVector);
	this->UpdateViewMatrix();
}

void Camera::AdjustRotation(float x, float y, float z)
{
	this->rot.x += x;
	this->rot.y += y;
	this->rot.z += z;
	this->rotVector = XMLoadFloat3(&this->rot);
	this->UpdateViewMatrix();
}

void Camera::UpdateViewMatrix()
{
	// calc camera rot matrix
	XMMATRIX camRotationMatrix = XMMatrixRotationRollPitchYaw(this->rot.x, this->rot.y, this->rot.z);
	// calc unit vector of cam target based on camera forward value transformed by cam rotation
	XMVECTOR camTarget = XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, camRotationMatrix);
	// Adjust cam target to be offset by the camera's current position
	camTarget += this->posVector;
	// calculate up direction based on current rotation
	XMVECTOR upDir = XMVector3TransformCoord(this->DEFAULT_UP_VECTOR, camRotationMatrix);
	// rebuild view matrix
	this->viewMatrix = XMMatrixLookAtLH(this->posVector, camTarget, upDir);
}
