#pragma once
#include "Asteroid.h"

enum RotateDir{ STP = 0, RGH = 1, LFT = -1 };

class Player : public Actor {
private:
	float rotation;
	float rotSpeed;
	float force;
	int lives;
	RotateDir rDir;
	bool canMove;
	bool thrustOn;
	Polygon flame;

public:
	Player(sf::Vector2f pos, float rotation);
	~Player() = default;

	float GetRotation() const;
	int GetLives();

	void Move(float time, std::vector<Actor*>& asteroids) override;
	//bool Collision(std::vector<Actor*>& actors) override;
	void Rotate(RotateDir nDir);
	void SetRotation(float angle);
	void Thrust(bool on);
	void HyperJump();
	void BonusLife();
	void Refresh();
	void Draw(sf::RenderWindow& w) override;
	void Destroy() override;
};

