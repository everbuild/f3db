/*----------------------------------------------------------------------------*\
  MODULE NAME: car.cpp
      PROJECT: F3DB
       AUTHOR: Evert Bauwens
         DATE: september 8 2003
  DESCRIPTION: 
\*----------------------------------------------------------------------------*/
#include "car.h"


namespace F3DB {

	
	Car::Car(Scene::Class *cls): Obstacle(cls, F3DB_CAR_X, F3DB_CAR_Y, F3DB_CAR_Z, F3DB_CAR_DIST, 3.3f, 1.6f) {
	}


	void Car::AvoidCollisions(CarList &cars) {
		ObstacleList obstacles;
		for(CarList::iterator i = cars.begin(); i != cars.end(); i ++) obstacles.push_back(*i);
		Obstacle::AvoidCollisions(obstacles);
	}


}
