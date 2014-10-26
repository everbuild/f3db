/*----------------------------------------------------------------------------*\
  MODULE NAME: leaf.cpp
      PROJECT: F3DB
       AUTHOR: Evert Bauwens
         DATE: september 8 2003
  DESCRIPTION: 
\*----------------------------------------------------------------------------*/
#include "leaf.h"


namespace F3DB {

	
	Leaf::Leaf(Scene::Class *cls): Obstacle(cls, F3DB_LEAF_X, F3DB_LEAF_Y, F3DB_LEAF_Z, F3DB_LEAF_DIST, 2.1f, 1.9f) {
		hasFrog = false;
	}


	void Leaf::AvoidCollisions(LeafList &leaves) {
		ObstacleList obstacles;
		for(LeafList::iterator i = leaves.begin(); i != leaves.end(); i ++) obstacles.push_back(*i);
		Obstacle::AvoidCollisions(obstacles);
	}

	
}
