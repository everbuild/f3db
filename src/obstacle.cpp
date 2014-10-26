/*----------------------------------------------------------------------------*\
  MODULE NAME: obstacle.cpp
      PROJECT: F3DB
       AUTHOR: Evert Bauwens
         DATE: september 8 2003
  DESCRIPTION: Base class for leaf and car.
\*----------------------------------------------------------------------------*/
#include "obstacle.h"


namespace F3DB {

	
	Obstacle::Obstacle(Scene::Class *cls, float x, float y, float z, float dist, float width, float depth): Object(cls) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->width = width;
		this->depth = depth;
		xl = x - dist;
		SetVisible(false);
		started = false;
		lane = 0;
		jump = 0.0;
	}



	void Obstacle::Start(uint8 lane, float speed) {
		this->lane = lane;
		bool left = speed > 0.0f;
		d = left ? speed : -speed;
		SetPosition(D3DXVECTOR3(left ? -x : x, y, z + lane*2.0f));
		SetMovement(D3DXVECTOR3(speed, 0.0f, 0.0f));
		SetVisible(true);
		started = true;
		jump = left ? -width : width;
		dir = speed > 0 ? 1.0f : -1.0f;
	}



	void Obstacle::Stop() {
		SetVisible(false);
		started = false;
	}



	void Obstacle::Update(double time) {
		Object::Update(time);
		if(!started) return;
		float x = GetPosition().x;
		if(x < xl || x > this->x) Stop();
	}

	
	void Obstacle::AvoidCollisions(ObstacleList &otherObstacles) {
		if(!started) return;
		for(ObstacleList::iterator i = otherObstacles.begin(); i != otherObstacles.end();) {
			if(this != *i) if(Collide(*i)) {
				AddPosition(D3DXVECTOR3(jump, 0.0, 0.0));
				i = otherObstacles.begin();
				continue;
			}
			i ++;
		}
	}


	bool Obstacle::Collide(Scene::Object *obj, float width, float depth) {
		D3DXVECTOR3 p1, p2;
		if(!IsVisible() || !obj->IsVisible()) return false;
		p1 = GetGlobalPosition();
		p2 = obj->GetGlobalPosition();
		if(fabs(p2.z - p1.z) > ((this->depth + depth)/2.0f)) return false;
		if(fabs(p2.x - p1.x) > ((this->width + width)/2.0f)) return false;
		return true;
	}


	bool Obstacle::Collide(Scene::Object *obj) {
		return Collide(obj, width, depth);
	}


	float Obstacle::GetDirection() {
		return dir;
	}
}
