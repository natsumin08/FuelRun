// 自分がどの「メニュー」の実装なのか
#include "game.h"
#include "system.h"

const int g_CarW[CAR_MAX] = { 32, 26, 26, 40 };
const int g_CarH[CAR_MAX] = { 48, 48, 48, 100 };

const float g_CarSpeed[CAR_MAX]
{
    0.0f,   // Player
    3.0f,   // CAR_YELLOW
    5.0f,   // CAR_BLUE
    2.0f    // CAAR_TRUCK
};

// 敵の車が重なっているか判定
bool IsOverlapping(int x1, int y1, CarType type1, int x2, int y2, CarType type2)
{
    int dx = abs(x1 - x2);
    int dy = abs(y1 - y2);
    int wid = g_CarW[type1] / 2 + g_CarW[type2] / 2 - CAR_COLLISION_BUFFER;
    int hei = g_CarH[type1] / 2 + g_CarH[type2] / 2 - CAR_COLLISION_BUFFER;

    return (dx < wid && dy < hei);
}

// ステージに応じた敵のタイプをランダムで取得
CarType GetRandomCarTypeForStage(int stage)
{
    if (stage == 1)
        return CAR_YELLOW;
    else if (stage == 2)
        return (CarType)(CAR_YELLOW + rand() % 2);  // YELLOW, BLUE
    else
        return (CarType)(CAR_YELLOW + rand() % 3);  // YELLOW, BLUE, TRUCK
}

// 重複しない座標に敵を配置
void SetNonOverlappingPosition(GameState& state, int carIndex, int yRange, bool checkAllCars)
{
    int newX, newY;
    bool overlap;
    int attemptCount = 0;   // 無限ループ防止用

    // 先にタイプと速度を決める
    CarType newType = GetRandomCarTypeForStage(state.currentStage);
    state.computers[carIndex].type = newType;
    state.computers[carIndex].speedOffset = (rand() % 20) / 10.0f;

    do
    {
        overlap = false;
        newX = rand() % COM_SPAWN_X_RANGE + COM_SPAWN_X_MIN;
        newY = COM_SPAWN_Y - (rand() % yRange);

        // checkAllCars == true : 全車（j != carIndex）
        //              == false: 配置済み（j < carIndex）の車とチェック
        int checkLimit = checkAllCars ? COM_MAX : carIndex;
        for (int j = 0; j < checkLimit; j++)
        {
            if (checkAllCars && carIndex == j) continue;

            if (IsOverlapping(newX, newY, newType,
                state.computers[j].x, state.computers[j].y, state.computers[j].type))
            {
                overlap = true;
                break;
            }
        }
        attemptCount++;
    } while (overlap && attemptCount < SPAWN_ATTEMPT_LIMIT);

    state.computers[carIndex].x = newX;
    state.computers[carIndex].y = newY;
    state.computers[carIndex].flag = 0;
}


// プレイヤーの更新
void UpdatePlayer(Player& player, Scene scene)
{
    if (scene != SCENE_PLAY) return;

    GetMousePoint(&player.x, &player.y);
    if (player.x < PLAYER_MIN_X) player.x = PLAYER_MIN_X;
    if (player.x > PLAYER_MAX_X) player.x = PLAYER_MAX_X;
    if (player.y < PLAYER_MIN_Y) player.y = PLAYER_MIN_Y;
    if (player.y > PLAYER_MAX_Y) player.y = PLAYER_MAX_Y;
}

// 燃料アイテムの更新
void UpdateFuel(FuelItem& fuelItem, Scene scene)
{
    if (scene != SCENE_PLAY) return;

    fuelItem.y += 4;
    if (fuelItem.y > FUEL_RESPAWN_Y)
        fuelItem.y = FUEL_SPAWN_Y_START;
}

// 敵の車（プレイ中）の更新
void UpdateComputers_Play(GameState& state)
{
    for (int i = 0; i < COM_MAX; i++)
    {
        // 速度計算
        float baseSpeed = g_CarSpeed[state.computers[i].type];
        float speed = baseSpeed + state.computers[i].speedOffset;
        state.computers[i].y += (int)speed;    // 車ごとに速度を変える

        // 画面の下から外に出たかを判定
        if (state.computers[i].y > COM_RESPAWN_Y)
            SetNonOverlappingPosition(state, i, COM_RESPAWN_Y_RANGE, true);

        // 追い抜き判定
        if (state.computers[i].y > state.player.y && state.computers[i].flag == 0)
        {
            state.computers[i].flag = 1;
            state.score += SCORE_OVERTAKE;
            if (state.score > state.highScore) state.highScore = state.score;
        }
    }
}

// 当たり判定
void CheckCollisions(GameState& state)
{
    // 敵の車との当たり判定
    for (int i = 0; i < COM_MAX; i++)
    {
        int dx = abs(state.computers[i].x - state.player.x);
        int dy = abs(state.computers[i].y - state.player.y);
        int wid = g_CarW[state.player.type] / 2 + g_CarW[state.computers[i].type] / 2 - CAR_COLLISION_BUFFER;
        int hei = g_CarH[state.player.type] / 2 + g_CarH[state.computers[i].type] / 2 - CAR_COLLISION_BUFFER;

        if (dx < wid && dy < hei)   // 接触
        {
            state.playerFlashTimer = 10;
            PlaySoundMem(state.sounds.seCrash, DX_PLAYTYPE_BACK);
            state.fuel -= FUEL_CRASH_PENALTY;
        }
    }

    // 燃料アイテムとの当たり判定
    if (abs(state.fuelItem.x - state.player.x) < g_CarW[state.player.type] / 2 + FUEL_COLLISION_WIDTH_BUFFER &&
        abs(state.fuelItem.y - state.player.y) < g_CarH[state.player.type] / 2 + FUEL_COLLISION_HEIGHT_BUFFER)
    {
        state.fuelItem.x = rand() % FUEL_SPAWN_X_RANGE + FUEL_SPAWN_X_MIN;
        state.fuelItem.y = FUEL_SPAWN_Y_RETRY;
        state.fuel += FUEL_VALUE;
        PlaySoundMem(state.sounds.seFuel, DX_PLAYTYPE_BACK);
    }
}

// ゲームプレイ中の更新
void UpdatePlay(GameState& state)
{
    UpdatePlayer(state.player, state.scene);
    UpdateComputers_Play(state);
    UpdateFuel(state.fuelItem, state.scene);

    state.fuel -= FUEL_CONSUMPTION;

    // 点滅タイマーを減らす処理
    if (state.playerFlashTimer > 0)
        state.playerFlashTimer--;

    CheckCollisions(state);

    // ゲームオーバー処理
    if (state.fuel < 0)
    {
        state.fuel = 0;
        state.scene = SCENE_OVER;
        state.timer = 0;
        StopSoundMem(state.sounds.bgm);
        PlaySoundMem(state.sounds.jin, DX_PLAYTYPE_BACK);
    }

    bool stageClear = false;

    // --- 条件1：スコア制限ステージの判定 ---
    int clearScore = STAGE_CLEAR_SCORE[state.currentStage - 1];
    if (clearScore > 0)
    {
        if (state.score >= clearScore)
            stageClear = true;
    }

    // --- 条件2：時間制限ステージの判定 ---
    if (state.stageTimer > 0)
    {
        state.stageTimer--;
        if (state.stageTimer == 0)
            stageClear = true;
    }

    // --- 共通のクリア処理 ---
    if (stageClear)
    {
        state.scene = SCENE_STAGE_CLEAR;
        state.timer = 0;
        StopSoundMem(state.sounds.bgm);
        // TODO: クリアジングル
    }
}

// ゲームプレイ中の描画
void DrawPlay(GameState& state, const Images& images)
{
    // 燃料アイテムの描画
    DrawGraph(state.fuelItem.x - 12, state.fuelItem.y - 12, images.fuel, TRUE);

    // 点滅描画処理
    if (state.playerFlashTimer > 0)
    {
        int col = GetColor(rand() % 256, rand() % 256, rand() % 256);
        SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
        DrawBox(state.player.x - g_CarW[state.player.type] / 2, state.player.y - g_CarH[state.player.type] / 2,
            state.player.x + g_CarW[state.player.type] / 2, state.player.y + g_CarH[state.player.type] / 2, col, TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }
}
