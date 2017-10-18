#pragma once

#include <vector>
#include <chrono>
#include "../common/box.h"
#include "Box2D/Box2D.h"

#include "obj.h"

#include "../Win32/Debug.h"

class gdObj;

class gdWorld {
private:
	float max_size = 5.0f; // Box2Dの最大空間サイズ
	float rate = 1.0f; // Box2D空間単位/ユーザー空間単位
	DWORD now_time = 0; // フレームレート取得用の現在時刻(ms単位)
	DWORD before_time = 0; // フレームレート取得用の1フレーム前時刻(ms単位)
	double fps = 60.0; // 現在のフレームレート
	b2World *world = nullptr; // Box2D空間
	gdBox size; // ユーザー空間サイズ
	b2Vec2 gravity = b2Vec2(0.0f, 9.8f); // 重力設定
	std::unique_ptr<gdObj> left_wall; // 左壁
	std::unique_ptr<gdObj> top_wall; // 上壁
	std::unique_ptr<gdObj> right_wall; // 右壁
	std::unique_ptr<gdObj> bottom_wall; // 下壁

public:
	bool init(float size_x, float size_y);
	bool init(HWND hwnd);
	void update(bool stop = 0);
	void setGravity(float x, float y);
	void resetGravity();
	inline b2World* getWorld() {
		return world;
	}
	inline float getRate() { return rate; }
	inline double getFps() { return fps; }
	inline gdBox getBox() { return size; }
	inline b2Vec2 getDefaultGravity() { return gravity; };
};