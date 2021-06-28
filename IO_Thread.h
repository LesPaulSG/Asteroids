#pragma once
#include "BulletManager.h"

void input(BulletManager& bm, std::chrono::duration<float>& t, bool& gameOver);

void CheckEvent(sf::Event& event, BulletManager& bm, float t);

void KeyboardCheck(sf::Event& event, BulletManager& bm, float t);