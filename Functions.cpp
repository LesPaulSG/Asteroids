#include "Functions.h"

std::vector<sf::SoundBuffer> buffers;
std::vector<sf::Sound> sounds;

sf::Music oddity;

sf::Font font;

void LoadResources() {
	font.loadFromFile("res/vector.ttf");
	oddity.openFromFile("res/sounds/SpaceOddity320.wav");
	oddity.setLoop(true);
	oddity.play();

	buffers.reserve(soundFileNames.size());
	sounds.reserve(soundFileNames.size());

	for (auto& name : soundFileNames) {
		buffers.push_back(sf::SoundBuffer());
		buffers.back().loadFromFile("res/sounds/" + name + ".wav");
		sounds.push_back(sf::Sound(buffers.back()));
		sounds.back().setVolume(30.f);
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

const sf::Font& GetFont() {return font;}

bool isPointInRange(float x, float a, float b) {
	if (x == a) return true;
	if (x == b) return true;
	if ((x > std::min(a, b)) && (x < std::max(a, b))) return true;
	return false;
}

float VectorsModule(const sf::Vector2f& vec){
	return std::sqrt(vec.x * vec.x + vec.y * vec.y);
}


void RotateVector(sf::Vector2f& vec, float angle, const sf::Vector2f& anchor) {
	float xNew = (vec.x - anchor.x) * cos(angle) - (vec.y - anchor.y) * sin(angle) + anchor.x;
	float yNew = (vec.x - anchor.x) * sin(angle) + (vec.y - anchor.y) * cos(angle) + anchor.y;
	vec.x = xNew;
	vec.y = yNew;
}

void RotateUnitVector(sf::Vector2f& vec, float angle){
	vec.x = sin(angle);
	vec.y = -cos(angle);
}

bool isPassingVerBrd(const sf::Vector2f& vec){
	if (vec.x >= WIDTH)  return true;
	else if (vec.x <= 0) return true;
	return false;
}

bool isPassingHorBrd(const sf::Vector2f& vec){
	if (vec.y >= HEIGHT) return true;
	else if (vec.y <= 0) return true;
	return false;
}

void PassScreenBorder(sf::Vector2f& vec) {
	if (vec.x >= WIDTH)  vec.x -= WIDTH;
	else if (vec.x <= 0) vec.x += WIDTH;
	if (vec.y >= HEIGHT) vec.y -= HEIGHT;
	else if (vec.y <= 0) vec.y += HEIGHT;
}

void FormatText(sf::Text& txt) {
	txt.setOutlineColor(sf::Color::White);
	txt.setOutlineThickness(0.5);
	txt.setFillColor(sf::Color::White);
}

void TextToCenter(sf::Text& txt, float offsetX, float offsetY) {
	txt.setPosition(WIDTH  * offsetX - txt.getGlobalBounds().width  / 2,
					HEIGHT * offsetY - txt.getGlobalBounds().height / 2);
}
