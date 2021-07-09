#include <SFML/Audio.hpp>

#include "Functions.h"

std::vector<sf::SoundBuffer> buffers;
std::vector<sf::Sound> sounds;

sf::Font font;

void LoadSounds() {
	font.loadFromFile("vector.ttf");

	buffers.reserve(soundFileNames.size());
	sounds.reserve(soundFileNames.size());

	for (auto& name : soundFileNames) {
		buffers.push_back(sf::SoundBuffer());
		buffers.back().loadFromFile("sounds/" + name + ".wav");
		sounds.push_back(sf::Sound(buffers.back()));
	}
}

void PlaySound(Sound s) {
	sounds[(int)s].play();
}

void LoopSound(Sound s) {
	sounds[(int)s].setLoop(true);
	PlaySound(s);
}

void EndSoundLoop(Sound s){
	sounds[(int)s].stop();
}

const sf::Font& GetFont() {
	return font;
}

bool isPointInRange(float x, float a, float b) {
	return (x >= std::min(a, b) && x <= std::max(a, b));
}

float VectorsModule(const sf::Vector2f& vec){
	return std::sqrt(vec.x * vec.x + vec.y * vec.y);
}

void RotateVector(sf::Vector2f& vec, float angle){
	float xNew = vec.x * cos(angle) - vec.y * sin(angle);
	float yNew = vec.x * sin(angle) + vec.y * cos(angle);
	vec.x = xNew;
	vec.y = yNew;
}

void RotateVectorAround(sf::Vector2f& vec, const sf::Vector2f& anchor, float angle) {
	float xNew = (vec.x - anchor.x) * cos(angle) - (vec.y - anchor.y) * sin(angle) + anchor.x;
	float yNew = (vec.x - anchor.x) * sin(angle) + (vec.y - anchor.y) * cos(angle) + anchor.y;
	vec.x = xNew;
	vec.y = yNew;
}

void RotateUnitVector(sf::Vector2f& vec, float angle){
	vec.x = sin(angle);
	vec.y = -cos(angle);
}

bool PassScreenBorder(sf::Vector2f& vec) {
	bool passed = false;
	if (vec.x >= WIDTH) {
		vec.x -= WIDTH;
		passed = true;
	}
	if (vec.x <= 0) {
		vec.x += WIDTH;
		passed = true;
	}
	if (vec.y >= HEIGHT) {
		vec.y -= HEIGHT;
		passed = true;
	}
	if (vec.y <= 0) {
		vec.y += HEIGHT;
		passed = true;
	}
	return passed;
}