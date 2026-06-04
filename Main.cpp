#include "DxLib.h"
#include <stdlib.h>
#include <math.h>

#include "config.h"
#include "game.h"
#include "scene.h"
#include "system.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // 定数
    const int WIDTH = 720, HEIGHT = 640; // ウィンドウの幅と高さのピクセル数

    SetWindowText("Fuel Run"); // ウィンドウのタイトル
    SetGraphMode(WIDTH, HEIGHT, 32); // ウィンドウの大きさとカラービット数の指定
    ChangeWindowMode(TRUE); // ウィンドウモードで起動
    if (DxLib_Init() == -1) return -1; // ライブラリ初期化 エラーが起きたら終了
    SetBackgroundColor(0, 0, 0); // 背景色の指定
    SetDrawScreen(DX_SCREEN_BACK); // 描画面を裏画面にする

    // --- 変数の宣言 --
    GameState state;

    // --- 初期化 ---
    LoadGameAssets(state);
    InitializeGame(state);
    state.scene = SCENE_TITLE;
    state.timer = 0;

    while (1) // メインループ
    {
        ClearDrawScreen(); // 画面をクリアする
        state.timer++;

        // --- 更新処理 ---
        switch (state.scene)
        {
        case SCENE_TITLE:       UpdateTitle(state);         break;
        case SCENE_STAGE_START: UpdateStageStart(state);    break;
        case SCENE_PLAY:        UpdatePlay(state);          break;
        case SCENE_STAGE_CLEAR: UpdateStageClear(state);    break;
        case SCENE_OVER:        UpdateOver(state);          break;
        case SCENE_GAME_CLEAR:  UpdateGameClear(state);     break;
        }

        // --- 描画処理 ---
        DrawBackground(state.bgY, state.scene, state.images);
        if (state.scene != SCENE_TITLE)
            drawCar(state.player.x, state.player.y, state.player.type, state.images);
        DrawComputers(state.computers, state.images);

        // シーン別の描画
        switch (state.scene)
        {
        case SCENE_TITLE:       DrawTitle(state);       break;
        case SCENE_STAGE_START: DrawStageStart(state);  break;
        case SCENE_PLAY:        DrawPlay(state, state.images);        break;
        case SCENE_STAGE_CLEAR: DrawStageClear(state);  break;
        case SCENE_OVER:        DrawOver(state);        break;
        case SCENE_GAME_CLEAR:  DrawGameClear(state);   break;
        }

        DrawUI(state);

        // --- ループの最後 ---
        ScreenFlip(); // 裏画面の内容を表画面に反映させる
        WaitTimer(16); // 一定時間待つ
        if (ProcessMessage() == -1) break; // Windowsから情報を受け取りエラーが起きたら終了
        if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) break; // ESCキーが押されたら終了
    }

    DxLib_End(); // ＤＸライブラリ使用の終了処理
    return 0; // ソフトの終了
}