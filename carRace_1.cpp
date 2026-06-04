#include "DxLib.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // 定数
    const int WIDTH = 720, HEIGHT = 640; // ウィンドウの幅と高さのピクセル数

    SetWindowText("カーレース"); // ウィンドウのタイトル
    SetGraphMode(WIDTH, HEIGHT, 32); // ウィンドウの大きさとカラービット数の指定
    ChangeWindowMode(TRUE); // ウィンドウモードで起動
    if (DxLib_Init() == -1) return -1; // ライブラリ初期化 エラーが起きたら終了
    SetBackgroundColor(0, 0, 0); // 背景色の指定
    SetDrawScreen(DX_SCREEN_BACK); // 描画面を裏画面にする

    int bgY = 0; // 道路をスクロールさせるための変数
    int imgBG = LoadGraph("image/bg_long.png"); // 背景の画像

    while (1) // メインループ
    {
        ClearDrawScreen(); // 画面をクリアする

        // 背景のスクロール処理
        bgY = bgY + 10;
        if (bgY >= HEIGHT*2) bgY = bgY - HEIGHT*2;
        if (bgY < 0) bgY = bgY + HEIGHT*2;
        DrawGraph(0, bgY - HEIGHT*2, imgBG, FALSE);
        DrawGraph(0, bgY, imgBG, FALSE);

        ScreenFlip(); // 裏画面の内容を表画面に反映させる
        WaitTimer(16); // 一定時間待つ
        if (ProcessMessage() == -1) break; // Windowsから情報を受け取りエラーが起きたら終了
        if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) break; // ESCキーが押されたら終了
    }

    DxLib_End(); // ＤＸライブラリ使用の終了処理
    return 0; // ソフトの終了
}