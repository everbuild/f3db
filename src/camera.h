/*----------------------------------------------------------------------------*\
  MODULE NAME: camera.h
      PROJECT: F3DB
       AUTHOR: Evert Bauwens
         DATE: september 8 2003
  DESCRIPTION: Camera class
\*----------------------------------------------------------------------------*/
#ifndef F3DB_CAMERA_H
#define F3DB_CAMERA_H
#include "d3dx9math.h"
#include <math.h>


#define F3DB_FOV 0.7853981f
#define F3DB_SCREENWIDTH 0.36f
#define F3DB_DISTANCE 500.0f
#define F3DB_ASPECT 1.3333333f
const float F3DB_NEARCLIPZ = F3DB_SCREENWIDTH/sinf(F3DB_FOV/2.0f);



namespace F3DB {
	
	
	class Camera {
		
	public:
		
		D3DXVECTOR3 position, rotation;
		float zoom;
		
		bool viewChanged, projChanged;
		
		Camera();
		
		void UpdateCamera();
		void SetFarClippingZ(float z);
		void SetAspectRatio(float aspect);
		void GetViewMatrix(D3DXMATRIX &matrix);
		void GetProjMatrix(D3DXMATRIX &matrix);
		float GetFarClippingZ();
		float GetNearClippingZ();
		
	private:
		
		D3DXVECTOR3 oldPosition, oldRotation;
		float oldZoom, fov, nearClipZ, farClipZ, aspect;
	};
	
	
	
}



#endif // #ifndef F3DB_CAMERA_H