#pragma once
#include "Asteroid.h"

enum MoveDir{ STP, BST, RGH, LFT };

class Player{
private:
	sf::Vector2f pos;
	sf::Vector2f forwardVector;
	sf::Vector2f rightVector;
	float rotation;
	float speed;
	float rotSpeed;
	float radius;
	float force;
	MoveDir dir;
	int lives;

	Polygon body;

public:
	Player(sf::Vector2f pos, float rotation);
	~Player() = default;

	//const sf::CircleShape& GetBody() const;
	const sf::Vector2f& GetPosition() const;
	const sf::Vector2f& GetForwardVector() const;
	float GetRotation() const;
	int GetLives();

	void SetDir(MoveDir nDir);
	void Draw(sf::RenderWindow& w);

	void CheckCollision(float time, std::vector<Asteroid>& asteroids);
	void Collision(float time, const sf::Vector2f& iPoint, const sf::Vector2f& oldPos, const Wall& wall);
	void Update(float time, std::vector<Asteroid>& asteroids);
	void Rotate(float angle);
	bool Move(float time);	
	void BonusLife();
	void Refresh();
};

