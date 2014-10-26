/*----------------------------------------------------------------------------*\
  MODULE NAME: frog.cpp
      PROJECT: F3DB
       AUTHOR: Evert Bauwens
         DATE: september 8 2003
  DESCRIPTION: 
\*----------------------------------------------------------------------------*/
#include "frog.h"


namespace F3DB {

	
	Frog::Frog(Scene::Class *cls): Object(cls) {
		status = F3DB_FS_IDLE;
	}


	void Frog::Update(double time) {
		Object::Update(time);
		if(status == F3DB_FS_IDLE) {
			D3DXVECTOR3 pos = GetPosition();
			pos.y = GetY(GetGlobalPosition().z);
			SetPosition(pos);
		}
	}


	float Frog::GetY(float z) {
		if(z < 11.0f) return 0.5f;
		if(z < 11.5f) return 0.5f + z - 11.0f;
		if(z < 20.5f) return 1.0f;
		if(z < 21.0f) return 1.0f - z + 20.5f;
		if(z < 22.5f) return 0.5f;
		if(z < 27.5f) return -0.3f;
		return 0.5f;
	}
	
}
