#include "IO_Thread.h"
#include "Saucer.h"

IoManager::IoManager(BulletManager& bm_, bool& gameOver_) :
		score("0", GetFont()),
		activeText(S_PRESS_ANY_KEY, GetFont()),
		initials("a__", GetFont(), 60),
		
		extraLive(sf::Vector2f(30.f, 70.f), STARSHIP_PATTERN),

		deltaTime(0),
		clock(std::chrono::high_resolution_clock::now()),

		w(nullptr),
		bm(bm_),
		gameOver(gameOver_),
		
		currentState(GameState::START),
		playerDead(false)
{
	FormatText(score);
	score.setPosition(5.f, 5.f);

	FormatText(activeText);
	TextToCenter(activeText);

	FormatText(initials);
	initials.setLetterSpacing(5.f);
	TextToCenter(initials, 0.5f, 0.7f);

	leaders.reserve(LEADERBOARD_MAX_SIZE);
	VFX.reserve(ASTEROIDS_MAX_QUANTITY);
}

void IoManager::Update(){
	w = new sf::RenderWindow(sf::VideoMode(WIDTH, HEIGHT), "Asteroids", sf::Style::Fullscreen);
	w->setMouseCursorVisible(false);

	while (w->isOpen()) {
		while (w->pollEvent(evt)) {
			CheckEvent();
		}

		w->clear();
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
		w->display();

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
	bm.Draw(*w);
	if (anim > 0.5f) {
		w->draw(activeText);
		if (anim > 1.f) anim = 0.f;
	}
}

void IoManager::Game(){
	if (!bm.isPlayerHasLives()) {
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
}

void IoManager::GameOver() {
	static float passedTime = 0;
	passedTime += deltaTime.count();
	if (passedTime > 2.f) {
		passedTime = 0.f;
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
	bm.Draw(*w);
	w->draw(activeText);
}

void IoManager::NewTopScore(){
	w->draw(activeText);
	w->draw(initials);
}

void IoManager::LeaderBoard(){
	w->draw(score);
	w->draw(activeText);
}

void IoManager::ChangeOnStart(){
	currentState = START;
	VFX.clear();
	leaders.clear();
	activeText.setString(S_PRESS_ANY_KEY);
	TextToCenter(activeText);
}

void IoManager::ChangeOnGame(){
	currentState = IN_GAME;
	bm.StartGame();
}

void IoManager::ChangeOnOver(){
	currentState = GAME_OVER;
	activeText.setString(S_GAME_OVER);
	TextToCenter(activeText);
}

void IoManager::ChangeOnInitials(){
	currentState = NEW_TOP_SCORE;
	activeText.setString(S_NEW_HIGH_SCORE);
	TextToCenter(activeText);
	initials.setString("a__");
}

void IoManager::ChangeOnLeaderboard(){
	currentState = LEADERBORD;
	activeText.setString(S_HIGH_SCORE);
	int counter = 0;
	for (auto& iter : leaders) {
		++counter;
		activeText.setString(activeText.getString() + "\n"
							+ std::to_string(counter) + " "
							+ std::to_string(iter.first) + " "
							+ iter.second);
	}
	TextToCenter(activeText);
}

void IoManager::LoadLeaderBoard(){
	std::ifstream fin("res/leaders.txt");
	int size, score;
	std::string name;
	fin >> size;
	size = std::min(size, LEADERBOARD_MAX_SIZE);
	for (int i = 0; i < size; ++i) {
		fin >> name >> score;
		leaders.push_back(std::make_pair(score, name));
	} 
	fin.close();
}

void IoManager::SaveLeagerBoard(){
	std::ofstream fout("res/leaders.txt");
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
				bm.GetPlayer().SetRotateDir(RotateDir::RGH);
				break;
			case sf::Keyboard::A:
				bm.GetPlayer().SetRotateDir(RotateDir::LFT);
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
	static int activeSymPos = 0;
	switch (currentState) {
	case IN_GAME:
		if (bm.GetPlayer().CanMove()) {
			switch (evt.key.code) {
			case sf::Keyboard::J:
				bm.GetPlayer().Thrust(false);
				break;
			case sf::Keyboard::D:
				bm.GetPlayer().SetRotateDir(RotateDir::STP);
				break;
			case sf::Keyboard::A:
				bm.GetPlayer().SetRotateDir(RotateDir::STP);
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
			++activeSymPos;
			UpdateInitials(activeSymPos);
			break;
		case sf::Keyboard::D:
			UpdateInitials(activeSymPos, true);
			break;
		case sf::Keyboard::A:			
			UpdateInitials(activeSymPos, false);
			break;
		}
		if (activeSymPos > 2) {
			ChangeOnLeaderboard();
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

void IoManager::UpdateInitials(int pos, bool ink){
	static std::string l_initials("a__");
	static char activeSymbol = 65;
	static int activePos = 0;
	if (activePos != pos) {
		if (activePos < 2) l_initials.at(activePos + 1) = 'a';
		initials.setString(l_initials);
		activePos = pos;
		activeSymbol = 65;
		if (pos > 2) {
			UpdateLeaderbord(l_initials);
			l_initials = "a__";
			activePos = 0;
		}
		return;
	}
		 if ( ink && (int)activeSymbol < 90) ++activeSymbol;
	else if (!ink && (int)activeSymbol > 65) --activeSymbol;
	l_initials.at(pos) = activeSymbol;
	initials.setString(l_initials);
}
