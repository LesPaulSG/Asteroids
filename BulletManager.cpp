#include <cmath>
#include <iostream>

#include "BulletManager.h"

std::random_device rd_;
std::mt19937_64 gen_(rd_());
//std::default_random_engine generator;
std::uniform_int_distribution<int> xDistr(50, 1870);
std::uniform_int_distribution<int> yDistr(50, 1230);
std::uniform_int_distribution<int> dDistr(-10, 10);

BulletManager::BulletManager() : player(sf::Vector2f(960, 540), 1.57) {
	bullets.reserve(BULLETS_MAX_CAPACITY);
	walls.reserve(WALLS_MAX_CAPACITY);
	asteroids.reserve(ASTEROIDS_MAX_QUANTITY);
}

const std::vector<Bullet>& BulletManager::GetBullets() const {return bullets;}

const std::vector<Wall>& BulletManager::GetWalls() const {return walls;}

std::mutex& BulletManager::GetBmMutex() {return bmMutex;}

Player& BulletManager::GetPlayer() {return player;}

void BulletManager::AddTask(Task& pt) {
	std::lock_guard lg(bmMutex);
	tasks.push(pt);
}

void BulletManager::AddWall(Wall&& wall) {
	if (walls.size() < WALLS_MAX_CAPACITY) {
		walls.push_back(wall);
	}
}

void BulletManager::CreateWall(const sf::Vector2f& start, const sf::Vector2f& end) {
	if (Line(start, end).lenght > 10) {										//check lengtn of wall for visible value	TODO add static method for lenght
		AddWall(Wall(start, end));
	}
}

void BulletManager::Update(float time) {
	player.Update(time, walls);
	
	if (!tasks.empty()) {
		std::lock_guard lg(bmMutex);
		while (!tasks.empty()) {
			auto cTask = tasks.front();
			tasks.pop();
			switch (cTask.type)
			{
			case TaskType::ADD_BULLET:
				Fire(std::move(cTask.pt.ab.pos), cTask.pt.ab.dir, cTask.pt.ab.speed, cTask.pt.ab.lifeTime);
				break;
			case TaskType::ADD_WALL:
				CreateWall(std::move(cTask.pt.aw.pos), std::move(cTask.pt.aw.dir));
				break;
			default:
				break;
			}
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
		for (auto iter = walls.begin(); iter != walls.end(); ++iter) {
			if (!iter->GetAlive()) {
				walls.erase(iter);
				break;
			}
		}
		for (auto iter = asteroids.begin(); iter != asteroids.end(); ++iter) {
			if (!iter->GetAlive()) {
				int stage = iter->GetStage();
				sf::Vector2f pos = iter->GetPos();
				asteroids.erase(iter);
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
			pos.x = xDistr(gen_);
			pos.y = yDistr(gen_);
			dir.x = dDistr(gen_) / 10.f;
			dir.y = dDistr(gen_) / 10.f;
			asteroids.push_back(Asteroid(pos, dir));  //initial stage == 3
			timePassed = 0;
		}
	}
}

void BulletManager::CrackAsteroid(sf::Vector2f& pos, int stage) {
	int num = 4 / stage;	//stage == 2 -> num == 4; stage == 3 -> num == 2; not a bug, but a feature
	static sf::Vector2f dir;
	for (int i = 0; i < num; ++i) {
		dir.x = dDistr(gen_) / 10.f;
		dir.y = dDistr(gen_) / 10.f;
		asteroids.push_back(Asteroid(pos+dir*10.f, dir, stage));  //initial stage == 3
	}
}

void BulletManager::Draw(sf::RenderWindow& w){
	std::lock_guard guard(bmMutex);
	for (auto& iter : bullets) {
		w.draw(iter.GetBody());
	}
	for (auto& iter : walls) {
		w.draw(iter.GetBody());
	}
	for (auto& iter : asteroids) {
		iter.Draw(w);
	}
	player.Draw(w);
}
