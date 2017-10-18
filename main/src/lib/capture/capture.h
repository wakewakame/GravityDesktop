#pragma once

#include "../dxtk/pch.h"

using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Details;

class gdCapture {
protected:
	HWND hwnd;
	SIZE size;
	ID3D11Device *device;
	ID3D11DeviceContext *deviceContext;
	ID3D11Resource *resource;
	ID3D11Texture2D *renderTarget;
	ID3D11Texture2D *renderTargetBackup;
	ID3D11ShaderResourceView *shaderResourceView;
	ID3D11ShaderResourceView *shaderResourceViewBackup;
	DirectX::SpriteBatch* spriteBatch;
	ID3D11BlendState *CoverBlend;
	HRESULT err;
	std::string error_text;

	// ウィンドウキャプチャ関数
	using PfnDwmGetDxSharedSurface = BOOL(WINAPI*)(HWND, HANDLE*, UINT64*, DXGI_FORMAT*, DWORD*, UINT64*);

	void DXRelease() {
		if (resource != NULL) resource->Release();
		if (renderTarget != NULL) renderTarget->Release();
		if (renderTargetBackup != NULL) renderTargetBackup->Release();
		if (spriteBatch != NULL) delete spriteBatch;
		if (CoverBlend != NULL) CoverBlend->Release();
	}

	void getDXError() {
		switch (err) {
		case D3D11_ERROR_FILE_NOT_FOUND:
			error_text += "(D3D11_ERROR_FILE_NOT_FOUND)";
			break;
		case D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS:
			error_text += "(D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS)";
			break;
		case D3D11_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS:
			error_text += "(D3D11_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS)";
			break;
		case D3D11_ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD:
			error_text += "(D3D11_ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD)";
			break;
			//case D3DERR_INVALIDCALL:
			error_text += "(D3DERR_INVALIDCALL)";
			break;
			//case D3DERR_WASSTILLDRAWING:
			error_text += "(D3DERR_WASSTILLDRAWING)";
			break;
		case E_FAIL:
			error_text += "(E_FAIL)";
			break;
		case E_INVALIDARG:
			error_text += "(E_INVALIDARG)";
			break;
		case E_OUTOFMEMORY:
			error_text += "(E_OUTOFMEMORY)";
			break;
		case S_FALSE:
			error_text += "(S_FALSE)";
			break;
		case S_OK:
			error_text += "(S_OK)";
			break;
		default:
			error_text += "(unknow error)";
			break;
		}
	}

public:
	gdCapture() {
		size.cx = 0;
		size.cy = 0;
		resource = NULL;
		renderTarget = NULL;
		renderTargetBackup = NULL;
		shaderResourceView = NULL;
		shaderResourceViewBackup = NULL;
		spriteBatch = NULL;
		CoverBlend = NULL;
		err = 0;
		error_text = "";
	}
	~gdCapture() {
		// 変数開放
		DXRelease();
	}
	bool init(ID3D11DeviceContext* set_deviceContext, HWND set_hwnd) {
		// 変数初期化
		hwnd = set_hwnd;
		deviceContext = set_deviceContext;
		deviceContext->GetDevice(&device);
		HANDLE phSurface; // ウィンドウの表面への共有ハンドル
		// ウィンドウサイズ取得
		RECT rc;
		GetWindowRect(hwnd, &rc);
		size.cx = rc.right - rc.left;
		size.cy = rc.bottom - rc.top;
		// グラフィックスアダプタ取得
		ComPtr<IDXGIFactory> factory;
		ComPtr<IDXGIAdapter> adapter;
		DXGI_ADAPTER_DESC desc{};
		UINT64 adapterLuid;
		if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)factory.GetAddressOf()))) {
			error_text += "CreateDXGIFactory fuynction Error";
			return 1;
		}
		if (FAILED(factory->EnumAdapters(0, adapter.GetAddressOf()))) {
			error_text += "IDXGIFactory::EnumAdapters function Error";
			return 1;
		}
		if (FAILED(adapter->GetDesc(&desc))) {
			error_text += "IDXGIAdapter::GetDesc Error";
			return 1;
		}
		adapterLuid = desc.AdapterLuid.HighPart;
		adapterLuid <<= 32;
		adapterLuid |= desc.AdapterLuid.LowPart;
		// 関数取得
		HMODULE user32dll;
		PfnDwmGetDxSharedSurface getSurface;
		user32dll = LoadLibrary(TEXT("user32.dll"));
		if (user32dll == NULL) {
			error_text += "user32.dll Load Error";
			return 1;
		}
		getSurface = (PfnDwmGetDxSharedSurface)GetProcAddress(user32dll, "DwmGetDxSharedSurface");
		if (getSurface == NULL) {
			error_text += "GetProcAddress \"DwmGetDxSharedSurface\" function Error";
			FreeLibrary(user32dll);
			return 1;
		}
		// 関数実行
		DXGI_FORMAT pFmtWindow;;
		ULONG pPresentFlags;
		ULONGLONG pWin32kUpdateId;
		phSurface = nullptr;
		pPresentFlags = 0;
		pWin32kUpdateId = 0;
		if (getSurface(hwnd, &phSurface, &adapterLuid, &pFmtWindow, &pPresentFlags, &pWin32kUpdateId) == 0) {
			error_text += "DwmGetDxSharedSurface function Error";
			FreeLibrary(user32dll);
			return 1;
		}
		// dll開放
		FreeLibrary(user32dll);
		// 共有リソースにアクセスできるようにする
		err = device->OpenSharedResource(phSurface, IID_ID3D11Resource, (void**)(&resource));
		if (FAILED(err))
		{
			error_text += "ID3D11Device::OpenSharedResource function Error";
			getDXError();
			DXRelease();
			return 1;
		}
		// 取得したリソースをテクスチャに変換
		err = resource->QueryInterface(IID_ID3D11Texture2D, (void**)(&renderTarget));
		if (FAILED(err))
		{
			error_text += "ID3D11Resource::QueryInterface function Error";
			getDXError();
			DXRelease();
			return 1;
		}
		// テクスチャのバックアップ生成
		D3D11_TEXTURE2D_DESC renderTargetDesk;
		renderTarget->GetDesc(&renderTargetDesk);
		device->CreateTexture2D(&renderTargetDesk, NULL, &renderTargetBackup);
		deviceContext->CopyResource(renderTargetBackup, renderTarget);
		// シェーダリソースビューの生成
		err = device->CreateShaderResourceView(renderTarget, NULL, &shaderResourceView);
		if (FAILED(err)) {
			error_text += "ID3D11Device::CreateShaderResourceView function Error";
			getDXError();
			DXRelease();
			return 1;
		}
		// バックアップシェーダリソースビューの生成
		err = device->CreateShaderResourceView(renderTargetBackup, NULL, &shaderResourceViewBackup);
		if (FAILED(err)) {
			error_text += "ID3D11Device::CreateShaderResourceView function Error";
			getDXError();
			DXRelease();
			return 1;
		}
		// スプライトの生成
		spriteBatch = new DirectX::SpriteBatch(deviceContext);
		// カバーブレンド生成
		D3D11_BLEND_DESC BlendDesc;
		ZeroMemory(&BlendDesc, sizeof(BlendDesc));
		BlendDesc.AlphaToCoverageEnable = FALSE;
		BlendDesc.IndependentBlendEnable = FALSE;
		for (int i = 0; i < 8; i++) {
			BlendDesc.RenderTarget[i].BlendEnable = TRUE;
			BlendDesc.RenderTarget[i].SrcBlend = D3D11_BLEND_ONE;
			BlendDesc.RenderTarget[i].DestBlend = D3D11_BLEND_ZERO;
			BlendDesc.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
			BlendDesc.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
			BlendDesc.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ONE;
			BlendDesc.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			BlendDesc.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		}
		device->CreateBlendState(&BlendDesc, &CoverBlend);
		// 処理終了
		return 0;
	}
	void render(bool cover) {
		if (cover) {
			spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, nullptr, nullptr, nullptr, nullptr, [=] {
				float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
				deviceContext->OMSetBlendState(CoverBlend, blendFactor, 0xffffffff);
			});
		}
		else {
			spriteBatch->Begin();
		}
		spriteBatch->Draw(
			shaderResourceView, // テクスチャ
			DirectX::SimpleMath::Vector2(0.0, 0.0), // 描画の中心座標
			DirectX::Colors::White // スプライトを着色する色
		);
		spriteBatch->End();
	}
	void renderBackup(bool cover) {
		if (cover) {
			spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, nullptr, nullptr, nullptr, nullptr, [=] {
				float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
				deviceContext->OMSetBlendState(CoverBlend, blendFactor, 0xffffffff);
			});
		}
		else {
			spriteBatch->Begin();
		}
		spriteBatch->Draw(
			shaderResourceViewBackup, // テクスチャ
			DirectX::SimpleMath::Vector2(0.0, 0.0), // 描画の中心座標
			DirectX::Colors::White // スプライトを着色する色
		);
		spriteBatch->End();
	}
	inline HWND getHwnd() {
		return hwnd;
	}
	inline ID3D11Device* getDevice() {
		return device;
	}
	inline ID3D11DeviceContext* getDeviceContext() {
		return deviceContext;
	}
	inline ID3D11Resource* getResource() {
		return resource;
	}
	inline ID3D11Texture2D* getTexture() {
		return renderTarget;
	}
	inline ID3D11Texture2D* getTextureBackup() {
		return renderTargetBackup;
	}
	inline ID3D11ShaderResourceView* getResourceView() {
		return shaderResourceView;
	}
	inline ID3D11ShaderResourceView* getResourceViewBackup() {
		return shaderResourceViewBackup;
	}
	inline DirectX::SpriteBatch* getSpriteBatch() {
		return spriteBatch;
	}
	inline SIZE getSize() {
		return size;
	}
	inline std::string getError() {
		return error_text;
	}
};