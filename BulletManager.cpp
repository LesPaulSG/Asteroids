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
	gameRunning = true;
	std::lock_guard lg(bmMutex);
	actors.clear();
	//player = std::move(new Player(PLAYER_DEFAULT_POS, 1.57));
	//player->Refresh(); //kostyl
	player->Reset();
	actors.push_back(player);
	score = 0;
	saucerSpawned = false;
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
				if(iter->isPlayerDestoyed()) UpdateScore(stage); 
			}
		}
	}
	if (saucerSpawned && !saucer->isAlive()) {
		saucerSpawned = false;
		explosions.push(saucer->GetPos());
		if(saucer->isPlayerDestoyed()) UpdateScore(saucer->isBig() ? 2 : 1);
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
	static sf::Vector2f dir;
	static Delay del(waitTime);
	if (actors.size() < 2) {
		if (del.Wait(deltaTime)) {
			for (int i = 0; i < 10; ++i) {
				dir.x = RAND_DIR(gen) * 15.f / 3.f;
				dir.y = RAND_DIR(gen) * 15.f / 3.f;
				actors.push_back(new Asteroid(sf::Vector2f(RAND_X(gen), RAND_Y(gen)), dir));  //initial stage == 3
			}
		}
	}
}

void BulletManager::SpawnSaucer(float deltaTime){
	static Delay del(5.5f);
	if (!saucerSpawned ) {
		if(del.Wait(deltaTime)) {
			bool spawnBig = (score > 40'000 || RAND_BOOL(gen)) ? false : true;
			sf::Vector2f startPos;
			startPos.x = RAND_BOOL(gen) ? (WIDTH - 0.1f) : 0.1f;
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
		dir.x = RAND_DIR(gen) * 15.f / stage;
		dir.y = RAND_DIR(gen) * 15.f / stage;
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

void BulletManager::UpdateScore(int stage){
	static int bonusLife = 0;
	score += 300 / stage;
	if (bonusLife < score/10'000) {
		player->BonusLife();
		++bonusLife;
	}
}

void BulletManager::Clear(){
	std::lock_guard lg(bmMutex);
	if (saucerSpawned) saucer->Destroy();
	bullets.clear();
	actors.clear();
	//player->~Player();
	//player = nullptr;
	gameRunning = false;

	/*for (auto& iter : actors) {
		iter->Destroy();
	}*/
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
