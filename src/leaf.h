/*----------------------------------------------------------------------------*\
  MODULE NAME: leaf.h
      PROJECT: F3DB
       AUTHOR: Evert Bauwens
         DATE: september 8 2003
  DESCRIPTION: 
\*----------------------------------------------------------------------------*/
#ifndef F3DB_LEAF_H
#define F3DB_LEAF_H
#include "obstacle.h"


#define F3DB_LEAF_X 31.0f
#define F3DB_LEAF_Y -0.79f
#define F3DB_LEAF_Z 23.0f
#define F3DB_LEAF_DIST 62.0f


namespace F3DB {


	class Leaf;
	typedef list<Leaf*> LeafList;


	class Leaf: public Obstacle {
		
	public:

		bool hasFrog;
		
		Leaf(Scene::Class *cls);

		void AvoidCollisions(LeafList &leaves);
	};

	
}


#endif // #ifndef F3DB_LEAF_H