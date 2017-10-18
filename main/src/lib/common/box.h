#pragma once

#include "../dxtk/pch.h"
#include "Box2D/Box2D.h"

using gdVec1 = float;
using gdVec2 = DirectX::SimpleMath::Vector2;
using gdVec3 = DirectX::SimpleMath::Vector3;
using gdVec4 = DirectX::SimpleMath::Vector4;

class gdBox {
private:
	gdVec4 rect_var = gdVec4(0.0f, 0.0f, 0.0f, 0.0f);
	gdVec2 position_var = gdVec2(0.0f, 0.0f);
	gdVec1 angle_var = 0.0f;

public:
	// constructor
	gdBox();
	gdBox(gdVec2 set_size, gdVec2 set_position = gdVec2(0.0f, 0.0f), gdVec1 set_angle = 0.0f);
	gdBox(gdVec4 set_rect, gdVec1 set_angle = 0.0f);
	// rect
	inline void rect(gdVec4 set_rect) {
		rect_var = set_rect;
		position_var.x = (rect_var.z + rect_var.x) / 2.0f;
		position_var.y = (rect_var.w + rect_var.y) / 2.0f;
	}
	inline void rect(gdVec1 x, gdVec1 y, gdVec1 z, gdVec1 w) {
		rect(gdVec4(x, y, z, w));
	}
	inline gdVec4 rect() {
		return rect_var;
	}
	// position
	inline void position(gdVec2 set_pos) {
		rect_var.x += (set_pos.x - position_var.x);
		rect_var.y += (set_pos.y - position_var.y);
		rect_var.z += (set_pos.x - position_var.x);
		rect_var.w += (set_pos.y - position_var.y);
		position_var = set_pos;
	}
	inline void position(gdVec1 x, gdVec1 y) {
		position(gdVec2(x, y));
	}
	inline gdVec2 position() {
		return position_var;
	}
	// size
	inline void size(gdVec2 set_size) {
		rect_var.z = rect_var.x + set_size.x;
		rect_var.w = rect_var.y + set_size.y;
		position_var.x = (rect_var.z + rect_var.x) / 2.0f;
		position_var.y = (rect_var.w + rect_var.y) / 2.0f;
	}
	inline void size(gdVec1 x, gdVec1 y) {
		size(gdVec2(x, y));
	}
	inline gdVec2 size() {
		return gdVec2(rect_var.z - rect_var.x, rect_var.w - rect_var.y);
	}
	// angle
	inline void angle(gdVec1 x) {
		angle_var = x;
	}
	inline gdVec1 angle() {
		return angle_var;
	}
	// for Box2d
	inline void b2position(b2Vec2 set_position) { position(set_position.x, set_position.y); }
	inline void b2size(b2Vec2 set_size) { size(set_size.x, set_size.y); }
	inline b2Vec2 b2position() { return b2Vec2(position_var.x, position_var.y); }
	inline b2Vec2 b2size() { return b2Vec2(size().x, size().x); }
};