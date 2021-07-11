#include "IO_Thread.h"
#include "Saucer.h"

#include "SFML/Audio.hpp"
#include <fstream>

IoManager::IoManager(BulletManager& bm_, bool& gameOver_) :
		bm(bm_),
		gameOver(gameOver_),
		deltaTime(0),
		clock(std::chrono::high_resolution_clock::now()),
		currentState(GameState::START),
		score("0", GetFont()),
		activeText(S_PRESS_ANY_KEY, GetFont()),
		initials("a__", GetFont(), 60),
		extraLive(sf::Vector2f(30.f, 70.f), STARSHIP_PATTERN),
		playerDead(false)
{
	score.setOutlineColor(sf::Color::White);
	score.setOutlineThickness(0.5);
	score.setPosition(5.f, 5.f);
	score.setFillColor(sf::Color::White);

	activeText.setOutlineColor(sf::Color::White);
	activeText.setOutlineThickness(0.5);
	activeText.setPosition(WIDTH/2 - activeText.getGlobalBounds().width/2, HEIGHT / 2 - activeText.getGlobalBounds().height / 2);
	activeText.setFillColor(sf::Color::White);

	initials.setOutlineColor(sf::Color::White);
	initials.setOutlineThickness(0.5);
	initials.setPosition(WIDTH / 2 - activeText.getGlobalBounds().width / 2, HEIGHT * 0.7f - activeText.getGlobalBounds().height / 2);
	initials.setFillColor(sf::Color::White);
	initials.setLetterSpacing(5.f);

	leaders.reserve(10);
	VFX.reserve(45);

	clock = std::chrono::high_resolution_clock::now();
}

void IoManager::Update(){
	w = new sf::RenderWindow(sf::VideoMode(WIDTH, HEIGHT), "Asteroids", sf::Style::Fullscreen);
	w->setMouseCursorVisible(false);

	while (w->isOpen()) {
		while (w->pollEvent(evt)) {
			CheckEvent();
		}

		switch (currentState){
		case START:
			Start();
			break;
		case IN_GAME:
			Game();
			break;
		case GAME_OVER:
			GameOver();
			break;
		case NEW_TOP_SCORE:
			NewTopScore();
			break;
		case LEADERBORD:
			LeaderBoard();
			break;
		}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			gameOver = true;
			w->close();
		}

		deltaTime = std::chrono::high_resolution_clock::now() - clock;
		clock = std::chrono::high_resolution_clock::now();
	}
}

void IoManager::Start() {
	static float anim = 0.f;
	anim += deltaTime.count();
	w->clear();
	bm.Draw(*w);
	if (anim > 0.5f) {
		w->draw(activeText);
		if (anim > 1.f) anim = 0.f;
	}
	w->display();
}

void IoManager::Game(){
	if (!bm.GetPlayer().isAlive()) {
		ChangeOnOver();
		LoadLeaderBoard();
		return;
	}

	if (bm.isExplosions()) {
		VFX.push_back(bm.PopExplosion());
	}
	for (auto iter = VFX.begin(); iter != VFX.end(); ++iter) {
		iter->Update(deltaTime.count());
		if (!iter->isAlive()) {
			VFX.erase(iter);
			break;
		}
	}

	w->clear();

	for (auto& iter : VFX) {
		iter.Draw(*w);
	}
	for (int i = 0; i < bm.GetPlayerLives(); ++i) {
		extraLive.Move(sf::Vector2f(30.f * i + 30.f, 70.f));
		extraLive.Draw(*w);
	}

	bm.Draw(*w);

	score.setString(std::to_string(bm.GetScore()));

	w->draw(score);
	w->display();
}

void IoManager::GameOver() {
	static float passed = 0;
	passed += deltaTime.count();
	if (passed > 3) {
		passed = 0.f;
		if (leaders.size() < 10) {
			ChangeOnInitials();
			bm.Clear();
			return;
		}
		for (auto& iter : leaders) {
			if (bm.GetScore() > iter.first) {
				ChangeOnInitials();
				bm.Clear();
				return;
			}
		}
		bm.Clear();
		ChangeOnLeaderboard();
		return;
	}
	w->clear();
	bm.Draw(*w);
	w->draw(activeText);
	w->display();
}

void IoManager::NewTopScore(){
	w->clear();
	w->draw(score);
	w->draw(activeText);
	w->draw(initials);
	w->display();
}

void IoManager::LeaderBoard(){
	w->clear();
	w->draw(score);
	w->draw(activeText);
	w->display();
}

void IoManager::ChangeOnStart(){
	currentState = START;
	VFX.clear();
	leaders.clear();
	activeText.setString(S_PRESS_ANY_KEY);
	activeText.setPosition(WIDTH / 2 - activeText.getGlobalBounds().width / 2, HEIGHT / 2 - activeText.getGlobalBounds().height / 2);
}

void IoManager::ChangeOnGame(){
	currentState = IN_GAME;
	bm.StartGame();

}

void IoManager::ChangeOnOver(){
	currentState = GAME_OVER;
	activeText.setString(S_GAME_OVER);
	activeText.setPosition(WIDTH / 2 - activeText.getGlobalBounds().width / 2, HEIGHT / 2 - activeText.getGlobalBounds().height / 2);
}

void IoManager::ChangeOnInitials(){
	currentState = NEW_TOP_SCORE;
	activeText.setString(S_NEW_HIGH_SCORE);
	activeText.setPosition(WIDTH / 2 - activeText.getGlobalBounds().width / 2, HEIGHT / 2 - activeText.getGlobalBounds().height / 2);
	initials.setString("a__");
}

void IoManager::ChangeOnLeaderboard(){
	currentState = LEADERBORD;
	activeText.setString(S_HIGH_SCORE);
	int counter = 0;
	for (auto& iter : leaders) {
		++counter;
		activeText.setString(activeText.getString() + "\n" + std::to_string(counter) + " " + std::to_string(iter.first) + " " + iter.second);
	}
	activeText.setPosition(WIDTH / 2 - activeText.getGlobalBounds().width / 2, HEIGHT / 2 - activeText.getGlobalBounds().height / 2);
}

void IoManager::LoadLeaderBoard(){
	std::ifstream fin("res/leaders.txt");
	int size, score;
	std::string name;
	fin >> size;
	for (int i = 0; i < size; ++i) {
		fin >> name >> score;
		leaders.push_back(std::make_pair(score, name));
	} 
	fin.close();
}

void IoManager::SaveLeagerBoard(){
	std::ofstream fout("leaders.txt");
	fout << leaders.size() << std::endl;
	for (auto& iter : leaders) {
		fout << iter.second << " " << iter.first << std::endl;
	}
	fout.close();
}

void IoManager::CheckEvent() {
	switch (evt.type) {
	case sf::Event::Closed:
		break;
	case sf::Event::KeyPressed:
		KeyboardPressCheck();
		break;
	case sf::Event::KeyReleased:
		KeyboardReleaseCheck();
		break;
	}
}

void IoManager::KeyboardPressCheck() {
	switch (currentState)	{
	case START:
		ChangeOnGame();
		return;
	case IN_GAME:
		if (bm.GetPlayer().CanMove()) {
			switch (evt.key.code) {
			case sf::Keyboard::J:
				bm.GetPlayer().Thrust(true);
				break;
			case sf::Keyboard::D:
				bm.GetPlayer().Rotate(RGH);
				break;
			case sf::Keyboard::A:
				bm.GetPlayer().Rotate(LFT);
				break;
			}
		}
		break;
	case LEADERBORD:
		ChangeOnStart();
		return;
	}	
}

void IoManager::KeyboardReleaseCheck() {
	static std::string l_initials("___");
	static char activeSymbol = 65;
	static int activeSymPos = 0;
	switch (currentState) {
	case IN_GAME:
		if (bm.GetPlayer().CanMove()) {
			switch (evt.key.code) {
			case sf::Keyboard::J:
				bm.GetPlayer().Thrust(false);
				break;
			case sf::Keyboard::D:
				bm.GetPlayer().Rotate(STP);
				break;
			case sf::Keyboard::A:
				bm.GetPlayer().Rotate(STP);
				break;
			case sf::Keyboard::G:
				bm.GetPlayer().HyperJump();
				break;
			case sf::Keyboard::L:
				bm.Shoot();
				break;
			}
		}
		break;
	case NEW_TOP_SCORE:
		switch (evt.key.code) {
		case sf::Keyboard::G:
			l_initials.at(activeSymPos) = activeSymbol;
			l_initials.at(activeSymPos+1) = 'a';
			++activeSymPos;
			initials.setString(l_initials);
			activeSymbol = 65;
			break;
		case sf::Keyboard::D:
			if ((int)activeSymbol < 90) {
				++activeSymbol;
				l_initials.at(activeSymPos) = activeSymbol;
				initials.setString(l_initials);
			}
			break;
		case sf::Keyboard::A:			
			if ((int)activeSymbol > 65) {
				--activeSymbol;
				l_initials.at(activeSymPos) = activeSymbol;
				initials.setString(l_initials);
			}
			break;
		}
		if (activeSymPos > 2) {
			ChangeOnLeaderboard();
			UpdateLeaderbord(l_initials);
			activeSymPos = 0;
			return;
		}
		break;
	}
}

void IoManager::UpdateLeaderbord(const std::string& newInitials){
	leaders.push_back(std::make_pair(bm.GetScore(), newInitials));
	std::sort(leaders.begin(), leaders.end(), []
											(std::pair<int, std::string> f,
												std::pair<int, std::string> s)
											{return f.first > s.first; });
	if (leaders.size() > 10) leaders.pop_back();
	SaveLeagerBoard();
}
