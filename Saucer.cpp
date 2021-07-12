#include "Saucer.h"

Saucer::Saucer(sf::Vector2f p, bool b) : 
		Actor(p, sf::Vector2f(0.f, 0.f), b ? BIG_SAUCER_PATTERN : SMALL_SAUCER_PATTERN),
		big(b),
		cooldown(0.f)
{
	speed = big ? 150.f : 300.f;
	dir = (pos.x > WIDTH / 2) ? sf::Vector2f(-1.f, 0.f) :
								sf::Vector2f( 1.f, 0.f);
	LoopSound(big ? Sound::SOUC_B : Sound::SOUC_S);
}

void Saucer::Move(float time, std::vector<Actor*>& actors){
	if (!actors.empty()) {
		float yDist = std::abs(actors.at(0)->GetPos().y - pos.y);
		if (yDist > HEIGHT * 0.2f) {
			if (actors.at(0)->GetPos().y > pos.y) dir.y = 1.f;
			else dir.y = -1.f;
		}
		else dir.y = 0.f;
	}
	if (isPassingVerBrd(pos + dir)) {
		dir.x *= -1.f;
		pos.x += dir.x * 10.f;
	}
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

void Saucer::Destroy(bool playerDestroy){
	EndSoundLoop(big ? Sound::SOUC_B : Sound::SOUC_S);
	Actor::Destroy(playerDestroy);
}

bool Saucer::isBig(){return big;}

Shot Saucer::GetShoot(sf::Vector2f playerPos){
	if (big) {
		return Shot(pos, RAND_ANGLE_360(gen));
	}
	float angle = Line::Rotation(playerPos, pos) + RAND_DISPERSION(gen);
	if (playerPos.x < pos.x) angle *= -1.f;
	return Shot(pos, angle);
}