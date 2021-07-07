#pragma once
#include <vector>
#include <queue>

#include "Bullet.h"
#include "Player.h"

enum class TaskType {
	ADD_BULLET
};

struct AddBullet {//: public PackagedTask {
	sf::Vector2f pos;
	float dir;
	float speed;
	float lifeTime;
};

union PackagedTask {
	AddBullet ab;
};

struct Task {
	TaskType type;
	PackagedTask pt;
};

class BulletManager {
private:
	std::vector<Bullet> bullets;
	std::vector<Asteroid> asteroids;

	std::queue<Task> tasks;
	std::queue<sf::Vector2f> explosions;

	mutable std::mutex bmMutex;

	Player player;

	int score = 0;

public:
	BulletManager();
	~BulletManager() = default;

	const std::vector<Bullet>& GetBullets() const;
	std::mutex& GetBmMutex();
	Player& GetPlayer();
	int GetScore();
	int GetPlayerLives();

	void AddTask(Task& pt);
	void Update(float time);
	void Fire(const sf::Vector2f& pos, float dir, float speed, float lifeTime);
	void GenerateAsteroid(float deltaTime);
	void CrackAsteroid(sf::Vector2f& pos, int stage);
	void Draw(sf::RenderWindow& w);
	void UpdateScore(int stage);

	bool isExplosions();
	sf::Vector2f PopExplosion();
};
