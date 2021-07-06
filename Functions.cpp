#include <SFML/Audio.hpp>

#include "Functions.h"

std::vector<sf::SoundBuffer> buffers;
std::vector<sf::Sound> sounds;

//sf::SoundBuffer b;
//sf::Sound s;
void LoadSounds() {
	//sf::SoundBuffer b;
	//sf::Sound s;
	//FIRE
	//b.loadFromFile("sounds/fire.wav");
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

	//THRUST
	//b.loadFromFile("sounds/thrust.wav");
	/*buffers.push_back(b);
	//BANG_S
	b.loadFromFile("sounds/bangSmall.wav");
	buffers.push_back(b);
	//BANG_M
	b.loadFromFile("sounds/bangMedium.wav");
	buffers.push_back(b);
	//BANG_L
	b.loadFromFile("sounds/bangLarge.wav");
	buffers.push_back(b);
	//SOUC_S
	b.loadFromFile("sounds/saucerSmall.wav");
	buffers.push_back(b);
	//SOUS_B
	b.loadFromFile("sounds/saucerBig.wav");
	buffers.push_back(b);
	//EXTRA
	b.loadFromFile("sounds/extraShip.wav");
	buffers.push_back(b);*/

	//for (auto& iter : buffers) {
		//s.setBuffer(b);
		//sounds.push_back(s);
	//}
}

void PlaySound(Sound ss) {
	sounds[(int)ss].play();
	///switch (s)
	//{
	//case Sound::FIRE:
	/*sf::SoundBuffer bT;
	bT.loadFromFile("sounds/fire.wav");
	sf::Sound sT;
	sT.setBuffer(bT);
	sT.play();*/
	/*	break;
	case Sound::THRUST:
		sounds[1].play();
		break;
	case Sound::BANG_S:
		sounds[2].play();
		break;
	case Sound::BANG_M:
		sounds[3].play();
		break;
	case Sound::BANG_L:
		sounds[4].play();
		break;
	case Sound::SOUC_S:
		sounds[5].play();
		break;
	case Sound::SOUS_B:
		sounds[6].play();
		break;
	case Sound::EXTRA:
		sounds[7].play();
		break;
	}*/
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
		vec.x = 0.1;
		passed = true;
	}
	if (vec.x <= 0) {
		vec.x = WIDTH-0.1;
		passed = true;
	}
	if (vec.y >= HEIGHT) {
		vec.y = 0.1;
		passed = true;
	}
	if (vec.y <= 0) {
		vec.y = HEIGHT-0.1;
		passed = true;
	}
	return passed;
}
