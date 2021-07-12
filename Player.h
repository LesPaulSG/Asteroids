#pragma once
#include "Asteroid.h"

enum class RotateDir{ STP = 0, RGH = 1, LFT = -1 };

class Player : public Actor {
private:
	Polygon flame;
	float rotation;
	float rotSpeed;
	float force;
	int lives;
	RotateDir rDir;
	bool canMove;
	bool thrustOn;

public:
	Player(sf::Vector2f pos, float rotation);

	float GetRotation() const;
	int GetLives() const;
	bool CanMove() const;

	void Move(float time, std::vector<Actor*>& actors) override;
	bool Collision(const std::vector<Actor*>& actors) override;
	void SetRotateDir(RotateDir nDir);
	void Thrust(bool on);
	void HyperJump();
	void BonusLife();
	void Refresh();
	void Reset();
	void Draw(sf::RenderWindow& w) const override;
	void Destroy(bool playerDestroy = false) override;
};

