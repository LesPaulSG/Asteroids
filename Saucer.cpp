#include "Saucer.h"

Saucer::Saucer(sf::Vector2f p, bool b): 
	Actor(p, sf::Vector2f(0.1, 0.1), BIG_SAUCER_PATTERN),
	cooldown(0.f) {
	radius = 30;
	LoopSound(big ? Sound::SOUC_B : Sound::SOUC_S);
}

void Saucer::Move(float time, std::vector<Actor*>& asteroids){
	Actor::Move(time, asteroids);
	cooldown += time;
}


bool Saucer::CanShoot(){
	if (cooldown >= 1.f) {
		cooldown = 0.f;
		return true;
	}
	return false;
}

void Saucer::Destroy(){
	EndSoundLoop(big ? Sound::SOUC_B : Sound::SOUC_S);
	Actor::Destroy();
}

Shot Saucer::GetShoot(sf::Vector2f playerPos){
	float angle = Line::Rotation(playerPos, pos);
	sf::Vector2f from(radius, 0);
	if (playerPos.x < pos.x) angle *= -1.f;
	RotateVector(from, angle);
	return Shot(from+pos, angle);
}