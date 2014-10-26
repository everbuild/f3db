/*----------------------------------------------------------------------------*\
  MODULE NAME: leafshadow.cpp
      PROJECT: F3DB
       AUTHOR: Evert Bauwens
         DATE: september 8 2003
  DESCRIPTION: 
\*----------------------------------------------------------------------------*/
#include "leafshadow.h"


namespace F3DB {

	
	LeafShadow::LeafShadow(Scene::Class *cls, Leaf *leaf): Object(cls) {
		this->leaf = leaf;
	}


	void LeafShadow::Update(double time) {
		Object::Update(time);
		D3DXVECTOR3 v = leaf->GetPosition();
		v.x += F3DB_LEAF_SHADOW_DX;
		v.y -= F3DB_LEAF_SHADOW_DEPTH;
		SetPosition(v);
		SetVisible(leaf->IsVisible());
	};

	
}
