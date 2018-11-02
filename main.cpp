//================================================
//	メイン[main.cpp]
//		AT12F75_28_生井龍希
//			2018 10 19
//================================================
//===== ヘッダファイル =====
#include "namaex3d.h"
#include "game.h"

//===== 関数定義 =====
/**
 * @brief	メイン
 * @param	インスタンスハンドル, 呼び出し先インスタンスハンドル, コマンドラインポインタ, 立ち上げ時のウィンドウの表示状態
 */
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	Application test(_T("TEST"), 1024, 768, hInstance, nCmdShow);

	/* シーンのインスタンス化 */
	Game game;	// この時にコンストラクタが自動で実行される

	/* シーンの追加 */
	test.AddScene((Scene *)&game);

	/* アプリケーションの再生 */
	test.Play();
}