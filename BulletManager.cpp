#include <cmath>

#include "BulletManager.h"

std::uniform_int_distribution<int> xDistr(50, 1870);
std::uniform_int_distribution<int> yDistr(50, 1230);
std::uniform_int_distribution<int> dDistr(-10, 10);

BulletManager::BulletManager() : player(sf::Vector2f(960, 540), 1.57) {
	bullets.reserve(BULLETS_MAX_CAPACITY);
	asteroids.reserve(ASTEROIDS_MAX_QUANTITY);
}

const std::vector<Bullet>& BulletManager::GetBullets() const {return bullets;}

std::mutex& BulletManager::GetBmMutex() {return bmMutex;}

Player& BulletManager::GetPlayer() {return player;}

void BulletManager::AddTask(Task& pt) {
	std::lock_guard lg(bmMutex);
	tasks.push(pt);
}

void BulletManager::Update(float time) {
	player.Update(time);
	
	if (!tasks.empty()) {
		std::lock_guard lg(bmMutex);
		while (!tasks.empty()) {
			auto cTask = tasks.front();
			tasks.pop();
			Fire(std::move(cTask.pt.ab.pos), cTask.pt.ab.dir, cTask.pt.ab.speed, cTask.pt.ab.lifeTime);
		}
	}
	{
		std::lock_guard lg(bmMutex);
		for (auto iter = bullets.begin(); iter != bullets.end(); ++iter) {
			if (!iter->GetAlive()) {
				bullets.erase(iter);
				break;
			}
		}
		for (auto iter = asteroids.begin(); iter != asteroids.end(); ++iter) {
			if (!iter->GetAlive()) {
				int stage = iter->GetStage();
				sf::Vector2f pos = iter->GetPos();
				asteroids.erase(iter);
				explosions.push(pos);
				if (stage > 1) {
					CrackAsteroid(pos, --stage);
				}
				break;
			}
		}
	}

	GenerateAsteroid(time);
	for (auto& iter : asteroids) {
		iter.Move(time, asteroids);
	}
	for (auto iter = bullets.begin(); iter != bullets.end(); ++iter) {
		iter->Update(time, asteroids);
	}
}

void BulletManager::Fire(const sf::Vector2f& pos, float dir, float speed, float lifeTime) {
	if (bullets.size() < BULLETS_MAX_CAPACITY) {
		bullets.push_back(Bullet(pos, dir, std::max(speed, 10.f), lifeTime));
	}
}

void BulletManager::GenerateAsteroid(float deltaTime){
	static float timePassed = 0;
	static sf::Vector2f pos;
	static sf::Vector2f dir;
	timePassed += deltaTime;

	if (timePassed >= 1.f) {
		if (asteroids.size() < ASTEROIDS_MAX_QUANTITY/3) {
			pos.x = xDistr(gen);
			pos.y = yDistr(gen);
			dir.x = dDistr(gen) / 10.f;
			dir.y = dDistr(gen) / 10.f;
			asteroids.push_back(Asteroid(pos, dir));  //initial stage == 3
			timePassed = 0;
		}
	}
}

void BulletManager::CrackAsteroid(sf::Vector2f& pos, int stage) {
	int num = 4 / stage;
	static sf::Vector2f dir;
	for (int i = 0; i < num; ++i) {
		dir.x = dDistr(gen) / 10.f;
		dir.y = dDistr(gen) / 10.f;
		asteroids.push_back(Asteroid(pos+dir*10.f, dir, stage));
	}
}

void BulletManager::Draw(sf::RenderWindow& w){
	std::lock_guard guard(bmMutex);
	for (auto& iter : bullets) {
		w.draw(iter.GetBody());
	}
	for (auto& iter : asteroids) {
		iter.Draw(w);
	}
	player.Draw(w);
}

bool BulletManager::isExplosions() {
	if (explosions.empty()) {
		return false;
	}
	return true;
}

sf::Vector2f BulletManager::PopExplosion(){
	sf::Vector2f res = explosions.front();
	explosions.pop();
	return res;
}
