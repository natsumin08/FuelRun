#pragma once

#include "config.h"

// -------------------------------------------------------
// scene.cppで実装される関数の「宣言」
// -------------------------------------------------------

// --- シーン更新関数 ---
void UpdateTitle(GameState& state);
void UpdateStageStart(GameState& state);
void UpdateStageClear(GameState& state);
void UpdateOver(GameState& state);
void UpdateGameClear(GameState& state);

// --- シーン描画関数 ---
void DrawTitle(const GameState& state);
void DrawStageStart(const GameState& state);
void DrawStageClear(const GameState& state);
void DrawOver(const GameState& state);
void DrawGameClear(const GameState& state);

// --- シーンヘルパー関数 ---
void UpdateComputers_NonPlay(ComputerCar computers[]);