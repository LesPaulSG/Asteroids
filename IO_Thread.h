#pragma once
#include "BulletManager.h"

enum GameState {START, IN_GAME, NEW_TOP_SCORE, LEADERBORD};

void input(BulletManager& bm, std::chrono::duration<float>& t, bool& gameOver);

void Start(float deltaTime);

void Game(float deltaTime);

void NewTopScore(float deltaTime);

void LeaderBoard(float deltaTime);

void CheckEvent(sf::Event& event, BulletManager& bm, float t);

void KeyboardPressCheck(sf::Event& event, BulletManager& bm, float t);

void KeyboardReleaseCheck(sf::Event& event, BulletManager& bm, float t);