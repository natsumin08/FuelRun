#pragma once

#include "config.h"

// --------------------------------------------------
// game.cppで実装される関数の「宣言」
// --------------------------------------------------

bool IsOverlapping(int x1, int y1, CarType type1, int x2, int y2, CarType type2);

CarType GetRandomCarTypeForStage(int stage);

void SetNonOverlappingPosition(GameState& state, int carIndex, int yRange, bool checkAllCars);

// プレイヤーの更新
void UpdatePlayer(Player& player, Scene scene);

// 燃料アイテムの更新
void UpdateFuel(FuelItem& fuelItem, Scene scene);

// 敵の車（プレイ中）の更新
void UpdateComputers_Play(GameState& state);

// 当たり判定
void CheckCollisions(GameState& state);

// ゲームプレイ中の更新
void UpdatePlay(GameState& state);

// ゲームプレイ中の描画
void DrawPlay(GameState& state, const Images& images);