#pragma once

#include "../icon/Icons.h"
#include "../Box2D/world.h"
#include "imageIcon.h"

class gdImageIcons {
private:
	gdCapture capture;
	gdIcons icons;
	gdWorld world;
	std::vector<gdImageIcon> obj;
	std::vector<gdObj> wall;
	gdBox WorldRect;
	float mostNear = 0.0f;
	unsigned int mostNearIndex = -1;
	unsigned int drugIndex = -1;
	double start_time = 1.0; // アニメーションが開始されるまでの時間(s)
	double time = 0.0; // アニメーションが開始されるまでの間の経過時間(s)
	bool exitFlag = 0; // 終了フラグ
	double exitAnimation1 = 0.0; // 終了アニメーション(0<=exitAnimation1<=1)(y=x)
	double exitAnimation2 = 0.0; // 終了アニメーション(0<=exitAnimation2<=1)(y=1-(x-1)^2)
	DWORD exitTime = 450; // 終了アニメーションにかける時間(ms単位)
	void mouseEvent() {
		// カーソルがアイコン上にあるとき、マウスから一番距離の近いアイコン取得
		mostNearIndex = -1;
		mostNear = 0.0f;
		for (size_t i = 0; i < obj.size(); i++) {
			if (obj[i].getOnMouse()) {
				if (mostNearIndex == -1) {
					mostNearIndex = i;
					mostNear = obj[i].getLocalMouse().Length();
				}
				if (obj[i].getLocalMouse().Length() < mostNear) {
					mostNearIndex = i;
					mostNear = obj[i].getLocalMouse().Length();
				}
			}
		}
		if (mostNearIndex != -1) {
			// Hot
			obj[mostNearIndex].getIcon()->hot();
			// SelectとFocus
			if (Mouse::getClickB().l) {
				if ((!Keyboard::getShift().a) && (!Keyboard::getCtrl().a)) icons.unselect();
				if (Keyboard::getCtrl().a) obj[mostNearIndex].getIcon()->select(NULL, LVIS_FOCUSED | LVIS_SELECTED);
				else obj[mostNearIndex].getIcon()->select(LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
			}
			// 左クリック追従アイコンのインデックス指定
			if (Mouse::getClickB().l) drugIndex = mostNearIndex;
		}
		else
		{
			// Unhot
			icons.unhot();
			// UnselectとUnfocus
			if (Mouse::getClickB().l && (!Keyboard::getShift().a) && (!Keyboard::getCtrl().a)) icons.unselect();
		}
		// 左クリック追従アイコンの解放
		if (Mouse::getClickC().l && (drugIndex != -1)) drugIndex = -1;
		// 重力リセット
		if (Mouse::getClickC().l && (drugIndex == -1)) world.resetGravity();
		// 左クリック追従
		if (drugIndex != -1) obj[drugIndex].getObj()->come(Mouse::getCursor().x, Mouse::getCursor().y);
		// 重力を画面の中心からカーソル方向の向きにする
		if ((drugIndex == -1) && Mouse::getClickA().l) {
			gdVec2 vec = gdVec2(
				Mouse::getCursor().x - (world.getBox().size().x / 2.0f),
				Mouse::getCursor().y - (world.getBox().size().y / 2.0f)
			);
			vec /= WorldRect.size().y / 2.0f;
			vec *= (vec.Length() > 1.0f) ? (1.0f / vec.Length()) : 1.0f;
			vec *= (world.getDefaultGravity().Length() / world.getRate());
			world.setGravity(
				vec.x,
				vec.y
			);
		}
		// 右クリック追従
		if (Mouse::getClickA().r) {
			for (size_t i = 0; i < obj.size(); i++) {
				if (obj[i].getIcon()->getSelect()) obj[i].getObj()->spring(Mouse::getCursor().x, Mouse::getCursor().y);
			}
		}
		// アイコン左ダブルクリック時
		if (Mouse::getDoubleClick().l && (mostNearIndex != -1)) {
			// ファイル実行
			PostMessage(
				icons.getHwnd(),
				WM_LBUTTONDBLCLK,
				0,
				MAKELPARAM(
					obj[mostNearIndex].getIcon()->ico.position().x,
					obj[mostNearIndex].getIcon()->ico.position().y
				)
			);
		}
	}
	bool keyEvent() {
		if (Keyboard::getEsc().b) exitFlag = 1;
		if (exitFlag) {
			if (exitAnimation1 == 1.0) return 1;
			exitAnimation1 += 1000.0 / (world.getFps() * (double)exitTime);
			if (exitAnimation1 > 1.0) {
				exitAnimation1 = 1.0;
			}
			exitAnimation2 = exitAnimation1*(2.0 - exitAnimation1);
			for (size_t i = 0; i < obj.size(); i++) {
				obj[i].setOrigin(exitAnimation2);
			}
		}
		return 0;
	}

public:
	~gdImageIcons() {
		for (size_t i = 0; i < obj.size(); i++) {
			obj[i].release();
		}
		for (size_t i = 0; i < wall.size(); i++) {
			wall[i].release();
		}
	}
	bool init(HWND hwnd, ID3D11DeviceContext* deviceContext, double set_start_time = 1.0) {
		start_time = set_start_time;
		RECT rc;
		GetWindowRect(hwnd, &rc);
		WorldRect = gdGeneral::RECT2gdBox(rc);
		obj.clear();
		capture.init(deviceContext, hwnd);
		icons.init(hwnd);
		world.init(hwnd);
		update();
		return 0;
	}
	void setWallObj(RECT rc) {
		// オブジェクト生成
		wall.push_back(gdObj());
		wall.back().init(&world, gdGeneral::RECT2gdBox(rc), false);
	}
	void setWallObj(HWND objHwnd) {
		RECT rc;
		GetWindowRect(objHwnd, &rc);
		rc.left -= WorldRect.rect().x;
		rc.top -= WorldRect.rect().y;
		rc.right -= WorldRect.rect().x;
		rc.bottom -= WorldRect.rect().y;
		setWallObj(rc);
	}
	bool update(bool stop = false) {
		if (time <= start_time) {
			time += 1.0f / getWorld()->getFps();
			stop = true;
		}
		// アイコン情報更新
		if (icons.update()) return 1;
		// デスクトップのアイコン数が増減していた場合
		if (obj.size() != icons.icon.size()) {
			// リセット
			for (size_t i = 0; i < obj.size(); i++) {
				obj[i].release();
			}
			obj = std::vector<gdImageIcon>(icons.icon.size());
			for (size_t i = 0; i < obj.size(); i++) {
				obj[i].init(&world, &icons.icon[i], &capture);
			}
		}
		// Box2d更新
		world.update(stop);
		// imageIcon更新
		for (size_t i = 0; i < obj.size(); i++) {
			// imageIcon更新
			obj[i].update();
		}
		// マウスイベント処理
		mouseEvent();
		// キーイベント処理
		if (keyEvent()) return 1;
		// 処理終了
		return 0;
	}
	bool render() {
		for (int i = 0; i < obj.size(); i++) {
			if (obj[i].render()) return 1;
		}
		return 0;
	}
	inline gdWorld* getWorld() {
		return &world;
	}
};