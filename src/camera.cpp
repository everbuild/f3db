/*----------------------------------------------------------------------------*\
  MODULE NAME: camera.cpp
      PROJECT: F3DB
       AUTHOR: Evert Bauwens
         DATE: september 8 2003
  DESCRIPTION: Camera class
\*----------------------------------------------------------------------------*/
#include "camera.h"
#include <string.h>



namespace F3DB {
	
	
	Camera::Camera() {
		memset(&position, 0, sizeof(D3DXVECTOR3));
		memset(&rotation, 0, sizeof(D3DXVECTOR3));
		zoom = 1.0;
		memset(&oldPosition, 0, sizeof(D3DXVECTOR3));
		memset(&oldRotation, 0, sizeof(D3DXVECTOR3));
		oldZoom = 1.0;
		fov = F3DB_FOV;
		nearClipZ = F3DB_NEARCLIPZ;
		farClipZ = F3DB_DISTANCE;
		aspect = F3DB_ASPECT;
	}
	
	
	void Camera::UpdateCamera() {
		if(position != oldPosition) {
			oldPosition = position;
			viewChanged = true;
		}
		if(rotation != oldRotation) {
			oldRotation = rotation;
			viewChanged = true;
		}
		if(zoom != oldZoom) {
			oldZoom = zoom;
			fov = F3DB_FOV/zoom;
			nearClipZ = F3DB_SCREENWIDTH/sinf(fov/2.0f);
			projChanged = true;
		}
	}
	
	void Camera::SetFarClippingZ(float z) {
		farClipZ = z;
		projChanged = true;
	}
	
	void Camera::SetAspectRatio(float aspect) {
		this->aspect = aspect;
		projChanged = true;
	}
	
	void Camera::GetViewMatrix(D3DXMATRIX &matrix) {
		D3DXVECTOR3 up, pos, lookat;
		memcpy(&pos, &position, sizeof(D3DXVECTOR3));
		up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		lookat = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		D3DXMatrixRotationYawPitchRoll(&matrix, rotation.y, rotation.x, rotation.z);
		D3DXVec3TransformNormal(&up, &up, &matrix);
		D3DXVec3TransformNormal(&lookat, &lookat, &matrix);
		lookat += pos;
		D3DXMatrixLookAtLH(&matrix, (D3DXVECTOR3*)&position, &lookat, &up);
	}
	
	void Camera::GetProjMatrix(D3DXMATRIX &matrix) {
		D3DXMatrixPerspectiveFovLH(&matrix, fov, aspect, nearClipZ, farClipZ);
	}
	
	float Camera::GetFarClippingZ() {
		return farClipZ;
	}
	
	float Camera::GetNearClippingZ() {
		return nearClipZ;
	}
	
	
}