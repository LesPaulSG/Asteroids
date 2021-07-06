#include "Explosion.h"

std::uniform_int_distribution<int> pos(0, 10);
std::uniform_int_distribution<int> dir(-10, 10);

Explosion::Explosion(sf::Vector2f p)
	: lifeTime(0.f) {
	sparks.reserve(NUM_OF_SPARKS);
	offsets.reserve(NUM_OF_SPARKS);
	sf::CircleShape sh;
	sf::Vector2f ofs;
	for (int i = 0; i < NUM_OF_SPARKS; ++i) {
		sh.setFillColor(sf::Color::White);
		sh.setPosition(sf::Vector2f(pos(gen), pos(gen))+p);
		sh.setRadius(1);
		sparks.push_back(sh);
		ofs.x = dir(gen) / 10.f;
		ofs.y = dir(gen) / 10.f;
		offsets.push_back(ofs);
	}
}

void Explosion::Update(float deltaTime){
	for (int i = 0; i < NUM_OF_SPARKS; ++i) {
		sparks[i].setPosition(sparks[i].getPosition() + offsets[i]);
		sparks[i].setFillColor(sf::Color(255, 255, 255, 255 - lifeTime*85));
	}
	lifeTime += deltaTime;
}

void Explosion::Draw(sf::RenderWindow& w){
	for (auto& iter : sparks) {
		w.draw(iter);
	}
}

bool Explosion::isAlive(){
	if (lifeTime < 3) {
		return true;
	}
	return false;
}
