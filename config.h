#pragma once

#include "DxLib.h"

// -------------------------------------------
// グローバル定数（値が変わらないもの）
// -------------------------------------------

// 画面サイズ
const int SCREEN_WIDTH = 720;
const int SCREEN_HEIGHT = 640;

// 車の画像管理
enum CarType {
    CAR_RED,
    CAR_YELLOW,
    CAR_BLUE,
    CAR_TRUCK,
    CAR_MAX
};

// 敵の車の数
const int COM_MAX = 8;

// ゲームシーン
enum Scene {
    SCENE_TITLE,
    SCENE_STAGE_START,
    SCENE_PLAY,
    SCENE_STAGE_CLEAR,
    SCENE_OVER,
    SCENE_GAME_CLEAR
};

// ステージ設定
const int STAGE_MAX = 3;
// ステージごとのクリアスコア
const int STAGE_CLEAR_SCORE[STAGE_MAX] = { 1000, 0, 3000 };
// ステージごとの制限時間
const int STAGE_TIME_LIMIT_SEC[STAGE_MAX] = { 0, 30, 0 };

// プレイヤーの移動範囲
const int PLAYER_MIN_X = 260;
const int PLAYER_MAX_X = 460;
const int PLAYER_MIN_Y = 40;
const int PLAYER_MAX_Y = 600;

// 敵の車のスポーン位置
const int COM_SPAWN_X_MIN = 270;
const int COM_SPAWN_X_RANGE = 180;
const int COM_SPAWN_Y = -100;
const int COM_RESPAWN_Y = SCREEN_HEIGHT + 100;
const int COM_RESPAWN_Y_RANGE = 160;

// 燃料アイテム
const int FUEL_SPAWN_X_MIN = 270;
const int FUEL_SPAWN_X_RANGE = 180;
const int FUEL_SPAWN_Y_START = -100;
const int FUEL_SPAWN_Y_RETRY = -500;
const int FUEL_RESPAWN_Y = SCREEN_HEIGHT;
const int FUEL_VALUE = 200;     // 回復量
const int FUEL_CRASH_PENALTY = 10;  // 衝突時の減少量
const int FUEL_CONSUMPTION = 1;     // 毎フレームの消費量
const int INITIAL_FUEL = 1000;

// スコア
const int SCORE_OVERTAKE = 100;

// 当たり判定バッファ
const int CAR_COLLISION_BUFFER = 4;
const int FUEL_COLLISION_WIDTH_BUFFER = 12;
const int FUEL_COLLISION_HEIGHT_BUFFER = 12;

// シーン遷移
const int SCENE_CHANGE_WAIT_FRAME = 30;
const int GAMEOVER_WAIT_FRAMES = 60 * 5;
const int GAMECLEAR_WAIT_FRAMES = 60 * 10;

// スポーン試行回数
const int SPAWN_ATTEMPT_LIMIT = 100;


// -------------------------------------------
// データ構造
// -------------------------------------------

// プレイヤー
struct Player {
    int x, y;
    CarType type;
};

// 敵の車
struct ComputerCar {
    int x, y;
    CarType type;
    int flag;
    float speedOffset;
};

// 燃料アイテム
struct FuelItem { int x, y; };

// サウンドアセット
struct Sounds {
    int bgm;
    int jin;
    int seFuel;
    int seCrash;
};

// 画像アセット
struct Images {
    int car[CAR_MAX];
    int fuel;
    int bg;
};

// ゲーム全体の状態を管理
struct GameState
{
    // オブジェクト
    Player player;
    ComputerCar computers[COM_MAX];
    FuelItem fuelItem;
    Sounds sounds;
    Images images;

    // ゲーム状態変数
    int bgY;
    int score;
    int highScore;
    int fuel;
    Scene scene;
    int timer;
    int currentStage;
    int playerFlashTimer;
    int stageTimer;
};


// -------------------------------------------
// ファイルスコープ変数
// -------------------------------------------
extern const int g_CarW[CAR_MAX];
extern const int g_CarH[CAR_MAX];

// 車ごとの基本速度
extern const float g_CarSpeed[CAR_MAX];