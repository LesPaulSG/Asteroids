#include "Explosion.h"

Explosion::Explosion(sf::Vector2f p) :
		lifeTime(0.f)
{
	sparks.reserve(NUM_OF_SPARKS);
	offsets.reserve(NUM_OF_SPARKS);

	for (unsigned i = 0; i < NUM_OF_SPARKS; ++i) {
		sparks.push_back(sf::CircleShape(1.5f));
		sparks.back().setFillColor(sf::Color::White);
		sparks.back().setPosition(p);
		offsets.push_back(sf::Vector2f(RAND_DIR(gen), RAND_DIR(gen)));
	}
}

void Explosion::Update(float deltaTime){
	for (unsigned i = 0; i < NUM_OF_SPARKS; ++i) {
		sparks.at(i).setPosition(sparks.at(i).getPosition() + offsets.at(i) * deltaTime * 350.f);
		sparks.at(i).setFillColor(sf::Color(255, 255, 255, 255 - lifeTime * 170));
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
