#include "Saucer.h"

Saucer::Saucer(sf::Vector2f p, bool b)
	: pos(p),
	dir(0.f, 0.f),
	speed(150.f),
	cooldown(0.f),
	body(pos, {
	sf::Vector2f(0, -20),
	sf::Vector2f(30, 0),
	sf::Vector2f(0, 20),
	sf::Vector2f(-30, 0) }),
	big(b) {

}

void Saucer::Update(float deltaTime){
	cooldown += deltaTime;
	pos += dir * speed * deltaTime;
	body.Move(pos);
}

void Saucer::Draw(sf::RenderWindow& w){
	body.Draw(w);
}

bool Saucer::CanShoot(float deltaTime){
	if (cooldown >= 1.f) {
		cooldown = 0.f;
		return true;
	}
	return false;
}
