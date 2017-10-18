#include "world.h"

bool gdWorld::init(float size_x, float size_y) {
	// 代入
	size.size(size_x, size_y);
	rate = max_size / size.size().y; // Box2Dの空間の縦軸最大値をmax_sizeに設定					
	gravity = b2Vec2(0.0, 9.8f); // 重力設定
	// 空間生成
	world = new b2World(gravity);
	// 壁生成
	left_wall = std::unique_ptr<gdObj>(new gdObj);
	top_wall = std::unique_ptr<gdObj>(new gdObj);
	right_wall = std::unique_ptr<gdObj>(new gdObj);
	bottom_wall = std::unique_ptr<gdObj>(new gdObj);
	left_wall->init(this, gdBox({ 10.0f,size.size().y }, { 0.0f - 5.0f, size.size().y / 2.0f }), false);
	top_wall->init(this, gdBox({ size.size().x,10.0f }, { size.size().x / 2.0f, 0.0f - 5.0f }), false);
	right_wall->init(this, gdBox({ 10.0f,size.size().y }, { size.size().x + 5.0f, size.size().y / 2.0f }), false);
	bottom_wall->init(this, gdBox({ size.size().x,10.0f }, { size.size().x / 2.0f, size.size().y + 5.0f }), false);
	return 0;
}
bool gdWorld::init(HWND hwnd) {
	RECT rc;
	GetWindowRect(hwnd, &rc);
	return init(rc.right - rc.left, rc.bottom - rc.top);
}
void gdWorld::update(bool stop) {
	// 現在のフレームレート取得
	now_time = timeGetTime();
	if (before_time == 0) fps = 120.0;
	else fps = 1000.0 / (double)(now_time - before_time);
	before_time = now_time;
	// Box2D更新
	if (!stop) {
		world->Step(
			(float32)(1.0 / fps),
			(int32)(fps),
			2
		);
	}
}
void gdWorld::setGravity(float x, float y) {
	world->SetGravity(rate * b2Vec2(x, y));
}
void gdWorld::resetGravity() {
	world->SetGravity(gravity);
}