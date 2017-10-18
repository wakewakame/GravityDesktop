#pragma once

#include "../icon/Icons.h"
#include "../Box2D/World.h"

class gdImageIcon{
private:
	gdCapture *capture;
	gdIcon *icon;
	gdObj obj;
	gdBox image_ico;
	gdBox image_all;
	gdVec2 local_mouse;
	bool on_mouse;
	float origin = 0.0f;

public:
	bool init(gdWorld *world, gdIcon *set_icon, gdCapture *set_capture) {
		icon = set_icon;
		obj.init(world, set_icon->ico);
		image_ico = icon->ico;
		image_all = icon->all;
		capture = set_capture;
		return 0;
	}
	void update() {
		// obj更新
		obj.update();
		// image_ico更新
		image_ico = obj.getBox();
		image_ico.position(image_ico.position()*(1.0f-origin)+icon->ico.position()*(origin));
		image_ico.angle(image_ico.angle()*(1.0f - origin) + icon->ico.angle()*(origin));
		// image_all角度更新
		image_all.angle(image_ico.angle());
		// gap_vecを回転行列にかける
		gdVec2 gap_vec;
		gap_vec.x =
			(icon->all.position() - icon->ico.position()).x * std::cos(image_ico.angle()) -
			(icon->all.position() - icon->ico.position()).y * std::sin(image_ico.angle());
		gap_vec.y =
			(icon->all.position() - icon->ico.position()).x * std::sin(image_ico.angle()) +
			(icon->all.position() - icon->ico.position()).y * std::cos(image_ico.angle());
		// 計算
		image_all.position(image_ico.position() + gap_vec);
		// 相対カーソル座標算出
		double len = gdVec2(
			(Mouse::getCursor().x - (image_all.position().x)),
			(Mouse::getCursor().y - (image_all.position().y))
		).Length();
		float angle = std::atan2(
			Mouse::getCursor().y - image_all.position().y,
			Mouse::getCursor().x - image_all.position().x
		) - image_all.angle();
		local_mouse.x = std::cos(angle)*len;
		local_mouse.y = std::sin(angle)*len;
		// カーソルがアイコン上にあるか判定
		on_mouse =
			std::abs(local_mouse.x) <= icon->all.size().x / 2.0f &&
			std::abs(local_mouse.y) <= icon->all.size().y / 2.0f;
	}
	bool render() {
		RECT rc = gdGeneral::gdBox2RECT(icon->all.rect());
		capture->getSpriteBatch()->Begin();
		capture->getSpriteBatch()->Draw(
			capture->getResourceView(), // テクスチャ
			image_ico.position(), // 描画の中心座標
			&rc, // 描画範囲
			DirectX::Colors::White, // スプライトを着色する色
			image_ico.angle(), // 回転
			image_ico.size() / 2.0 // 画像の中心軸座標
		);
		capture->getSpriteBatch()->End();
		return 0;
	}
	void release() {
		obj.release();
	}
	inline gdVec2 getLocalMouse() {
		return local_mouse;
	}
	inline bool getOnMouse() {
		return on_mouse;
	}
	inline gdIcon* getIcon() {
		return icon;
	}
	inline gdObj* getObj() {
		return &obj;
	}
	inline void setOrigin(float set_origin) {
		origin = set_origin;
	}
};