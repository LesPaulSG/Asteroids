#include "Explosion.h"

Explosion::Explosion(sf::Vector2f p) :
		lifeTime(0.f)
{
	sparks.reserve(NUM_OF_SPARKS);
	offsets.reserve(NUM_OF_SPARKS);
	sf::CircleShape sh;
	sf::Vector2f ofs;
	for (int i = 0; i < NUM_OF_SPARKS; ++i) {
		sh.setFillColor(sf::Color::White);
		sh.setPosition(p);
		sh.setRadius(1);
		sparks.push_back(sh);
		ofs.x = RAND_DIR(gen);
		ofs.y = RAND_DIR(gen);
		offsets.push_back(ofs);
	}
}

void Explosion::Update(float deltaTime){
	for (int i = 0; i < NUM_OF_SPARKS; ++i) {
		sparks[i].setPosition(sparks[i].getPosition() + offsets[i]);
		sparks[i].setFillColor(sf::Color(255, 255, 255, 255 - lifeTime*170));
	}
	lifeTime += deltaTime;
}

void Explosion::Draw(sf::RenderWindow& w){
	for (auto& iter : sparks) {
		w.draw(iter);
	}
}

bool Explosion::isAlive(){
	if (lifeTime < 1.5f) {
		return true;
	}
	return false;
}
