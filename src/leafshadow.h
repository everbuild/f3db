/*----------------------------------------------------------------------------*\
  MODULE NAME: leafshadow.h
      PROJECT: F3DB
       AUTHOR: Evert Bauwens
         DATE: september 8 2003
  DESCRIPTION: 
\*----------------------------------------------------------------------------*/
#ifndef F3DB_LEAFSHADOW_H
#define F3DB_LEAFSHADOW_H
#include "leaf.h"

#define F3DB_LEAF_SHADOW_DEPTH 2.2f
#define F3DB_LEAF_SHADOW_DX 1.0f


namespace F3DB {


	class LeafShadow;
	typedef list<LeafShadow*> LeafShadowList;


	class LeafShadow: public Scene::Object {
		
	public:

		LeafShadow(Scene::Class *cls, Leaf *leaf);

		void Update(double time);

	private:

		Leaf *leaf;
	};

	
}


#endif // #ifndef F3DB_LEAFSHADOW_H