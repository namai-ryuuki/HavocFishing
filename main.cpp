//================================================
//	���C��[main.cpp]
//		AT12F75_28_���䗴��
//			2018 10 19
//================================================
//===== �w�b�_�t�@�C�� =====
#include "namaex3d.h"

//===== �֐���` =====
/**
 * @brief	���C��
 * @param	�C���X�^���X�n���h��, �Ăяo����C���X�^���X�n���h��, �R�}���h���C���|�C���^, �����グ���̃E�B���h�E�̕\�����
 */
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	Application test(_T("TEST"), 1024, 768, hInstance, nCmdShow);

	test.Play();
}