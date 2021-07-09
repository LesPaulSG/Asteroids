#pragma once
#include "BulletManager.h"
#include "Explosion.h"

enum GameState { START, IN_GAME, GAME_OVER, NEW_TOP_SCORE, LEADERBORD };

class IoManager {
private:
	sf::RenderWindow* w;
	BulletManager& bm;
	bool& gameOver;
	std::chrono::duration<float> deltaTime;
	std::chrono::steady_clock::time_point clock;
	GameState currentState;
	sf::Text pressAnyKey;
	sf::Text score;
	sf::Text enterInitials;
	sf::Text initials;
	sf::Text leaderBoard;
	//std::vector<sf::Text> leaders;
	std::vector<Polygon> lives;
	std::vector<Explosion> VFX;
	std::vector<sf::RectangleShape> cursors;
	std::vector<std::pair<int, std::string>> leaders;
	bool playerDead;
	sf::Event evt;
	char activeSymbol;
public:
	IoManager(BulletManager& bm_, bool& gameOver_);
	void Update();
	void Start();
	void Game();
	void GameOver();
	void NewTopScore();
	void LeaderBoard();
	void LoadLeaderBoard();
	void CheckEvent();
	void KeyboardPressCheck();
	void KeyboardReleaseCheck();
};