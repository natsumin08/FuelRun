#include "DxLib.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    SetWindowText("RGB値による色指定"); // ウィンドウのタイトル
    SetGraphMode(600, 600, 32); // ウィンドウの大きさとカラービット数の指定
    ChangeWindowMode(TRUE); // ウィンドウモードで起動
    if (DxLib_Init() == -1) return -1; // ライブラリ初期化 エラーが起きたら終了
    SetBackgroundColor(0, 0, 0); // 背景色の指定
    ClearDrawScreen(); // 画面をクリアする
    SetDrawBlendMode(DX_BLENDMODE_ADD, 255); // ブレンドモードの指定
    DrawCircle(300, 220, 200, GetColor(255, 0, 0), TRUE); // Red
    DrawCircle(200, 380, 200, GetColor(0, 255, 0), TRUE); // Green
    DrawCircle(400, 380, 200, GetColor(0, 0, 255), TRUE); // Blue
    WaitKey(); // キー入力があるまで待つ
    DxLib_End(); // ＤＸライブラリ使用の終了処理
    return 0; // ソフトの終了
}