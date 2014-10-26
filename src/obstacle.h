/*----------------------------------------------------------------------------*\
  MODULE NAME: obstacle.h
      PROJECT: F3DB
       AUTHOR: Evert Bauwens
         DATE: september 8 2003
  DESCRIPTION: Base class for leaf and car.
\*----------------------------------------------------------------------------*/
#ifndef F3DB_OBSTACLE_H
#define F3DB_OBSTACLE_H
#include "scene.h"



namespace F3DB {


	class Obstacle;

	typedef std::list<Obstacle*> ObstacleList;


	class Obstacle: public Scene::Object {
		
	public:

		bool started;
		uint8 lane;

		Obstacle(Scene::Class *cls, float x, float y, float z, float dist, float width, float depth);

		void Start(uint8 lane, float speed);
		void Stop();

		virtual void Update(double time);

		void AvoidCollisions(ObstacleList &otherObstacles);
		bool Collide(Scene::Object *obj);
		bool Collide(Scene::Object *obj, float width, float depth);

		float GetDirection();

	private:

		float x, y, z, d, dist, xl, width, depth, jump, dir;
	};

	
}


#endif // #ifndef F3DB_OBSTACLE_H