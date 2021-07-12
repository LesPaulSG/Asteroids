#include "BulletManager.h"

BulletManager::BulletManager() :
		player(new Player(PLAYER_DEFAULT_POS, 1.57)),
		saucer(nullptr),
		saucerSpawned(false),
		gameRunning(false)
{
	bullets.reserve(BULLETS_MAX_CAPACITY);
	actors.reserve(ASTEROIDS_MAX_QUANTITY);
	GenerateAsteroid(0.f, 0.f);
}

Player& BulletManager::GetPlayer() const {return *player;}

int BulletManager::GetScore() const {return score;}

int BulletManager::GetPlayerLives() const {return player->GetLives();}

bool BulletManager::isPlayerHasLives() const {
	return player->GetLives() > 0;
}

void BulletManager::StartGame(){
	player->Reset();
	score = 0;
	gameRunning = true;
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
			Fire(shots.front(), player->GetRadius(), true);
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
	//if(gameRunning) SpawnSaucer(time);
	
	if (player->CanMove()) {
		if (saucerSpawned && saucer->CanShoot()) {
			Fire(saucer->GetShoot(player->GetPos()), saucer->GetRadius());
		}
	}
	for (auto iter : actors) {
		iter->Update(time, actors);
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
	static float passedTime = 0.f;
	int quntuty = (score > 40'000) ? 15 : 10;
	if (actors.size() < 2) {
		if ((passedTime +=deltaTime) >= waitTime) {
			passedTime = 0.f;
			for (int i = 0; i < quntuty; ++i) {
				if (RAND_BOOL(gen)) {
					pos.x = RAND_BOOL(gen) ? (WIDTH) : 0.f;
					pos.y = RAND_Y(gen);
				}
				else {
					pos.x = RAND_X(gen);
					pos.y = RAND_BOOL(gen) ? (HEIGHT) : 0.f;
				}
				dir.x = RAND_DIR(gen);
				dir.y = RAND_DIR(gen);
				actors.push_back(new Asteroid(pos, dir));  //initial stage == 3
			}
		}
	}
}

void BulletManager::SpawnSaucer(float deltaTime){
	static float passedTime;
	static sf::Vector2f startPos;
	if (!saucerSpawned) {
		if((passedTime += deltaTime) > 15.f && actors.size() > 1) {
			passedTime = 0.f;
			bool spawnBig = !(score > 40'000 || RAND_BOOL(gen));
			startPos.x = RAND_BOOL(gen) ? WIDTH : 0.f;
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
		dir.x = RAND_DIR(gen);
		dir.y = RAND_DIR(gen);
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
	saucerSpawned = false;
	saucer == nullptr;
	bullets.clear();
	actors.clear();
	GenerateAsteroid(0.f, 0.f);
	explosions = std::queue<sf::Vector2f>();
	gameRunning = false;
}

bool BulletManager::isExplosions() const {
	return !explosions.empty();
}

const sf::Vector2f& BulletManager::PopExplosion(){
	static sf::Vector2f res;
	res = explosions.front();
	explosions.pop();
	return res;
}
