#pragma once

class Error {
private:
	static std::string error;

public;
	static void setError(std::string e);
	static std::string getError();
};

//class Box {
//private:
//	using namespace DirectX::SimpleMath;
//
//public:
//	Vector4 rect;
//	Vector2 position;
//	float angle;
//};
//
//class Obj {
//private:
//	float mul;
//	b2World *world;
//	b2BodyDef bodyDef;
//	b2PolygonShape bodySize;
//	b2FixtureDef fixtureDef;
//	b2Body *body;
//	b2Vec position;
//	float angle;
//	Box box;
//
//public:
//	Obj();
//	Obj(...);
//	void Set(...);
//	void upload();
//};
//
//class Motion {
//private:
//	static b2World *world;
//	static b2Vec gravity;
//	static std::vector<Obj> obj;
//
//public:
//	static init();
//	static add(..);
//	static upload();
//};

//class Texture {
//private:
//	ID3D11Device *device;
//	ID3D11DeviceContext *deviceContext;
//	ID3D11Resource *resource;
//	ID3D11Texture2D *texture;
//	ID3D11ShaderResourceView *shaderResourceView;
//	DirectX::SpriteBatch* spriteBatch;
//	ID3D11BlendState *CoverBlend;
//	Box box;
//	HRESULT err;
//
//public:
//	Texture();
//	Texture(...);
//	bool upload(...);
//	bool render();
//};
//
//class Capture {
//private:
//	HWND hwnd;
//	Texture screen;
//	Texture screenBackup;
//	Box size;
//
//public:
//	Capture();
//	Capture(...);
//	bool update(...);
//	bool render();
//};

class Icon {
private:
	Capture cap;
	std::vector<Obj> icon;
};

class Message {
private:
	struct MSG {
		HWND hwnd;
		UINT message;
		WPARAM wParam;
		LPARAM lParam;
	};
	static std::vector<MSG> buffer;
	static std::vector<MSG> message;

public:
	static void add(...);
	static inline void updata();
	static inline unsigned int getSize();
	static inline MSG getMessage(unsigned int index);
};

class Mouse {
	// だいたいそのまま
}

class Keyboard {
	// だいたいそのまま
}

class WinMessage {
	// マウスやキーボード以外のウィンドウイベントを全て処理
}

class DXTK {
public:
	static void create(...);
	static inline void getDevice();
	static inline void getDeviceContext();
}

class WindowManager {
private:
	static HWND desktop;
	static HWND wallpaper;
	static std::vector<HWND> taskbar;

public:
	static bool updata();
	static inline HWND getDesktop();
	static inline HWND getWallpaper();
	static inline std::vrctor<HWND> getTaskbar();
}