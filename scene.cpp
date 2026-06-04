#include "scene.h"
#include "system.h"

void UpdateTitle(GameState& state)
{
    UpdateComputers_NonPlay(state.computers);
    if (GetMouseInput() & MOUSE_INPUT_LEFT)
    {
        InitializeGame(state);
        state.scene = SCENE_STAGE_START;
        state.timer = 0;
    }
}

void UpdateStageStart(GameState& state)
{
    UpdateComputers_NonPlay(state.computers);

    if ((GetMouseInput() & MOUSE_INPUT_LEFT) && state.timer > SCENE_CHANGE_WAIT_FRAME)
    {
        ResetPlay(state);
        state.scene = SCENE_PLAY;
        state.timer = 0;
    }
}

void UpdateStageClear(GameState& state)
{
    if ((GetMouseInput() & MOUSE_INPUT_LEFT) && state.timer > SCENE_CHANGE_WAIT_FRAME)
    {
        state.currentStage++;
        if (state.currentStage > STAGE_MAX)
            state.scene = SCENE_GAME_CLEAR;
        else
            state.scene = SCENE_STAGE_START;
        state.timer = 0;
    }
}

void UpdateOver(GameState& state)
{
    if (state.timer > GAMEOVER_WAIT_FRAMES)
    {
        state.scene = SCENE_TITLE;
        state.timer = 0;
    }
}

void UpdateGameClear(GameState& state)
{
    if (state.timer > GAMECLEAR_WAIT_FRAMES)  // 10秒待ってタイトル
    {
        state.scene = SCENE_TITLE;
        state.timer = 0;
    }
}


void DrawTitle(const GameState& state)
{
    drawText(160, 160, 0xffffff, "Fuel Run", 0, 100);
    if (state.timer % 60 < 30)
        drawText(210, 400, 0x00ff00, "Click to start.", 0, 40);
}


void DrawStageStart(const GameState& state)
{
    if (state.currentStage == 1)
    {
        drawText(180, 160, 0xffffff, "STAGE 1", 0, 100);
        drawText(100, 300, 0xffffff, "チュートリアル", 0, 40);
        drawText(100, 350, 0xeeeeee, "SCORE 1000をめざそう", 0, 30);
    }
    else if (state.currentStage == 2)
    {
        drawText(180, 160, 0xffffff, "STAGE 2", 0, 100);
        drawText(100, 300, 0xffffff, "タイムリミット", 0, 40);
        drawText(100, 350, 0xeeeeee, "30秒間 走り切ろう", 0, 30);
    }
    else if (state.currentStage == 3)
    {
        drawText(180, 160, 0xffffff, "STAGE 3", 0, 100);
        drawText(100, 300, 0xffffff, "スコアアタック", 0, 40);
        drawText(100, 350, 0xeeeeee, "SCORE 3000をめざそう", 0, 30);
    }
    else
    {
        drawText(180, 160, 0xffffff, "STAGE %d", state.currentStage, 100);
        
    }

    if (state.timer % 60 < 30)
        drawText(210, 450, 0x00ff00, "Click to start.", 0, 40);
}



void DrawStageClear(const GameState& state)
{
    drawText(100, 240, 0x00ff00, "STAGE CLEAR!", 0, 80);
    if (state.timer % 60 < 30)
        drawText(200, 400, 0x00ff00, "Click to next stage.", 0, 40);
}


void DrawOver(const GameState& state)
{
    drawText(180, 240, 0xff0000, "GAME OVER", 0, 80);
}


void DrawGameClear(const GameState& state)
{
    drawText(100, 240, 0xffff00, "GAME CLEAR!", 0, 80);
    drawText(200, 350, 0xffffff, "Congratulations!", 0, 40);
}

// 敵の車（プレイ中以外）の更新
void UpdateComputers_NonPlay(ComputerCar computers[])
{
    for (int i = 0; i < COM_MAX; i++)
    {
        computers[i].y = computers[i].y - 1 - i;
        if (computers[i].y < COM_SPAWN_Y)
            computers[i].y = COM_RESPAWN_Y;
    }
}