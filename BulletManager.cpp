#include <cmath>
#include <iostream>

#include "BulletManager.h"

BulletManager::BulletManager() : player(sf::Vector2f(960, 540), 0) {
	bullets.reserve(BULLETS_MAX_CAPACITY);
	walls.reserve(WALLS_MAX_CAPACITY);
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
	}

	for (auto iter = bullets.begin(); iter != bullets.end(); ++iter) {
		iter->Update(time, walls);
	}
}

void BulletManager::Fire(const sf::Vector2f& pos, float dir, float speed, float lifeTime) {
	if (bullets.size() < BULLETS_MAX_CAPACITY) {
		bullets.push_back(Bullet(pos, dir, std::max(speed, 10.f), lifeTime));
	}
}