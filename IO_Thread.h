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

	sf::Text score;
	sf::Text activeText;
	sf::Text initials;
	
	std::vector<Polygon> lives;
	std::vector<Explosion> VFX;
	std::vector<std::pair<int, std::string>> leaders;
	bool playerDead;
	sf::Event evt;
	
public:
	IoManager(BulletManager& bm_, bool& gameOver_);
	void Update();
	void Start();
	void Game();
	void GameOver();
	void NewTopScore();
	void LeaderBoard();
	void ChangeOnStart();
	void ChangeOnGame();
	void ChangeOnOver();
	void ChangeOnInitials();
	void ChangeOnLeaderboard();
	void LoadLeaderBoard();
	void CheckEvent();
	void KeyboardPressCheck();
	void KeyboardReleaseCheck();
	void UpdateLeaderbord(const std::string& newInitials);
};