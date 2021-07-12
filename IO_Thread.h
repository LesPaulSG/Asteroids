#pragma once
#include "BulletManager.h"
#include "Explosion.h"

enum GameState { START, IN_GAME, GAME_OVER, NEW_TOP_SCORE, LEADERBORD };

class IoManager {
private:
	sf::Text score;
	sf::Text activeText;
	sf::Text initials;

	Polygon extraLive;
	std::vector<Explosion> VFX;
	std::vector<std::pair<int, std::string>> leaders;
	
	std::chrono::duration<float> deltaTime;
	std::chrono::steady_clock::time_point clock;
	
	sf::Event evt;

	sf::RenderWindow* w;
	BulletManager& bm;
	bool& gameOver;
	
	GameState currentState;
	bool playerDead;
	
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
	void SaveLeagerBoard();

	void CheckEvent();
	void KeyboardPressCheck();
	void KeyboardReleaseCheck();
	void UpdateLeaderbord(const std::string& newInitials);
	void UpdateInitials(int pos, bool ink = false);
};