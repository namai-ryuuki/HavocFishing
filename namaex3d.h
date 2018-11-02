//================================================
//	Direct3D9�d�l���C�u����[namaex3d.h]
//		AT12F75_28_���䗴��
//			2018 10 19
//================================================
#ifndef NAMAEX_H
#define NAMAEX_H

//===== �C���N���[�h�t�@�C�� =====
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

//===== �v���O���}�R�����g ======
#pragma	comment (lib,"d3d9.lib")
#pragma	comment (lib,"d3dx9.lib")
#pragma comment (lib,"dxguid.lib")
#pragma comment (lib,"dinput8.lib")
#pragma comment (lib,"xinput.lib")

//===== �}�N����` =====
#define STRICT				// �^�`�F�b�N�������ɍs��
#define WIN32_LEAN_AND_MEAN	// �w�b�_�[���炠�܂�g��Ȃ��֐�������

#define RELEASE(x)	{ if(x) { (x)->Release(); (x) = NULL; } }	// ���S�ȊJ��

#define	NUM_KEY_MAX		(256)	// �L�[�̍ő吔

#define WINDOW_STYLE	(WS_OVERLAPPEDWINDOW ^WS_THICKFRAME ^ WS_MAXIMIZEBOX ^ WS_MINIMIZEBOX)	// �E�B���h�E�Y�N���X�̃X�^�C��
#define CLASS_NAME		_T("GameWindow")														// �E�B���h�E�Y�N���X�̖��O

#define FVF_VERTEX	(D3DFVF_XYZ | D3DFVF_DIFFUSE)	//�g�����X�t�H�[�����ꂽ�ʒu���W���܂ޒ��_�t�H�[�}�b�g

//===== �N���X�錾 =====
class Application;	// �A�v���P�[�V����
class Resource;		// ���\�[�X�Ǘ�
class Input;		// ���͊Ǘ�
class Scene;		// �V�[��
class Object;		// �I�u�W�F�N�g
class Component;	// �R���|�[�l���g
class Transform;	// ���W���
class XMesh;		// ���b�V��(.x)
class Sprite;		// �X�v���C�g

//===== �N���X��` =====
//----- �A�v���P�[�V���� -----
class Application {
public:
	/* WinMain����̃A�N�Z�X������ */
	friend int APIENTRY _tWinMain(HINSTANCE, HINSTANCE, LPSTR, int);
	
	/* ���_�f�[�^ */
	struct Vertex {
		float x, y, z;
		D3DCOLOR color;
	};

	/* �R���X�g���N�ƃf�X�g���N�^ */
	Application(char* windowName, int width, int height, HINSTANCE hInstance, int nCmdShow);
	~Application(void) {};

	/* �V�[���̕ύX */
	void SceneChange(int nextSceneNumber) { this->nextSceneNumber = nextSceneNumber; };

	/* �V�[���̒ǉ� */
	void AddScene(Scene *);

	/* �A�v���P�[�V�����̎��s */
	void Play();

	/* �Q�b�^�[ */
	LPDIRECT3DDEVICE9 GetDevice()	{ return pDevice; };
	Input* GetInput()				{ return pInput; };
	Scene* GetScenes(char* name);
	Scene* GetScenes()				{ return scenes[sceneNumber];};
	int GetSceneNumber()			{ return sceneNumber;};
	int GetScreenWidth()			{ return screenWidth; };
	int GetScreenHeight()			{ return screenHeight; };
private:
	HWND hWnd;					// �E�B���h�E�n���h��
	int screenWidth;			// �X�N���[������
	int screenHeight;			// �X�N���[������

	LPDIRECT3D9 pD3D;			// Direct3D�|�C���^
	LPDIRECT3DDEVICE9 pDevice;	// Direct3D�f�o�C�X�|�C���^

	Input* pInput;	// ���͊Ǘ�

	std::vector<Scene *> scenes;	// �V�[��
	int sceneNumber;				// ���s���̃V�[���ԍ�
	int nextSceneNumber;			// ���Ɏ��s�����V�[���ԍ�

	/* �f�o�C�X�֌W�����m�F���� */
	void Check(HRESULT work,char* workName){
		if (FAILED(work)) {
			char message[256];
			sprintf(message, _T("%s�Ɏ��s���܂���"), workName);
			MessageBox(hWnd, message, _T("ERROR"), MB_ICONWARNING);
			exit(-1);
		}
	}
};

//----- ���͊Ǘ� -----
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
	BYTE	KeyState[NUM_KEY_MAX];			// �L�[�{�[�h�̓��͏�񃏁[�N
	BYTE	KeyStateTrigger[NUM_KEY_MAX];	// �L�[�{�[�h�̃g���K�[��񃏁[�N
	BYTE	KeyStateRelease[NUM_KEY_MAX];	// �L�[�{�[�h�̃����[�X��񃏁[�N
	LPDIRECTINPUT8	pInput;			// ���̓|�C���^
	LPDIRECTINPUTDEVICE8 pKeyboard;	// �L�[�{�[�h�|�C���^
};

//----- �V�[�� -----
class Scene {
public:
	/* �A�N�Z�X�̋��� */
	friend void Application::AddScene(Scene *);

	Application *pApp;			// �A�v���P�[�V����
	Resource *pSceneResource;	// �V�[���̃��\�[�X
	vector<Object*> objects;	// �I�u�W�F�N�g

	/* �J���� */
	struct Camera{
		D3DXVECTOR3 Pos, Look, At;
	};
	Camera camera;

	/* �R���X�g���N�g�ƃf�X�g���N�^ */
	Scene(char *name);
	~Scene() {};

	/* ���{���� */
	void Init();
	void Update();
	void Draw();
	void Uninit();

	/* �I�u�W�F�N�g�̒ǉ� */
	void AddObject(Object* pObject);

protected:
	static int sceneMax;	// �V�[���̐�

	char* name;			// �V�[����
	int sceneNumber;	// �V�[���ԍ�

	/* �V�[���`�F���W */
	void SceneChange(char* nextSceneChange);
};

//----- �I�u�W�F�N�g -----
class Object {
public:
	int tag;		// �I�u�W�F�N�g�̃^�O
	Scene *pScene;	// �V�[���|�C���^

	vector<Component*> components;	// �R���|�[�l���g
	Transform* pTransform;	// �I�u�W�F�N�g�̈ʒu���

	Object();
	~Object() {};
	
	/* ���z�֐� */
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

//----- �R���|�[�l���g -----
class Component {
public:
	void Init();
	void Update();
	void Draw();
	void Uninit();
};

//----- �ʒu��� -----
class Transform {
public:
	D3DXVECTOR3 pos;		// ���W
	D3DXVECTOR3 scale;		// �X�P�[��
	D3DXVECTOR3 rotation;	// �p�x

	Transform() {
		pos = D3DXVECTOR3(0,0,0);
		scale = D3DXVECTOR3(1,1,1);
		rotation = D3DXVECTOR3(0,0,0);
	}
};

//----- ���\�[�X�Ǘ� -----
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

//----- X�t�@�C�� -----
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

		/* X�t�@�C���ǂݍ��� */
		D3DXLoadMeshFromX(FileName, D3DXMESH_SYSTEMMEM, pDevice, &pAdjacencyBuffer, &pMaterialBuffer, NULL, &MaterialMax, &pMesh);

		/* ���b�V���œK�� */
		pMesh->OptimizeInplace(D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE, (DWORD*)pAdjacencyBuffer->GetBufferPointer(), NULL, NULL, NULL);
		RELEASE(pAdjacencyBuffer);

		/* �@���x�N�g���̏������� */
		if (!(pMesh->GetFVF() & D3DFVF_NORMAL)) {
			ID3DXMesh* pTempMesh = NULL;
			pMesh->CloneMeshFVF(pMesh->GetOptions(),pMesh->GetFVF() | D3DFVF_NORMAL, pDevice, &pTempMesh);
			D3DXComputeNormals(pTempMesh, NULL);
			pMesh->Release();
			pMesh = pTempMesh;
		}

		/* �}�e���A���p�������̊m�� */
		pMaterial = new D3DMATERIAL9[MaterialMax];
		pTexture = new LPDIRECT3DTEXTURE9[MaterialMax];
		D3DXMATERIAL* Materials = (D3DXMATERIAL*)pMaterialBuffer->GetBufferPointer();
		
		/* �}�e���A���ǂݍ��� */
		for (unsigned int i = 0; i < MaterialMax; i++) {
			/* �}�e���A������ */
			pMaterial[i] = Materials[i].MatD3D;
			pMaterial[i].Ambient = pMaterial[i].Diffuse;
			
			/* �e�N�X�`���̓ǂݍ���*/
			pTexture[i] = NULL;			
			WCHAR textureFileName[MAX_PATH];
			MultiByteToWideChar(CP_ACP, 0, Materials[i].pTextureFilename, -1, textureFileName, MAX_PATH);
			textureFileName[MAX_PATH - 1] = '\0';
			D3DXCreateTextureFromFile(pDevice, (LPCSTR)textureFileName, &pTexture[i]);
		}
	};

	~XMesh() {};

	void Draw() {	
		
		/* ���[���h�s��쐬 */
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

		/* ���[���h�s��쐬 */
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

//----- �X�v���C�g ------

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
//		/* �e�N�X�`���T�C�Y�擾 */
//		float width = width;
//		float height = height;
//
//		/* �e�N�X�`���̉����͈� */
//		int texWidth = width / pTexture->getPatternSide(index);
//		int texHeight = height / (pTexture->getPatternMax(index) / pTexture->getPatternSide(index));
//
//		/* �e�N�X�`�����W�v�Z */
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
//		/* �X�P�[���v�Z */
//		texWidth *= pTransform->scale.x;
//		texHeight *= pTransform->scale.y;
//
//		/* ��]�v�Z */
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
//		/* �|���S���̒��_�̐ݒ� */
//		VECTOR vertex2D[] = {
//			{ D3DXVECTOR4(px[0] + posX - 0.5f, py[0] + posY - 0.5f, 1.0f,1.0f), color, D3DXVECTOR2(u0, v0) },
//			{ D3DXVECTOR4(px[1] + posX - 0.5f, py[1] + posY - 0.5f, 1.0f,1.0f), color, D3DXVECTOR2(u1, v0) },
//			{ D3DXVECTOR4(px[2] + posX - 0.5f, py[2] + posY - 0.5f, 1.0f,1.0f), color, D3DXVECTOR2(u0, v1) },
//			{ D3DXVECTOR4(px[3] + posX - 0.5f, py[3] + posY - 0.5f, 1.0f,1.0f), color, D3DXVECTOR2(u1, v1) }
//		};
//
//		/* ���z�A�h���X */
//		VECTOR *pV;
//
//		/* ���_�o�b�t�@�̃��b�N */
//		pVertexBuffer->Lock(0, 0, (void**)&pV, 0);
//		memcpy(pV, vertex2D, sizeof(vertex2D));
//		pVertexBuffer->Unlock();
//
//		/* �|���S���̕\�� */
//		pDevice->SetFVF(FVF_VERTEX);
//		pDevice->SetTexture(0, pTexture->getTexture(index));
//		pDevice->SetStreamSource(0, pVertexBuffer, 0, sizeof(VECTOR));
//		pDevice->SetIndices(pIndexBuffer);
//		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
//		}
//	};
//};

//===== �֐���` =====

////////// �E�B���h�E�v���V�[�W�� //////////
/**
 * @brief	�E�B���h�E�v���V�[�W��
 * @param	�E�B���h�E�n���h��, ���b�Z�[�W���ʎq, ���b�Z�[�W�̍ŏ��̃p�����[�^, ���b�Z�[�W��2�Ԗڂ̃p�����[�^
 */
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	/* ����{�^���̏��� */
	case WM_CLOSE:
		if (MessageBox(hWnd,_T( "�A�v���P�[�V�������I�����܂����H"), "�m�F", MB_OKCANCEL | MB_DEFBUTTON2 == IDOK)) {
			DestroyWindow(hWnd);
		}
		return 0;

	/* �v���O�����̏I�����V�X�e���ɒʒm */
	case WM_DESTROY:
		PostQuitMessage(NULL);
		break;

	/* ���̑��̃C�x���g���V�X�e���ɏ��������� */
	default:
		return (DefWindowProc(hWnd, msg, wParam, lParam));
	}
	return 0;
}

////////// �A�v���P�[�V���� //////////
/**
 * @brief	�A�v���P�[�V�����̃R���X�g���N�^
 * @param	�ŏ��̃V�[��,�E�B���h�E�̖��O,�E�B���h�E����,�E�B���h�E����,�C���X�^���X�n���h��,�E�B���h�E�����グ���̏��
 */
Application::Application(char* windowName, int width, int height, HINSTANCE hInstance, int nCmdShow) {

	/* �����W�F�l���[�^�[�������� */
	srand((unsigned)time(NULL));

	/* �����o�̏����� */
	sceneNumber = 0;
	nextSceneNumber = 0;

	/* �X�N���[���T�C�Y�̕ۑ� */
	screenWidth		= width;
	screenHeight	= height;

	/* �E�B���h�E�̏����ʒu��ݒ� */
	int window_x	= (GetSystemMetrics(SM_CXSCREEN) - width)	/ 2;
	int window_y	= (GetSystemMetrics(SM_CYSCREEN) - height)	/ 2;

	/* �E�B���h�E�N���X�̐ݒ� */
	WNDCLASSEX wc	= { sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW, WndProc, 0, 0, hInstance, NULL, NULL,
		(HBRUSH)(COLOR_WINDOW + 1), NULL, CLASS_NAME, NULL };

	/* �E�B���h�E�N���X�̓o�^ */
	Check(RegisterClassEx(&wc) == NULL,"�E�B���h�E�̍쐬");

	/* �E�B���h�E�n���h���̐ݒ� */
	hWnd = CreateWindow(CLASS_NAME, windowName, WINDOW_STYLE, window_x, window_y, width, height, NULL, NULL, hInstance, NULL);
	Check(hWnd != NULL, "�E�B���h�E�n���h���̐ݒ�");

	/* Direct3D�̐ݒ� */
	pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	Check(pD3D != NULL, "Direct3D�̐ݒ�");

	/* �f�B�X�v���C���[�h�̐ݒ� */
	D3DDISPLAYMODE d3ddm;
	Check(pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm), "�f�B�X�v���C���[�h�̐ݒ�");

	/* �v���[���p�����[�^ */
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));	
	d3dpp.Windowed = true;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	/* �f�o�C�X�쐬 */
	Check(pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &pDevice), "Direct3DDevice�̍쐬");
	D3DLIGHT9 light;
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r = light.Diffuse.g = light.Diffuse.b = 1.0f;
	light.Specular.r = light.Specular.g = light.Specular.b = 0.2f;
	light.Ambient.r = light.Ambient.g = light.Ambient.b = 0.5f;
	light.Direction = D3DXVECTOR3(1, -2, 1);
	pDevice->SetLight(0, &light);
	pDevice->LightEnable(0, TRUE);

	/* �����_�[�̐ݒ� */
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
	
	/* �ˉe�s��쐬 */
	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(90.0f),(float)width/height,1.0f,100.0f);
	pDevice->SetTransform(D3DTS_PROJECTION, &matProj);

	/* ���͊Ǘ��̏����� */
	pInput = new Input(hInstance, hWnd);
}

/**
 * @brief	�V�[���̒ǉ�
 * @param	�ǉ�����V�[���̃|�C���^
 */
void Application::AddScene(Scene *pScene) {
	scenes.push_back(pScene);
	pScene->pApp = this;
}

/**
 *	@brief	�A�v���P�[�V�����̎��s
 */
void Application::Play() {
	/* ���b�Z�[�W�̐ݒ� */
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	/* �E�B���h�E�\�� */
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);

	/* �ŏ��̃V�[���̏����� */
	scenes[sceneNumber]->Init();
	printf("");

	/* �A�v���P�[�V�������[�v */
	while(msg.message != WM_QUIT){
		/* �E�B���h�E�ւ̃C�x���g���擾 */
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			/* ���z�L�[���b�Z�[�W�𕶎����b�Z�[�W�֕ϊ� */
			TranslateMessage(&msg);
			/*�E�B���h�E�փC�x���g��ʒB */
			DispatchMessage(&msg);
		}
		else {
			/* ���͂̍X�V */
			pInput->Update();

			/* �V�[���̍X�V���� */
			scenes[sceneNumber]->Update();

			/* ��ʂ̃N���A */
			pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 255, 255), 1.0f, 0);

			/* �`��o�b�`���߂̊J�n */
			if (SUCCEEDED(pDevice->BeginScene())) {	

				/* �r���[�s��쐬 */
				D3DXMATRIXA16 matView;
				D3DXMatrixLookAtLH(&matView, &scenes[sceneNumber]->camera.Pos, &scenes[sceneNumber]->camera.Look, &scenes[sceneNumber]->camera.At);
				pDevice->SetTransform(D3DTS_VIEW, &matView);

				/* �V�[���̕`�� */
				scenes[sceneNumber]->Draw();
				
				/* �`��o�b�`���߂̏I�� */
				pDevice->EndScene();

				/* �o�b�N�o�b�t�@���t���b�v */
				pDevice->Present(NULL, NULL, NULL, NULL);

				/* �ҋ@�t���[�� */
				Sleep(10);
			}

						
			/* �V�[���`�F���W */
			if (sceneNumber != nextSceneNumber) {
				scenes[sceneNumber]->Uninit();
				sceneNumber = nextSceneNumber;
				scenes[sceneNumber]->Init();
			}
		}
	}
}

/**
 *	@brief	�V�[���̎擾
 *	@param	�擾����V�[���̖��O
 *	@return	�V�[���ԍ��A�Ȃ����NULL
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

////////// ���\�[�X //////////
/**
* @brief	���\�[�X�̏�����
* @param	�ǂݍ��݃t�@�C���l�[��
* @return	���b�V���ԍ�
*/
int Resource::AddXMesh(const char* fileName) {
	XMesh* xMesh;
	xMesh = new XMesh(fileName, pScene->pApp->GetDevice());
	xMeshs.push_back(xMesh);
	return xMeshs.size() - 1;
}

///////// �C���v�b�g //////////
/**
*	@brief	DirectInput�̐ݒ�
*	@param	�C���X�^���X�n���h��
*/
Input::Input(HINSTANCE hInstance, HWND hWnd) {
	/* DirectInput�I�u�W�F�N�g�̍쐬 */
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&pInput, NULL))) {
		MessageBox(hWnd, "DirectInput�̐ݒ�Ɏ��s���܂���", "ERROR", MB_ICONWARNING);
	};

	/* �L�[�{�[�h�f�o�C�X�̍쐬 */
	if (FAILED(pInput->CreateDevice(GUID_SysKeyboard, &pKeyboard, NULL))) {
		MessageBox(hWnd, "�L�[�{�[�h�̎擾�Ɏ��s���܂���", "ERROR", MB_ICONWARNING);
	}

	/* �f�[�^�t�H�[�}�b�g��ݒ� */
	if (FAILED(pKeyboard->SetDataFormat(&c_dfDIKeyboard))) {
		MessageBox(hWnd, "�L�[�{�[�h�̃f�[�^�t�H�[�}�b�g�ݒ�Ɏ��s���܂���", "ERROR", MB_ICONWARNING);
	}

	/* �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j*/
	if (FAILED(pKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))) {
		MessageBox(hWnd, "�L�[�{�[�h�̋������[�h���̐ݒ�Ɏ��s���܂���", "ERROR", MB_ICONWARNING);
	}

	/* �A�N�Z�X�����l��(���͐���J�n) */
	pKeyboard->Acquire();
}

/**
*	@brief	DirectInput�̍X�V
*/
void Input::Update() {
	/* �f�o�C�X����f�[�^���擾 */
	if (SUCCEEDED(pKeyboard->GetDeviceState(sizeof(KeyState), KeyState))) {
		for (int nCkey = 0; nCkey < NUM_KEY_MAX; nCkey++)
		{
			/* �L�[�g���K�[�E�����[�X���𐶐� */
			KeyStateTrigger[nCkey] = (KeyState[nCkey] ^ KeyState[nCkey]) & KeyState[nCkey];
			KeyStateRelease[nCkey] = (KeyState[nCkey] ^ KeyState[nCkey]) & KeyState[nCkey];
		}
	}
	else {
		/* �L�[�{�[�h�ւ̃A�N�Z�X�����擾 */
		pKeyboard->Acquire();
	}

	/* �f�o�C�X����f�[�^���擾 */
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
*	@brief	�L�[�{�[�h�v���X���󂯓n��
*	@return	�L�[�{�[�h�v���X���
*/
bool Input::Keyboard_IsPress(int key) {
	return (KeyState[key] & 0x80) ? true : false;
}

/**
*	@brief	�L�[�{�[�h�g���K�[���󂯓n��
*	@return	�L�[�{�[�h�g���K�[���
*/
bool Input::Keyboard_IsTrigger(int key) {
	return (KeyStateTrigger[key] & 0x80) ? true : false;
}

/**
*	@brief	�L�[�{�[�h�����[�X���󂯓n��
*	@return	�L�[�{�[�h�����[�X���
*/
bool Input::Keyboard_IsRelease(int key) {
	return (KeyStateRelease[key] & 0x80) ? true : false;
}

////////// �V�[�� //////////
/**
*	@brief	�V�[���̃R���X�g���N�^
*	@param	�V�[���̖��O
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
*	@brief	�V�[���`�F���W
*	@param	���̃V�[���̖��O
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

////////// �I�u�W�F�N�g //////////
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

////////// �X�v���C�g //////////

/**
*	@brief	�e�N�X�`���󂯓n��
*	@param	�e�N�X�`���ԍ�
*	@return	�e�N�X�`��
*/
//LPDIRECT3DTEXTURE9 Texture::getTexture(int index) {
//	return pTexture[index];
//}


#endif	// !NAMAEX_H