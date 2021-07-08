#include <cmath>

#include "BulletManager.h"

std::uniform_int_distribution<int> xDistr(50, WIDTH - 50);
std::uniform_int_distribution<int> yDistr(50, HEIGHT -50);
std::uniform_int_distribution<int> dDistr(-30, 30);

BulletManager::BulletManager() :
	player(new Player(PLAYER_DEFAULT_POS, 1.57)),
	saucerSpawned(false) {
	bullets.reserve(BULLETS_MAX_CAPACITY);
	actors.reserve(1000);
	actors.push_back(player);
}

const std::vector<Bullet>& BulletManager::GetBullets() const {return bullets;}

std::mutex& BulletManager::GetBmMutex() {return bmMutex;}

Player& BulletManager::GetPlayer() {return *player;}

int BulletManager::GetScore() {return score;}

int BulletManager::GetPlayerLives(){
	return player->GetLives();
}

void BulletManager::StartGame(){
	player = new Player(PLAYER_DEFAULT_POS, 1.57),
	saucerSpawned = false;
	bullets.clear();
	actors.clear();
	actors.push_back(player);
}

void BulletManager::Shoot() {
	std::lock_guard lg(bmMutex);
	shots.push(Shot(Shot(player->GetPos(), player->GetRotation())));
}

void BulletManager::Update(float time) {
	//player.Move(time, actors);

	if (!shots.empty()) {
		std::lock_guard lg(bmMutex);
		while (!shots.empty()) {
			Fire(shots.front(), 20); //player radius 
			shots.pop();
		}
	}
	{
		std::lock_guard lg(bmMutex);
		bullets.erase(std::remove_if(bullets.begin(),
									bullets.end(),
									[](Bullet b) {return !b.isAlive(); }),
					bullets.end());

		for (auto iter : actors) {
			if (dynamic_cast<Asteroid*>(iter)) {
				if (!iter->isAlive()) {
					int stage = dynamic_cast<Asteroid*>(iter)->GetStage();
					explosions.push(iter->GetPos());
					UpdateScore(stage);
					PlaySound(Sound(1 + stage));
					if (dynamic_cast<Asteroid*>(iter)->GetStage() > 1) {
						CrackAsteroid(iter->GetPos(), --stage);
					}

				}
			}
		}
		if (saucerSpawned && !saucer->isAlive()) {
			saucerSpawned = false;
			explosions.push(saucer->GetPos());
		}
		actors.erase(std::remove_if(actors.begin(),
									actors.end(),
									[](Actor* a) {return !a->isAlive(); }),
					actors.end());
	}

	GenerateAsteroid(time);
	SpawnSaucer(time);
	
	for (auto iter : actors) {
		iter->Move(time, actors);
	}
	if (saucerSpawned && saucer->CanShoot()) {
		Fire(saucer->GetShoot(player->GetPos()), saucer->GetRadius());
	}
	for (auto& iter : bullets) {
		iter.Update(time, actors);
	}
}

void BulletManager::Fire(Shot sho, float pushDist) {
	if (bullets.size() < BULLETS_MAX_CAPACITY) {
		bullets.push_back(Bullet(sho));
		bullets.back().PushForward(pushDist);
		PlaySound(Sound::FIRE);
	}
}

void BulletManager::GenerateAsteroid(float deltaTime){
	static sf::Vector2f pos;
	static sf::Vector2f dir;

	if (actors.size() < 2) {
		if (Delay(deltaTime, 3.f)) {
			for (int i = 0; i < 10; ++i) {
				pos.x = xDistr(gen);
				pos.y = yDistr(gen);
				dir.x = dDistr(gen) / 30.f;
				dir.y = dDistr(gen) / 30.f;
				actors.push_back(new Asteroid(pos, dir));  //initial stage == 3
			}
		}
	}
}

void BulletManager::SpawnSaucer(float deltaTime){
	if (!saucerSpawned ) {
		if(Delay(deltaTime, 5.5f)) {
			bool bigProb = randomBool(gen);
			bool spawnBig = (score > 40'000 || bigProb) ? false : true;
			sf::Vector2f startPos;
			startPos.x = randomBool(gen) ? (WIDTH - 0.1f) : 0.1f;
			startPos.y = yDistr(gen);
			saucer = new Saucer(startPos, spawnBig);
			actors.push_back(saucer);
			saucerSpawned = true;
		}
	}
}

void BulletManager::CrackAsteroid(const sf::Vector2f& pos, int stage) {
	int num = 4 / stage;
	static sf::Vector2f dir;
	for (int i = 0; i < num; ++i) {
		dir.x = dDistr(gen) / 10.f * stage;
		dir.y = dDistr(gen) / 10.f * stage;
		actors.push_back(new Asteroid(pos+dir*10.f, dir, stage)); //small offset 
	}
}

void BulletManager::Draw(sf::RenderWindow& w){
	std::lock_guard guard(bmMutex);
	for (auto& iter : bullets) {
		w.draw(iter.GetBody());
	}
	for (auto iter : actors) {
		iter->Draw(w);
	}
}

void BulletManager::UpdateScore(int stage){
	static int bonusLife = 0;
	score += 300 / stage;
	bonusLife += 300 / stage;
	if (bonusLife >= 10'000) {
		player->BonusLife();
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
