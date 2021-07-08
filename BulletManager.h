#pragma once
#include <vector>
#include <queue>

#include "Bullet.h"
#include "Player.h"
#include "Saucer.h"

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
	std::vector<Actor*> actors;

	std::queue<Shot> shots;
	std::queue<sf::Vector2f> explosions;

	mutable std::mutex bmMutex;

	Player player;

	int score = 0;
	bool saucerSpawned;

	Saucer* saucer;

public:
	BulletManager();
	~BulletManager() = default;

	const std::vector<Bullet>& GetBullets() const;
	std::mutex& GetBmMutex();
	Player& GetPlayer();
	int GetScore();
	int GetPlayerLives();

	void Shoot();
	void Update(float time);
	void Fire(Shot sho, float pushDist);
	void GenerateAsteroid(float deltaTime);
	void SpawnSaucer(float deltaTime);
	void CrackAsteroid(const sf::Vector2f& pos, int stage);
	void Draw(sf::RenderWindow& w);
	void UpdateScore(int stage);

	bool isExplosions();
	sf::Vector2f PopExplosion();
};
