#include "DxLib.h"
#include <stdlib.h>

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

// 影を付けた文字列を表示する関数
void drawText(int x, int y, int col, const char* txt, int val, int siz)
{
    SetFontSize(siz);
    DrawFormatString(x + 2, y + 2, 0x000000, txt, val);
    DrawFormatString(x, y, col, txt, val);
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

    // コンピューターが動かす車用の配列
    const int COM_MAX = 8;
    int computerX[COM_MAX], computerY[COM_MAX], computerType[COM_MAX], computerFlag[COM_MAX];
    for (int i = 0; i < COM_MAX; i++) // 初期値の代入
    {
        computerX[i] = rand() % 180 + 270;
        computerY[i] = -100;
        computerType[i] = YELLOW + rand() % 3;
        computerFlag[i] = 0;
    }

    // スコアとハイスコアを代入する変数
    int score = 0;
    int highScore = 5000;

    // 燃料アイテム用の変数
    int fuel = 0;
    int fuelX = WIDTH / 2;
    int fuelY = 0;
    int imgFuel = LoadGraph("image/fuel.png");

    while (1) // メインループ
    {
        ClearDrawScreen(); // 画面をクリアする

        // 背景のスクロール処理
        bgY = bgY + 10;
        if (bgY >= HEIGHT) bgY = bgY - HEIGHT;
        DrawGraph(0, bgY - HEIGHT, imgBG, FALSE);
        DrawGraph(0, bgY, imgBG, FALSE);

        // プレイヤーの車を動かす処理
        GetMousePoint(&playerX, &playerY);
        if (playerX < 260) playerX = 260;
        if (playerX > 460) playerX = 460;
        if (playerY < 40) playerY = 40;
        if (playerY > 600) playerY = 600;
        drawCar(playerX, playerY, playerType);

        // コンピューターの車を動かす処理
        for (int i = 0; i < COM_MAX; i++)
        {
            computerY[i] = computerY[i] + 1 + i;
            // 画面の下から外に出たかを判定
            if (computerY[i] > HEIGHT + 100)
            {
                computerX[i] = rand() % 180 + 270;
                computerY[i] = -100;
                computerType[i] = YELLOW + rand() % 3;
                computerFlag[i] = 0;
            }
            // ヒットチェック
            int dx = abs(computerX[i] - playerX); // x軸方向のピクセル数
            int dy = abs(computerY[i] - playerY); // y軸方向のピクセル数
            int wid = CAR_W[playerType] / 2 + CAR_W[computerType[i]] / 2 - 4;
            int hei = CAR_H[playerType] / 2 + CAR_H[computerType[i]] / 2 - 4;
            if (dx < wid && dy < hei) // 接触しているか
            {
                int col = GetColor(rand() % 256, rand() % 256, rand() % 256); // 重ねる色
                SetDrawBlendMode(DX_BLENDMODE_ADD, 255); // 色を加算する設定
                DrawBox(playerX - CAR_W[playerType] / 2, playerY - CAR_H[playerType] / 2, playerX + CAR_W[playerType] / 2, playerY + CAR_H[playerType] / 2, col, TRUE);
                SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); // 通常の描画に戻す
                fuel -= 10;
            }
            // 追い抜いたかを判定
            if (computerY[i] > playerY && computerFlag[i] == 0)
            {
                computerFlag[i] = 1;
                score += 100;
                if (score > highScore) highScore = score;
            }
            drawCar(computerX[i], computerY[i], computerType[i]);
        }

        // 燃料アイテムの処理
        fuelY += 4;
        if (fuelY > HEIGHT) fuelY = -100;
        if (abs(fuelX - playerX) < CAR_W[playerType] / 2 + 12 && abs(fuelY - playerY) < CAR_H[playerType] / 2 + 12)
        {
            fuelX = rand() % 180 + 270;
            fuelY = -500;
            fuel += 200;
        }
        DrawGraph(fuelX - 12, fuelY - 12, imgFuel, TRUE);

        // スコアなどの表示
        drawText(10, 10, 0x00ffff, "SCORE %d", score, 30);
        drawText(WIDTH - 200, 10, 0xffff00, "HI-SC %d", highScore, 30);
        int col = 0x00ff00; // 燃料の値を表示する色
        if (fuel < 400) col = 0xffc000;
        if (fuel < 200) col = 0xff0000;
        drawText(10, HEIGHT - 40, col, "FUEL %d", fuel, 30);

        ScreenFlip(); // 裏画面の内容を表画面に反映させる
        WaitTimer(16); // 一定時間待つ
        if (ProcessMessage() == -1) break; // Windowsから情報を受け取りエラーが起きたら終了
        if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) break; // ESCキーが押されたら終了
    }

    DxLib_End(); // ＤＸライブラリ使用の終了処理
    return 0; // ソフトの終了
}