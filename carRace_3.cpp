#include "DxLib.h"

// 車の画像を管理する定数と配列
enum { RED, YELLOW, BLUE, TRUCK };
const int CAR_MAX = 4;
int imgCar[CAR_MAX];
const int CAR_W[CAR_MAX] = { 32, 26, 26,  40 };
const int CAR_H[CAR_MAX] = { 48, 48, 48, 100 };

// 車を表示する関数
void drawCar(int x, int y, int type)
{
    DrawGraph(x - CAR_W[type] / 2, y - CAR_H[type] / 2, imgCar[type], TRUE);
}

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
    int imgBG = LoadGraph("image/bg.png"); // 背景の画像

    // 車の画像を配列に読み込む
    imgCar[RED] = LoadGraph("image/car_red.png");
    imgCar[YELLOW] = LoadGraph("image/car_yellow.png");
    imgCar[BLUE] = LoadGraph("image/car_blue.png");
    imgCar[TRUCK] = LoadGraph("image/truck.png");

    // プレイヤーの車用の変数
    int playerX = WIDTH / 2;
    int playerY = HEIGHT / 2;
    int playerType = RED;

    while (1) // メインループ
    {
        ClearDrawScreen(); // 画面をクリアする

        // 背景のスクロール処理
        bgY = bgY + 10;
        if (bgY >= HEIGHT) bgY = bgY - HEIGHT;
        DrawGraph(0, bgY - HEIGHT, imgBG, FALSE);
        DrawGraph(0, bgY, imgBG, FALSE);

        // 車両の表示　※制作過程
        drawCar(300, 360, RED);     // 赤
        drawCar(340, 360, YELLOW);  // 黄色
        drawCar(380, 360, BLUE);    // 青
        drawCar(420, 360, TRUCK);   // トラック

        // プレイヤーの車を動かす処理
        GetMousePoint(&playerX, &playerY);
        if (playerX < 260) playerX = 260;
        if (playerX > 460) playerX = 460;
        if (playerY < 40) playerY = 40;
        if (playerY > 600) playerY = 600;
        drawCar(playerX, playerY, playerType);

        ScreenFlip(); // 裏画面の内容を表画面に反映させる
        WaitTimer(16); // 一定時間待つ
        if (ProcessMessage() == -1) break; // Windowsから情報を受け取りエラーが起きたら終了
        if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) break; // ESCキーが押されたら終了
    }

    DxLib_End(); // ＤＸライブラリ使用の終了処理
    return 0; // ソフトの終了
}