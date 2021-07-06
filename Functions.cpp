#include "Functions.h"

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

void PassScreenBorder(sf::Vector2f& vec) {
	if (vec.x >= 1920)	vec.x = 0.1;
	if (vec.x <= 0)		vec.x = 1919.9;
	if (vec.y >= 1080)	vec.y = 0.1;
	if (vec.y <= 0)		vec.y = 1079.9;
}
