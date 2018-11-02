//================================================
//	Direct3D9仕様ライブラリ[namaex3d.h]
//		AT12F75_28_生井龍希
//			2018 10 19
//================================================
#ifndef NAMAEX_H
#define NAMAEX_H

//===== インクルードファイル =====
#include <Windows.h>
#include <tchar.h>
#include <d3dx9.h>
#define DIRECTINPUT_VERSION (0x0800)
#define INITGUID
#include <dinput.h>
#include <XInput.h>
#include <vector>
#include <time.h>

using namespace std;

//===== プログラマコメント ======
#pragma	comment (lib,"d3d9.lib")
#pragma	comment (lib,"d3dx9.lib")
#pragma comment (lib,"dxguid.lib")
#pragma comment (lib,"dinput8.lib")
#pragma comment (lib,"xinput.lib")

//===== マクロ定義 =====
#define STRICT				// 型チェックを厳密に行う
#define WIN32_LEAN_AND_MEAN	// ヘッダーからあまり使わない関数を除く

#define RELEASE(x)	{ if(x) { (x)->Release(); (x) = NULL; } }	// 安全な開放

#define	NUM_KEY_MAX		(256)	// キーの最大数

#define WINDOW_STYLE	(WS_OVERLAPPEDWINDOW ^WS_THICKFRAME ^ WS_MAXIMIZEBOX ^ WS_MINIMIZEBOX)	// ウィンドウズクラスのスタイル
#define CLASS_NAME		_T("GameWindow")														// ウィンドウズクラスの名前

#define FVF_VERTEX	(D3DFVF_XYZ | D3DFVF_DIFFUSE)	//トランスフォームされた位置座標を含む頂点フォーマット

//===== クラス宣言 =====
class Application;	// アプリケーション
class Resource;		// リソース管理
class Input;		// 入力管理
class Scene;		// シーン
class Object;		// オブジェクト
class Component;	// コンポーネント
class Transform;	// 座標情報
class XMesh;		// メッシュ(.x)
class Sprite;		// スプライト

//===== クラス定義 =====
//----- アプリケーション -----
class Application {
public:
	/* WinMainからのアクセスを許可 */
	friend int APIENTRY _tWinMain(HINSTANCE, HINSTANCE, LPSTR, int);
	
	/* 頂点データ */
	struct Vertex {
		float x, y, z;
		D3DCOLOR color;
	};

	/* コンストラクとデストラクタ */
	Application(char* windowName, int width, int height, HINSTANCE hInstance, int nCmdShow);
	~Application(void) {};

	/* シーンの変更 */
	void SceneChange(int nextSceneNumber) { this->nextSceneNumber = nextSceneNumber; };

	/* シーンの追加 */
	void AddScene(Scene *);

	/* アプリケーションの実行 */
	void Play();

	/* ゲッター */
	LPDIRECT3DDEVICE9 GetDevice()	{ return pDevice; };
	Input* GetInput()				{ return pInput; };
	Scene* GetScenes(char* name);
	Scene* GetScenes()				{ return scenes[sceneNumber];};
	int GetSceneNumber()			{ return sceneNumber;};
	int GetScreenWidth()			{ return screenWidth; };
	int GetScreenHeight()			{ return screenHeight; };
private:
	HWND hWnd;					// ウィンドウハンドル
	int screenWidth;			// スクリーン横幅
	int screenHeight;			// スクリーン高さ

	LPDIRECT3D9 pD3D;			// Direct3Dポインタ
	LPDIRECT3DDEVICE9 pDevice;	// Direct3Dデバイスポインタ

	Input* pInput;	// 入力管理

	std::vector<Scene *> scenes;	// シーン
	int sceneNumber;				// 実行中のシーン番号
	int nextSceneNumber;			// 次に実行されるシーン番号

	/* デバイス関係成功確認処理 */
	void Check(HRESULT work,char* workName){
		if (FAILED(work)) {
			char message[256];
			sprintf(message, _T("%sに失敗しました"), workName);
			MessageBox(hWnd, message, _T("ERROR"), MB_ICONWARNING);
			exit(-1);
		}
	}
};

//----- 入力管理 -----
class Input {
public:
	XINPUT_STATE padState[4];
	int padStateTrigger[4];
	int padStateRelease[4];
	Input(HINSTANCE hInstance, HWND hWnd);
	~Input() {};

	void Update();

	bool Keyboard_IsPress(int key);
	bool Keyboard_IsTrigger(int key);
	bool Keyboard_IsRelease(int key);

private:
	BYTE	KeyState[NUM_KEY_MAX];			// キーボードの入力情報ワーク
	BYTE	KeyStateTrigger[NUM_KEY_MAX];	// キーボードのトリガー情報ワーク
	BYTE	KeyStateRelease[NUM_KEY_MAX];	// キーボードのリリース情報ワーク
	LPDIRECTINPUT8	pInput;			// 入力ポインタ
	LPDIRECTINPUTDEVICE8 pKeyboard;	// キーボードポインタ
};

//----- シーン -----
class Scene {
public:
	/* アクセスの許可 */
	friend void Application::AddScene(Scene *);

	Application *pApp;			// アプリケーション
	Resource *pSceneResource;	// シーンのリソース
	vector<Object*> objects;	// オブジェクト

	/* カメラ */
	struct Camera{
		D3DXVECTOR3 Pos, Look, At;
	};
	Camera camera;

	/* コンストラクトとデストラクタ */
	Scene(char *name);
	~Scene() {};

	/* 実施処理 */
	void Init();
	void Update();
	void Draw();
	void Uninit();

	/* オブジェクトの追加 */
	void AddObject(Object* pObject);

protected:
	static int sceneMax;	// シーンの数

	char* name;			// シーン名
	int sceneNumber;	// シーン番号

	/* シーンチェンジ */
	void SceneChange(char* nextSceneChange);
};

//----- オブジェクト -----
class Object {
public:
	int tag;		// オブジェクトのタグ
	Scene *pScene;	// シーンポインタ

	vector<Component*> components;	// コンポーネント
	Transform* pTransform;	// オブジェクトの位置情報

	Object();
	~Object() {};
	
	/* 仮想関数 */
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Uninit() = 0;

protected:
	bool KeybordPress(int key) { return pScene->pApp->GetInput()->Keyboard_IsPress(key); };
	bool KeybordTrigger(int key) { return pScene->pApp->GetInput()->Keyboard_IsTrigger(key); };
	bool KeybordRelease(int key) { return pScene->pApp->GetInput()->Keyboard_IsRelease(key); };

	int AddXMesh(const char* FileName);

	void DrawXMesh(int meshNum);
	void DrawXMesh(int meshNum, D3DXVECTOR3 position);
};

//----- コンポーネント -----
class Component {
public:
	void Init();
	void Update();
	void Draw();
	void Uninit();
};

//----- 位置情報 -----
class Transform {
public:
	D3DXVECTOR3 pos;		// 座標
	D3DXVECTOR3 scale;		// スケール
	D3DXVECTOR3 rotation;	// 角度

	Transform() {
		pos = D3DXVECTOR3(0,0,0);
		scale = D3DXVECTOR3(1,1,1);
		rotation = D3DXVECTOR3(0,0,0);
	}
};

//----- リソース管理 -----
class Resource {
public:
	Resource(Scene* pScene) { this->pScene = pScene; };
	~Resource() {};
	
	std::vector<XMesh*>		xMeshs;
	std::vector<Sprite*>	sprites;

	int AddXMesh(const char* FileName);
private:
	Scene *pScene;
};

//----- Xファイル -----
class XMesh {
public:
	LPD3DXMESH pMesh;

	Transform *pTransform;

	LPD3DXBUFFER pAdjacencyBuffer;
	DWORD		 MaterialMax;
	LPD3DXBUFFER pMaterialBuffer;

	D3DMATERIAL9*		pMaterial;
	LPDIRECT3DTEXTURE9*	pTexture;

	LPDIRECT3DDEVICE9 pDevice;

	XMesh(const char* FileName, LPDIRECT3DDEVICE9 pDevice){
		this->pDevice = pDevice;

		/* Xファイル読み込み */
		D3DXLoadMeshFromX(FileName, D3DXMESH_SYSTEMMEM, pDevice, &pAdjacencyBuffer, &pMaterialBuffer, NULL, &MaterialMax, &pMesh);

		/* メッシュ最適化 */
		pMesh->OptimizeInplace(D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE, (DWORD*)pAdjacencyBuffer->GetBufferPointer(), NULL, NULL, NULL);
		RELEASE(pAdjacencyBuffer);

		/* 法線ベクトルの書き込み */
		if (!(pMesh->GetFVF() & D3DFVF_NORMAL)) {
			ID3DXMesh* pTempMesh = NULL;
			pMesh->CloneMeshFVF(pMesh->GetOptions(),pMesh->GetFVF() | D3DFVF_NORMAL, pDevice, &pTempMesh);
			D3DXComputeNormals(pTempMesh, NULL);
			pMesh->Release();
			pMesh = pTempMesh;
		}

		/* マテリアル用メモリの確保 */
		pMaterial = new D3DMATERIAL9[MaterialMax];
		pTexture = new LPDIRECT3DTEXTURE9[MaterialMax];
		D3DXMATERIAL* Materials = (D3DXMATERIAL*)pMaterialBuffer->GetBufferPointer();
		
		/* マテリアル読み込み */
		for (unsigned int i = 0; i < MaterialMax; i++) {
			/* マテリアル複写 */
			pMaterial[i] = Materials[i].MatD3D;
			pMaterial[i].Ambient = pMaterial[i].Diffuse;
			
			/* テクスチャの読み込み*/
			pTexture[i] = NULL;			
			WCHAR textureFileName[MAX_PATH];
			MultiByteToWideChar(CP_ACP, 0, Materials[i].pTextureFilename, -1, textureFileName, MAX_PATH);
			textureFileName[MAX_PATH - 1] = '\0';
			D3DXCreateTextureFromFile(pDevice, (LPCSTR)textureFileName, &pTexture[i]);
		}
	};

	~XMesh() {};

	void Draw() {	
		
		/* ワールド行列作成 */
		D3DXMATRIX mat;
		D3DXMatrixTranslation(&mat,pTransform->pos.x, pTransform->pos.y, pTransform->pos.z);
		pDevice->SetTransform(D3DTS_WORLD, &mat);

		pDevice->SetVertexShader(NULL);
		pDevice->SetFVF(pMesh->GetFVF());
		for (unsigned int i = 0; i < MaterialMax; i++) {
			pDevice->SetMaterial(&pMaterial[i]);
			pDevice->SetTexture(0, pTexture[i]);
			pMesh->DrawSubset(i);
		}
	};
	
	void Draw(D3DXVECTOR3 position) {

		/* ワールド行列作成 */
		D3DXMATRIX mat;
		D3DXMatrixTranslation(&mat, position.x, position.y, position.z);
		pDevice->SetTransform(D3DTS_WORLD, &mat);

		pDevice->SetVertexShader(NULL);
		pDevice->SetFVF(pMesh->GetFVF());
		for (unsigned int i = 0; i < MaterialMax; i++) {
			pDevice->SetMaterial(&pMaterial[i]);
			pDevice->SetTexture(0, pTexture[i]);
			pMesh->DrawSubset(i);
		}
	};
};

//----- スプライト ------

//class Sprite {
//	Transform *pTransform;
//
//	LPDIRECT3DDEVICE9 pDevice;
//
//	int width;
//	int height;
//	int maxPattern;
//	int sidePattern;
//
//	Sprite(const char* FileName, int width, int height, int Max, int Side, LPDIRECT3DDEVICE9 pDevice) {
//		FileName->
//	};
//
//	~Sprite() {};
//
//	void Draw() {
//		/* テクスチャサイズ取得 */
//		float width = width;
//		float height = height;
//
//		/* テクスチャの横込範囲 */
//		int texWidth = width / pTexture->getPatternSide(index);
//		int texHeight = height / (pTexture->getPatternMax(index) / pTexture->getPatternSide(index));
//
//		/* テクスチャ座標計算 */
//		float u0 = (float)texX / width;
//		float v0 = (float)texY / height;
//		float u1 = (float)(texX + texWidth) / width;
//		float v1 = (float)(texY + texHeight) / height;
//
//		if (pTransform->scale.x < 0) {
//			float w = u0;
//			u0 = u1;
//			u1 = w;
//
//			pTransform->scale.x *= (float)-1;
//		}
//		if (pTransform->scale.y < 0) {
//			float w = v0;
//			v0 = v1;
//			v1 = w;
//
//			pTransform->scale.y *= (float)-1;
//		}
//
//		/* スケール計算 */
//		texWidth *= pTransform->scale.x;
//		texHeight *= pTransform->scale.y;
//
//		/* 回転計算 */
//		float px[4], py[4];
//		float halfWidth = texWidth	* 0.5f;
//		float halfHeight = texHeight * 0.5f;
//		px[0] = -halfWidth * cos(pTransform->rotation.z) - -halfHeight * sin(pTransform->rotation.z);
//		py[0] = -halfWidth * sin(pTransform->rotation.z) + -halfHeight * cos(pTransform->rotation.z);
//		px[1] = halfWidth * cos(pTransform->rotation.z) - -halfHeight * sin(pTransform->rotation.z);
//		py[1] = halfWidth * sin(pTransform->rotation.z) + -halfHeight * cos(pTransform->rotation.z);
//		px[2] = -halfWidth * cos(pTransform->rotation.z) - halfHeight * sin(pTransform->rotation.z);
//		py[2] = -halfWidth * sin(pTransform->rotation.z) + halfHeight * cos(pTransform->rotation.z);
//		px[3] = halfWidth * cos(pTransform->rotation.z) - halfHeight * sin(pTransform->rotation.z);
//		py[3] = halfWidth * sin(pTransform->rotation.z) + halfHeight * cos(pTransform->rotation.z);
//
//		/* ポリゴンの頂点の設定 */
//		VECTOR vertex2D[] = {
//			{ D3DXVECTOR4(px[0] + posX - 0.5f, py[0] + posY - 0.5f, 1.0f,1.0f), color, D3DXVECTOR2(u0, v0) },
//			{ D3DXVECTOR4(px[1] + posX - 0.5f, py[1] + posY - 0.5f, 1.0f,1.0f), color, D3DXVECTOR2(u1, v0) },
//			{ D3DXVECTOR4(px[2] + posX - 0.5f, py[2] + posY - 0.5f, 1.0f,1.0f), color, D3DXVECTOR2(u0, v1) },
//			{ D3DXVECTOR4(px[3] + posX - 0.5f, py[3] + posY - 0.5f, 1.0f,1.0f), color, D3DXVECTOR2(u1, v1) }
//		};
//
//		/* 仮想アドレス */
//		VECTOR *pV;
//
//		/* 頂点バッファのロック */
//		pVertexBuffer->Lock(0, 0, (void**)&pV, 0);
//		memcpy(pV, vertex2D, sizeof(vertex2D));
//		pVertexBuffer->Unlock();
//
//		/* ポリゴンの表示 */
//		pDevice->SetFVF(FVF_VERTEX);
//		pDevice->SetTexture(0, pTexture->getTexture(index));
//		pDevice->SetStreamSource(0, pVertexBuffer, 0, sizeof(VECTOR));
//		pDevice->SetIndices(pIndexBuffer);
//		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
//		}
//	};
//};

//===== 関数定義 =====

////////// ウィンドウプロシージャ //////////
/**
 * @brief	ウィンドウプロシージャ
 * @param	ウィンドウハンドル, メッセージ識別子, メッセージの最初のパラメータ, メッセージの2番目のパラメータ
 */
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	/* 閉じるボタンの処理 */
	case WM_CLOSE:
		if (MessageBox(hWnd,_T( "アプリケーションを終了しますか？"), "確認", MB_OKCANCEL | MB_DEFBUTTON2 == IDOK)) {
			DestroyWindow(hWnd);
		}
		return 0;

	/* プログラムの終了をシステムに通知 */
	case WM_DESTROY:
		PostQuitMessage(NULL);
		break;

	/* その他のイベントをシステムに処理させる */
	default:
		return (DefWindowProc(hWnd, msg, wParam, lParam));
	}
	return 0;
}

////////// アプリケーション //////////
/**
 * @brief	アプリケーションのコンストラクタ
 * @param	最初のシーン,ウィンドウの名前,ウィンドウ横幅,ウィンドウ高さ,インスタンスハンドル,ウィンドウ立ち上げ時の状態
 */
Application::Application(char* windowName, int width, int height, HINSTANCE hInstance, int nCmdShow) {

	/* 乱数ジェネレーターを初期化 */
	srand((unsigned)time(NULL));

	/* メンバの初期化 */
	sceneNumber = 0;
	nextSceneNumber = 0;

	/* スクリーンサイズの保存 */
	screenWidth		= width;
	screenHeight	= height;

	/* ウィンドウの初期位置を設定 */
	int window_x	= (GetSystemMetrics(SM_CXSCREEN) - width)	/ 2;
	int window_y	= (GetSystemMetrics(SM_CYSCREEN) - height)	/ 2;

	/* ウィンドウクラスの設定 */
	WNDCLASSEX wc	= { sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW, WndProc, 0, 0, hInstance, NULL, NULL,
		(HBRUSH)(COLOR_WINDOW + 1), NULL, CLASS_NAME, NULL };

	/* ウィンドウクラスの登録 */
	Check(RegisterClassEx(&wc) == NULL,"ウィンドウの作成");

	/* ウィンドウハンドルの設定 */
	hWnd = CreateWindow(CLASS_NAME, windowName, WINDOW_STYLE, window_x, window_y, width, height, NULL, NULL, hInstance, NULL);
	Check(hWnd != NULL, "ウィンドウハンドルの設定");

	/* Direct3Dの設定 */
	pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	Check(pD3D != NULL, "Direct3Dの設定");

	/* ディスプレイモードの設定 */
	D3DDISPLAYMODE d3ddm;
	Check(pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm), "ディスプレイモードの設定");

	/* プレゼンパラメータ */
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));	
	d3dpp.Windowed = true;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	/* デバイス作成 */
	Check(pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &pDevice), "Direct3DDeviceの作成");
	D3DLIGHT9 light;
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r = light.Diffuse.g = light.Diffuse.b = 1.0f;
	light.Specular.r = light.Specular.g = light.Specular.b = 0.2f;
	light.Ambient.r = light.Ambient.g = light.Ambient.b = 0.5f;
	light.Direction = D3DXVECTOR3(1, -2, 1);
	pDevice->SetLight(0, &light);
	pDevice->LightEnable(0, TRUE);

	/* レンダーの設定 */
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	pDevice->SetRenderState(D3DRS_DITHERENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	pDevice->SetRenderState(D3DRS_AMBIENT, 0x33333333);
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	
	/* 射影行列作成 */
	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(90.0f),(float)width/height,1.0f,100.0f);
	pDevice->SetTransform(D3DTS_PROJECTION, &matProj);

	/* 入力管理の初期化 */
	pInput = new Input(hInstance, hWnd);
}

/**
 * @brief	シーンの追加
 * @param	追加するシーンのポインタ
 */
void Application::AddScene(Scene *pScene) {
	scenes.push_back(pScene);
	pScene->pApp = this;
}

/**
 *	@brief	アプリケーションの実行
 */
void Application::Play() {
	/* メッセージの設定 */
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	/* ウィンドウ表示 */
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);

	/* 最初のシーンの初期化 */
	scenes[sceneNumber]->Init();
	printf("");

	/* アプリケーションループ */
	while(msg.message != WM_QUIT){
		/* ウィンドウへのイベントを取得 */
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			/* 仮想キーメッセージを文字メッセージへ変換 */
			TranslateMessage(&msg);
			/*ウィンドウへイベントを通達 */
			DispatchMessage(&msg);
		}
		else {
			/* 入力の更新 */
			pInput->Update();

			/* シーンの更新処理 */
			scenes[sceneNumber]->Update();

			/* 画面のクリア */
			pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 255, 255), 1.0f, 0);

			/* 描画バッチ命令の開始 */
			if (SUCCEEDED(pDevice->BeginScene())) {	

				/* ビュー行列作成 */
				D3DXMATRIXA16 matView;
				D3DXMatrixLookAtLH(&matView, &scenes[sceneNumber]->camera.Pos, &scenes[sceneNumber]->camera.Look, &scenes[sceneNumber]->camera.At);
				pDevice->SetTransform(D3DTS_VIEW, &matView);

				/* シーンの描画 */
				scenes[sceneNumber]->Draw();
				
				/* 描画バッチ命令の終了 */
				pDevice->EndScene();

				/* バックバッファをフリップ */
				pDevice->Present(NULL, NULL, NULL, NULL);

				/* 待機フレーム */
				Sleep(10);
			}

						
			/* シーンチェンジ */
			if (sceneNumber != nextSceneNumber) {
				scenes[sceneNumber]->Uninit();
				sceneNumber = nextSceneNumber;
				scenes[sceneNumber]->Init();
			}
		}
	}
}

/**
 *	@brief	シーンの取得
 *	@param	取得するシーンの名前
 *	@return	シーン番号、なければNULL
 */
Scene* Application::GetScenes(char* name) {
	int i;

	for (i = 0; i < scenes.size(); i++) {
		break;
	}

	if (i != scenes.size()) {
		return scenes[i];
	}

	return NULL;
}

////////// リソース //////////
/**
* @brief	リソースの初期化
* @param	読み込みファイルネーム
* @return	メッシュ番号
*/
int Resource::AddXMesh(const char* fileName) {
	XMesh* xMesh;
	xMesh = new XMesh(fileName, pScene->pApp->GetDevice());
	xMeshs.push_back(xMesh);
	return xMeshs.size() - 1;
}

///////// インプット //////////
/**
*	@brief	DirectInputの設定
*	@param	インスタンスハンドル
*/
Input::Input(HINSTANCE hInstance, HWND hWnd) {
	/* DirectInputオブジェクトの作成 */
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&pInput, NULL))) {
		MessageBox(hWnd, "DirectInputの設定に失敗しました", "ERROR", MB_ICONWARNING);
	};

	/* キーボードデバイスの作成 */
	if (FAILED(pInput->CreateDevice(GUID_SysKeyboard, &pKeyboard, NULL))) {
		MessageBox(hWnd, "キーボードの取得に失敗しました", "ERROR", MB_ICONWARNING);
	}

	/* データフォーマットを設定 */
	if (FAILED(pKeyboard->SetDataFormat(&c_dfDIKeyboard))) {
		MessageBox(hWnd, "キーボードのデータフォーマット設定に失敗しました", "ERROR", MB_ICONWARNING);
	}

	/* 協調モードを設定（フォアグラウンド＆非排他モード）*/
	if (FAILED(pKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))) {
		MessageBox(hWnd, "キーボードの協調モードをの設定に失敗しました", "ERROR", MB_ICONWARNING);
	}

	/* アクセス権を獲得(入力制御開始) */
	pKeyboard->Acquire();
}

/**
*	@brief	DirectInputの更新
*/
void Input::Update() {
	/* デバイスからデータを取得 */
	if (SUCCEEDED(pKeyboard->GetDeviceState(sizeof(KeyState), KeyState))) {
		for (int nCkey = 0; nCkey < NUM_KEY_MAX; nCkey++)
		{
			/* キートリガー・リリース情報を生成 */
			KeyStateTrigger[nCkey] = (KeyState[nCkey] ^ KeyState[nCkey]) & KeyState[nCkey];
			KeyStateRelease[nCkey] = (KeyState[nCkey] ^ KeyState[nCkey]) & KeyState[nCkey];
		}
	}
	else {
		/* キーボードへのアクセス権を取得 */
		pKeyboard->Acquire();
	}

	/* デバイスからデータを取得 */
	for (int i = 0;i < 4; i++) {
		int work = padState[i].Gamepad.wButtons;

		XInputGetState(i, &padState[i]);

		if (padState[i].Gamepad.wButtons != work) {
			padStateTrigger[i] = padState[i].Gamepad.wButtons;
			padStateRelease[i] = work;
		}
		else {
			padStateTrigger[i] = 0;
			padStateRelease[i] = 0;
		}
	}
}

/**
*	@brief	キーボードプレス情報受け渡し
*	@return	キーボードプレス情報
*/
bool Input::Keyboard_IsPress(int key) {
	return (KeyState[key] & 0x80) ? true : false;
}

/**
*	@brief	キーボードトリガー情報受け渡し
*	@return	キーボードトリガー情報
*/
bool Input::Keyboard_IsTrigger(int key) {
	return (KeyStateTrigger[key] & 0x80) ? true : false;
}

/**
*	@brief	キーボードリリース情報受け渡し
*	@return	キーボードリリース情報
*/
bool Input::Keyboard_IsRelease(int key) {
	return (KeyStateRelease[key] & 0x80) ? true : false;
}

////////// シーン //////////
/**
*	@brief	シーンのコンストラクタ
*	@param	シーンの名前
*/
Scene::Scene(char *name) {
	this->name = name;
	pSceneResource = new Resource(this);
	camera.Pos = { 0.0f,3.0f,3.0f };
	camera.Look= { 0.0f,0.0f,0.0f };
	camera.At  = { 0.0f,1.0f,0.0f };
	sceneNumber++;
}

/**
*	@brief	シーンチェンジ
*	@param	次のシーンの名前
*/
void Scene::SceneChange(char* nextSceneName) {
	pApp->GetScenes(nextSceneName);
}

void Scene::Init() {
	for (int i = 0; i < objects.size(); i++) {
		objects[i]->Init();
	}
}
void Scene::Update() {
	for (int i = 0; i < objects.size(); i++) {
		objects[i]->Update();
	}
}
void Scene::Draw() {
	for (int i = 0; i < objects.size(); i++) {
		objects[i]->Draw();
	}
}
void Scene::Uninit() {
	for (int i = 0; i < objects.size(); i++) {
		objects[i]->Uninit();
	}
}

void Scene::AddObject(Object* pObject) {
	pObject->pScene = this;
	objects.push_back(pObject);
}

////////// オブジェクト //////////
Object::Object() {
	pTransform = new Transform();
};	

int Object::AddXMesh(const char* FileName) {
	int meshNum = pScene->pSceneResource->AddXMesh(FileName);
	pScene->pSceneResource->xMeshs[meshNum]->pTransform = pTransform;
	return meshNum;
}

void Object::DrawXMesh(int meshNum) {
	pScene->pSceneResource->xMeshs[meshNum]->Draw();
}
void Object::DrawXMesh(int meshNum, D3DXVECTOR3 position) {
	pScene->pSceneResource->xMeshs[meshNum]->Draw(position);
}

////////// スプライト //////////

/**
*	@brief	テクスチャ受け渡し
*	@param	テクスチャ番号
*	@return	テクスチャ
*/
//LPDIRECT3DTEXTURE9 Texture::getTexture(int index) {
//	return pTexture[index];
//}


#endif	// !NAMAEX_H