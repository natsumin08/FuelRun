#pragma once

#include "config.h"

// ----------------------------------------------------------
// system.cppで実装される関数の「宣言」
// ----------------------------------------------------------

// --- 初期化・リセット関数 ---
void LoadGameAssets(GameState& state);
void InitializeGame(GameState& state);
void ResetPlay(GameState& state);

// --- 描画ヘルパー関数 ---
void drawCar(int x, int y, CarType type, const Images& images);
void drawText(int x, int y, int col, const char* txt, int val, int siz);
void DrawBackground(int& bgY, Scene scene, const Images& images);
void DrawComputers(const ComputerCar computers[], const Images& images);
void DrawUI(const GameState& state);