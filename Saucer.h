#pragma once
#include "Actor.h"

class Saucer : public Actor {
private:
	float cooldown;
	bool big;
public:
	Saucer(sf::Vector2f p, bool b);
	~Saucer() override;
	void Move(float time, std::vector<Actor*>& asteroids) override;
	bool CanShoot();
	void Destroy(bool playerDestroy = false) override;
	bool isBig();
	Shot GetShoot(sf::Vector2f playerPos);
};