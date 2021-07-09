#include "Saucer.h"

std::uniform_real_distribution<float> shootAngle(0.f, 6.28319f);
std::uniform_real_distribution<float> dispersion(-0.05f, 0.05f);

Saucer::Saucer(sf::Vector2f p, bool b): 
	big(b),
	Actor(p, sf::Vector2f(0.f, 0.f), b ? BIG_SAUCER_PATTERN : SMALL_SAUCER_PATTERN),
	cooldown(0.f) {
	sf::Vector2f newDir = (pos.x > WIDTH / 2) ? sf::Vector2f(-1.f, 0.f) :
												sf::Vector2f( 1.f, 0.f);
	SetDirection(newDir);
	radius = 30;
	LoopSound(big ? Sound::SOUC_B : Sound::SOUC_S);
}

void Saucer::Move(float time, std::vector<Actor*>& actors){
	float yDist = std::abs(actors.at(0)->GetPos().y - pos.y);
	if (yDist > HEIGHT / 10) {
		if (actors.at(0)->GetPos().y > pos.y) dir.y = 1.f;
		else dir.y = -1.f;
	} else dir.y = 0.f;
	Actor::Move(time, actors);
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

bool Saucer::isBig(){
	return big;
}

Shot Saucer::GetShoot(sf::Vector2f playerPos){
	if (big) {
		return Shot(pos, shootAngle(gen));
	}
	float angle = Line::Rotation(playerPos, pos) + dispersion(gen);
	if (playerPos.x < pos.x) angle *= -1.f;
	return Shot(pos, angle);
}