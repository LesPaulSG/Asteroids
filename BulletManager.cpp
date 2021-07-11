#include <cmath>

#include "BulletManager.h"

BulletManager::BulletManager() :
		player(new Player(PLAYER_DEFAULT_POS, 1.57)),
		saucerSpawned(false),
		gameRunning(false)
{
	bullets.reserve(BULLETS_MAX_CAPACITY);
	actors.reserve(ASTEROIDS_MAX_QUANTITY);
	GenerateAsteroid(0, 0);
}

Player& BulletManager::GetPlayer() const {return *player;}

int BulletManager::GetScore() const {return score;}

int BulletManager::GetPlayerLives() const {return player->GetLives();}

void BulletManager::StartGame(){
	player->Reset();
	gameRunning = true;
	score = 0;
	saucerSpawned = false;

	std::lock_guard lg(bmMutex);
	actors.clear();
	actors.push_back(player);
}

void BulletManager::Shoot() {
	std::lock_guard lg(bmMutex);
	shots.push(Shot(Shot(player->GetPos(), player->GetRotation())));
}

void BulletManager::Update(float time) {
	if (!shots.empty()) {
		std::lock_guard lg(bmMutex);
		while (!shots.empty()) {
			Fire(shots.front(), player->GetBodyRadius(), true);
			shots.pop();
		}
	}
	
	for (auto iter : actors) {
		if (dynamic_cast<Asteroid*>(iter)) {
			if (!iter->isAlive()) {
				int stage = dynamic_cast<Asteroid*>(iter)->GetStage();
				explosions.push(iter->GetPos());
				PlaySound(Sound(1 + stage));
				if (stage > 1) {
					CrackAsteroid(iter->GetPos(), --stage);
				}
				if(iter->isPlayerDestoyed()) UpdateScore(dynamic_cast<Asteroid*>(iter)->GetCost());
			}
		}
	}
	if (saucerSpawned && !saucer->isAlive()) {
		saucerSpawned = false;
		explosions.push(saucer->GetPos());
		if(saucer->isPlayerDestoyed()) UpdateScore(saucer->isBig() ? Values::SAUC_B : Values::SAUC_S);
	}
	{
		std::lock_guard lg(bmMutex);
		bullets.erase(std::remove_if(bullets.begin(), 
									bullets.end(),
									[](Bullet b) {return !b.isAlive(); }),
					bullets.end());
		actors.erase(std::remove_if(actors.begin(),
									actors.end(),
									[](Actor* a) {return !a->isAlive(); }),
					actors.end());
	}

	GenerateAsteroid(time, 3);
	if(gameRunning) SpawnSaucer(time);
	
	if (saucerSpawned && saucer->CanShoot()) {
		Fire(saucer->GetShoot(player->GetPos()), saucer->GetBodyRadius());
	}
	for (auto iter : actors) {
		iter->Move(time, actors);
	}
	for (auto& iter : bullets) {
		iter.Update(time, actors);
	}
}

void BulletManager::Fire(Shot sho, float pushDist, bool playerShoot) {
	if (bullets.size() < BULLETS_MAX_CAPACITY) {
		bullets.push_back(Bullet(sho, playerShoot));
		bullets.back().PushForward(pushDist);
		PlaySound(Sound::FIRE);
	}
}

void BulletManager::GenerateAsteroid(float deltaTime, float waitTime){
	static sf::Vector2f pos, dir;
	static float timePassed = 0;
	int quntuty = (score > 40'000) ? 15 : 10;
	if (actors.size() < 2) {
		if ((timePassed+=deltaTime) >= waitTime) {
			timePassed = 0.f;
			for (int i = 0; i < quntuty; ++i) {
				if (RAND_BOOL(gen)) {
					pos.x = RAND_BOOL(gen) ? (WIDTH) : 0.f;
					pos.y = RAND_Y(gen);
				}
				else {
					pos.x = RAND_X(gen);
					pos.y = RAND_BOOL(gen) ? (HEIGHT) : 0.f;
				}
				dir.x = RAND_DIR(gen) * 35.f / 3.f;
				dir.y = RAND_DIR(gen) * 35.f / 3.f;
				actors.push_back(new Asteroid(pos, dir));  //initial stage == 3
			}
		}
	}
}

void BulletManager::SpawnSaucer(float deltaTime){
	static Delay del(15.f);
	static sf::Vector2f startPos;
	if (!saucerSpawned) {
		if(del.Wait(deltaTime) && actors.size() > 1) {
			bool spawnBig = (score > 40'000 || RAND_BOOL(gen)) ? false : true;
			startPos.x = RAND_BOOL(gen) ? (WIDTH) : 0.f;
			startPos.y = RAND_Y(gen);
			saucer = new Saucer(startPos, spawnBig);
			actors.push_back(saucer);
			saucerSpawned = true;
		}
	}
}

void BulletManager::CrackAsteroid(const sf::Vector2f& pos, int stage) {
	static sf::Vector2f dir;
	for (int i = 0; i < 2; ++i) {
		dir.x = RAND_DIR(gen) * 35.f / stage;
		dir.y = RAND_DIR(gen) * 35.f / stage;
		actors.push_back(new Asteroid(pos+dir*10.f, dir, stage)); //small offset 
	}
}

void BulletManager::Draw(sf::RenderWindow& w) const {
	std::lock_guard guard(bmMutex);
	for (auto& iter : bullets) {
		w.draw(iter.GetBody());
	}
	for (auto iter : actors) {
		if (gameRunning) {
			iter->Draw(w);
		}
		else if (!dynamic_cast<Player*>(iter)) {
			iter->Draw(w);
		}
		
	}
}

void BulletManager::UpdateScore(Values val){
	static int bonusLife = 0;
	score += (int)val;
	if (bonusLife < score/10'000) {
		player->BonusLife();
		++bonusLife;
	}
	if (score > 99'990) score = 0;
}

void BulletManager::Clear(){
	std::lock_guard lg(bmMutex);
	if (saucerSpawned) saucer->Destroy();
	bullets.clear();
	actors.clear();
	gameRunning = false;
}

bool BulletManager::isExplosions() {
	return !explosions.empty();
}

const sf::Vector2f& BulletManager::PopExplosion(){
	static sf::Vector2f res;
	res = explosions.front();
	explosions.pop();
	return res;
}
