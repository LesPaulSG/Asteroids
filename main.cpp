#include "IO_Thread.h"
#include <Windows.h>

int main(){
	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_SHOW);
	LoadResources();

	BulletManager bulletManager;
	
	std::chrono::duration<float> time{ 0 };
	bool gameOver = false;

	IoManager ioManager(bulletManager, gameOver);

	std::thread IoThread(&IoManager::Update, ioManager);		//input output thread
	IoThread.detach();

	//std::this_thread::sleep_for(std::chrono::seconds(3));
	auto clock = std::chrono::high_resolution_clock::now();
	while (!gameOver){
		bulletManager.Update(time.count());

		time  = std::chrono::high_resolution_clock::now() - clock;
		clock = std::chrono::high_resolution_clock::now();
	}
	return 0;
}