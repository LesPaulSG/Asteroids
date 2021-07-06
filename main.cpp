#include <fstream>

#include "IO_Thread.h"

int main(){
	BulletManager bulletManager;
	
	std::chrono::duration<float> time{ 0 };
	bool gameOver = false;

	std::thread IOThread(input, std::ref(bulletManager), std::ref(time), std::ref(gameOver));		//input output thread
	IOThread.detach();

	LoadSounds();

	//std::this_thread::sleep_for(std::chrono::seconds(3));
	auto clock = std::chrono::high_resolution_clock::now();											//upper

	while (!gameOver){
		bulletManager.Update(time.count());

		time  = std::chrono::high_resolution_clock::now() - clock;
		clock = std::chrono::high_resolution_clock::now();
	}
	std::cout << 'jui\n';

	return 0;
}