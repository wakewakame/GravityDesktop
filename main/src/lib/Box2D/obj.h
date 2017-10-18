#pragma once

#include "../common/box.h"
#include "../common/general.h"
#include "Box2D/Box2D.h"

#include "../Win32/Debug.h"

#include "world.h"
class gdWorld;

class gdObj {
private:
	float rate = 1.0f;
	gdWorld *gd_world = nullptr;
	b2World *world = nullptr;
	b2BodyDef bodyDef;
	b2PolygonShape bodySize;
	b2FixtureDef fixtureDef;
	b2Body *body = nullptr;
	b2Vec2 position = b2Vec2(0.0f, 0.0f);
	float angle = 0.0f;
	gdBox size;

public:
	void init(gdWorld *set_world, gdBox set_size, bool dynamic = true);
	virtual ~gdObj();
	void come(float x, float y);
	void spring(float x, float y);
	void update();
	void release();
	inline gdBox getBox() { 
		return size; 
	}
};