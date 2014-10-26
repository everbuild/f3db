/*----------------------------------------------------------------------------*\
  MODULE NAME: frog.h
      PROJECT: F3DB
       AUTHOR: Evert Bauwens
         DATE: september 8 2003
  DESCRIPTION: 
\*----------------------------------------------------------------------------*/
#ifndef F3DB_FROG_H
#define F3DB_FROG_H
#include "scene.h"


namespace F3DB {


	class Frog: public Scene::Object {
		
	public:

		enum Status {
			F3DB_FS_IDLE,
			F3DB_FS_ONLEAF,
			F3DB_FS_DYING,
			F3DB_FS_DRAWNING,
			F3DB_FS_ATFINISH
		};

		Status status;
		
		Frog(Scene::Class *cls);

		void Update(double time);

	private:

		float GetY(float z);

	};

	
}


#endif // #ifndef F3DB_FROG_H