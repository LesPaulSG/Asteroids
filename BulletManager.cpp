#include <cmath>

#include "BulletManager.h"

std::uniform_int_distribution<int> xDistr(50, WIDTH - 50);
std::uniform_int_distribution<int> yDistr(50, HEIGHT -50);
std::uniform_int_distribution<int> dDistr(-30, 30);

BulletManager::BulletManager() : player(PLAYER_DEFAULT_POS, 1.57) {
	bullets.reserve(BULLETS_MAX_CAPACITY);
	asteroids.reserve(ASTEROIDS_MAX_QUANTITY);
}

const std::vector<Bullet>& BulletManager::GetBullets() const {return bullets;}

std::mutex& BulletManager::GetBmMutex() {return bmMutex;}

Player& BulletManager::GetPlayer() {return player;}

int BulletManager::GetScore() {return score;}

int BulletManager::GetPlayerLives(){
	return player.GetLives();
}

void BulletManager::AddTask(Task& pt) {
	std::lock_guard lg(bmMutex);
	tasks.push(pt);
}

void BulletManager::Update(float time) {
	player.Update(time, asteroids);
	
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
				UpdateScore(stage);
				PlaySound(Sound(1 + stage));
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
		bullets.push_back(Bullet(pos, dir, 250.f, lifeTime));
		PlaySound(Sound::FIRE);
	}
}

void BulletManager::GenerateAsteroid(float deltaTime){
	static sf::Vector2f pos;
	static sf::Vector2f dir;

	if (asteroids.empty()) {
		for (int i = 0; i < 10; ++i) {
			pos.x = xDistr(gen);
			pos.y = yDistr(gen);
			dir.x = dDistr(gen) / 30.f;
			dir.y = dDistr(gen) / 30.f;
			asteroids.push_back(Asteroid(pos, dir));  //initial stage == 3
		}
	}

}

void BulletManager::CrackAsteroid(sf::Vector2f& pos, int stage) {
	int num = 4 / stage;
	static sf::Vector2f dir;
	for (int i = 0; i < num; ++i) {
		dir.x = dDistr(gen) / 10.f * stage;
		dir.y = dDistr(gen) / 10.f * stage;
		asteroids.push_back(Asteroid(pos+dir*10.f, dir, stage)); //small offset 
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

void BulletManager::UpdateScore(int stage){
	static int bonusLife = 0;
	score += 300 / stage;
	bonusLife += 300 / stage;
	if (bonusLife >= 10'000) {
		player.BonusLife();
		bonusLife = 0;
	}
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
