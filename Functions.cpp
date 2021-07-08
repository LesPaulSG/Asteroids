#include <SFML/Audio.hpp>

#include "Functions.h"

std::vector<sf::SoundBuffer> buffers;
std::vector<sf::Sound> sounds;

sf::Font font;

sf::Text pressAnyKey;
sf::Text score;
sf::Text enterInitials;
sf::Text leaderBoard;
std::vector<sf::Text> leaders;

void LoadSounds() {
	font.loadFromFile("vector.ttf");
	pressAnyKey.setFont(font);
	pressAnyKey.setCharacterSize(30);
	pressAnyKey.setOutlineColor(sf::Color::White);
	pressAnyKey.setOutlineThickness(0.5);
	pressAnyKey.setPosition(5.f, 5.f);
	pressAnyKey.setFillColor(sf::Color::White);

	buffers.push_back(sf::SoundBuffer());
	buffers.push_back(sf::SoundBuffer());
	buffers.push_back(sf::SoundBuffer());
	buffers.push_back(sf::SoundBuffer());
	buffers.push_back(sf::SoundBuffer());
	buffers.push_back(sf::SoundBuffer());
	buffers.push_back(sf::SoundBuffer());
	buffers.push_back(sf::SoundBuffer());

	buffers[0].loadFromFile("sounds/fire.wav");
	buffers[1].loadFromFile("sounds/thrust.wav");
	buffers[2].loadFromFile("sounds/bangSmall.wav");
	buffers[3].loadFromFile("sounds/bangMedium.wav");
	buffers[4].loadFromFile("sounds/bangLarge.wav");
	buffers[5].loadFromFile("sounds/saucerSmall.wav");
	buffers[6].loadFromFile("sounds/saucerBig.wav");
	buffers[7].loadFromFile("sounds/extraShip.wav");	

	sounds.push_back(sf::Sound());
	sounds.push_back(sf::Sound());
	sounds.push_back(sf::Sound());
	sounds.push_back(sf::Sound());
	sounds.push_back(sf::Sound());
	sounds.push_back(sf::Sound());
	sounds.push_back(sf::Sound());
	sounds.push_back(sf::Sound());

	sounds[0].setBuffer(buffers[0]);
	sounds[1].setBuffer(buffers[1]);
	sounds[2].setBuffer(buffers[2]);
	sounds[3].setBuffer(buffers[3]);
	sounds[4].setBuffer(buffers[4]);
	sounds[5].setBuffer(buffers[5]);
	sounds[6].setBuffer(buffers[6]);
	sounds[7].setBuffer(buffers[7]);
}

void PlaySound(Sound ss) {
	sounds[(int)ss].play();
}

void LoopSound(Sound ss) {
	sounds[(int)ss].setLoop(true);
	PlaySound(ss);
}

void EndSoundLoop(Sound ss){
	sounds[(int)ss].stop();
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

bool Delay(float deltaTime, const float waitTime){
	static float timePassed = 0.f;
	timePassed += deltaTime;
	if (timePassed > waitTime) {
		
		timePassed = 0.f;
		return true;
	}
	return false;
}
