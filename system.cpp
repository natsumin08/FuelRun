#include "system.h"
#include "game.h"

// アセットの読み込み
void LoadGameAssets(GameState& state)
{
    state.images.bg = LoadGraph("image/bg.png");
    state.images.car[CAR_RED] = LoadGraph("image/car_red.png");
    state.images.car[CAR_YELLOW] = LoadGraph("image/car_yellow.png");
    state.images.car[CAR_BLUE] = LoadGraph("image/car_blue.png");
    state.images.car[CAR_TRUCK] = LoadGraph("image/truck.png");
    state.images.fuel = LoadGraph("image/fuel.png");

    state.sounds.bgm = LoadSoundMem("sound/bgm.mp3");
    state.sounds.jin = LoadSoundMem("sound/gameover.mp3");
    state.sounds.seFuel = LoadSoundMem("sound/fuel.mp3");
    state.sounds.seCrash = LoadSoundMem("sound/crash.mp3");

    ChangeVolumeSoundMem(128, state.sounds.bgm);
    ChangeVolumeSoundMem(128, state.sounds.jin);
}

// ゲームの初期化（タイトル画面用）
void InitializeGame(GameState& state)
{
    state.player.x = SCREEN_WIDTH / 2;
    state.player.y = SCREEN_HEIGHT / 2;
    state.player.type = CAR_RED;

    for (int i = 0; i < COM_MAX; i++)
    {
        state.computers[i].x = rand() % COM_SPAWN_X_RANGE + COM_SPAWN_X_MIN;
        state.computers[i].y = -100;  // 初期位置
        state.computers[i].type = (CarType)(CAR_YELLOW + rand() % 3); // YELLOW, BLUE, TRUCK
        state.computers[i].flag = 0;
        state.computers[i].speedOffset = (rand() % 20) / 10.0f;
    }
    state.fuelItem.x = SCREEN_WIDTH / 2;
    state.fuelItem.y = 0;

    state.score = 0;
    state.highScore = 5000;
    state.currentStage = 1;
    state.fuel = 0;
    state.bgY = 0;
    state.timer = 0;
}

// プレイ開始時のリセット処理
void ResetPlay(GameState& state)
{
    state.player.x = SCREEN_WIDTH / 2;
    state.player.y = SCREEN_HEIGHT / 2;
    state.playerFlashTimer = 0;

    // 敵の車を画面外上にリセット
    for (int i = 0; i < COM_MAX; i++)
        SetNonOverlappingPosition(state, i, SCREEN_HEIGHT, false);

    state.fuelItem.x = SCREEN_WIDTH / 2;
    state.fuelItem.y = FUEL_SPAWN_Y_START;

    state.score = 0;
    state.fuel = INITIAL_FUEL;
    state.scene = SCENE_PLAY;
    // config.hの設定値（秒）を60倍（フレーム）してタイマーにセット
    state.stageTimer = STAGE_TIME_LIMIT_SEC[state.currentStage - 1] * 60;
    PlaySoundMem(state.sounds.bgm, DX_PLAYTYPE_LOOP); // BGMをループ再生
}

// 車を表示する関数
void drawCar(int x, int y, CarType type, const Images& images)
{
    // 型の範囲チェック（安全のため）
    if (type < 0 || type >= CAR_MAX) return;
    DrawGraph(x - g_CarW[type] / 2, y - g_CarH[type] / 2, images.car[type], TRUE);
}

// 影を付けた文字列を表示する関数
void drawText(int x, int y, int col, const char* txt, int val, int siz)
{
    SetFontSize(siz);
    DrawFormatString(x + 2, y + 2, 0x000000, txt, val);
    DrawFormatString(x, y, col, txt, val);
}

// 背景の描画
void DrawBackground(int& bgY, Scene scene, const Images& images)
{
    if (scene == SCENE_PLAY) bgY = (bgY + 10) % SCREEN_HEIGHT;  // プレイ中だけスクロール
    DrawGraph(0, bgY - SCREEN_HEIGHT, images.bg, FALSE);
    DrawGraph(0, bgY, images.bg, FALSE);
}

// 敵の車の描画
void DrawComputers(const ComputerCar computers[], const Images& images)
{
    for (int i = 0; i < COM_MAX; i++)
        drawCar(computers[i].x, computers[i].y, computers[i].type, images);
}

// UIの描画
void DrawUI(const GameState& state)
{
    // --- 基本UI（スコア、ハイスコア）---
    drawText(10, 10, 0x00ffff, "SCORE %d", state.score, 30);
    drawText(SCREEN_WIDTH - 200, 10, 0xffff00, "HI-SC %d", state.highScore, 30);

    // --- 基本UI（燃料）---
    int col = 0x00ff00; // 燃料の値を表示する色
    if (state.fuel < 400) col = 0xffc000;
    if (state.fuel < 200) col = 0xff0000;
    drawText(10, SCREEN_HEIGHT - 40, col, "FUEL %d", state.fuel, 30);

    // --- プレイ中の追加UI ---
    if (state.scene == SCENE_PLAY)
    {
        // Stage 1（チュートリアル）の説明表示
        if (state.currentStage == 1)
        {
            drawText(SCREEN_WIDTH - 220, 100, 0xffffff, "マウスで操作", 0, 24);
            drawText(SCREEN_WIDTH - 220, 130, 0xffffff, "F：燃料（回復）", 0, 24);
            drawText(SCREEN_WIDTH - 220, 160, 0xffffff, "敵：車", 0, 24);
            drawText(SCREEN_WIDTH - 220, 190, 0xffffff, "SCORE 1000でクリア", 0, 24);
        }

        // 時間制限ステージのタイマー表示
        if (state.stageTimer > 0)
        {
            // フレーム単位を秒単位に変換（60フレーム＝1秒）
            int timeSec = (state.stageTimer / 60) + 1;
            drawText(SCREEN_WIDTH / 2 - 50, 10, 0xff0000, "TIME %d", timeSec, 30);
        }
    }
}
