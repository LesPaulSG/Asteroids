#pragma once
#include <vector>
#include <queue>

#include "Bullet.h"
#include "Player.h"
#include "Saucer.h"

class BulletManager {
private:
	std::vector<Bullet> bullets;
	std::vector<Actor*> actors;

	std::queue<Shot> shots;
	std::queue<sf::Vector2f> explosions;

	mutable std::mutex bmMutex;

	Player* player;

	int score = 0;
	bool saucerSpawned;
	bool gameRunning;

	Saucer* saucer;

public:
	BulletManager();
	~BulletManager() = default;

	Player& GetPlayer() const;
	int GetScore() const;
	int GetPlayerLives() const;

	void StartGame();
	void Shoot();
	void Update(float time);
	void Fire(Shot sho, float pushDist, bool playerShoot = false);
	void GenerateAsteroid(float deltaTime, float waitTime);
	void SpawnSaucer(float deltaTime);
	void CrackAsteroid(const sf::Vector2f& pos, int stage);
	void Draw(sf::RenderWindow& w) const;
	void UpdateScore(int stage);
	void Clear();

	bool isExplosions();
	const sf::Vector2f& PopExplosion();
};
