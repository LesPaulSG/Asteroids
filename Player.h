#pragma once
#include "Wall.h"

enum MoveDir{ STP, FWD, BWD, RGH, LFT };

class Player{
private:
	sf::Vector2f pos;
	sf::Vector2f forwardVector;
	sf::Vector2f rightVector;
	float rotation;
	float speed;
	float radius;
	MoveDir dir;

	sf::CircleShape body;

public:
	Player(sf::Vector2f pos, float rotation);
	~Player() = default;

	const sf::CircleShape& GetBody() const;
	const sf::Vector2f& GetPosition() const;
	const sf::Vector2f& GetForwardVector() const;
	float GetRotation() const;

	void SetDir(MoveDir nDir);

	void CheckCollision(float time, const sf::Vector2f& oldPos, std::vector<Wall>& walls);
	void Collision(float time, const sf::Vector2f& iPoint, const sf::Vector2f& oldPos, const Wall& wall);
	void Update(float time, std::vector<Wall>& walls);
	void Rotate(float angle);
	void Rotate(const sf::Vector2f& mousePos);
	bool Move(float time);	
};

