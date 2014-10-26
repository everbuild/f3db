/*----------------------------------------------------------------------------*\
  MODULE NAME: car.h
      PROJECT: F3DB
       AUTHOR: Evert Bauwens
         DATE: september 8 2003
  DESCRIPTION: 
\*----------------------------------------------------------------------------*/
#ifndef F3DB_CAR_H
#define F3DB_CAR_H
#include "obstacle.h"


#define F3DB_CAR_X 32.0f
#define F3DB_CAR_Y 0.7f
#define F3DB_CAR_Z 13.0f
#define F3DB_CAR_DIST 64.0f


namespace F3DB {


	class Car;
	typedef list<Car*> CarList;

	class Car: public Obstacle {
		
	public:

		Car(Scene::Class *cls);

		void AvoidCollisions(CarList &cars);
	};

	
}


#endif // #ifndef F3DB_CAR_H