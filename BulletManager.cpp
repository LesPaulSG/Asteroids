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

void BulletManager::CreateWall(const sf::Vector2f& start, const sf::Vector2f& end, bool destructable) {
	if (Line(start, end).lenght > 10) {										//check lengtn of wall for visible value	TODO add static method for lenght
		AddWall(Wall(start, end, destructable));
	}
}

void BulletManager::Update(float time) {
	player.Update(time, walls);
	
	if (!tasks.empty()) {
		std::lock_guard lg(bmMutex);
		while (!tasks.empty()) {
			auto current_task = tasks.front();
			tasks.pop();
			switch (current_task.type)
			{
			case TaskType::ADD_BULLET:
				Fire(std::move(current_task.pt.ab.pos), std::move(current_task.pt.ab.dir), current_task.pt.ab.speed, current_task.pt.ab.lifeTime);
				break;
			case TaskType::ADD_WALL:
				CreateWall(std::move(current_task.pt.aw.pos), std::move(current_task.pt.aw.dir), current_task.pt.aw.destructable);
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

void BulletManager::Fire(const sf::Vector2f& pos, const sf::Vector2f& dir, float speed, float lifeTime) {
	if (bullets.size() < BULLETS_MAX_CAPACITY) {
		bullets.push_back(Bullet(pos, dir, std::min(speed, 10.f), lifeTime));
	}
}

void BulletManager::WallTrancform() {
	for (auto& iter : walls) {
		iter.Transform();
	}
}