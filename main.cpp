#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <SDL2/SDL.h> 


using namespace std;

void renderPlayer(SDL_Renderer* renderer, SDL_Rect player, int x, int y, int scale, vector<int> tailX, vector<int> tailY, int tailLength) {

SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White

	player.w = scale;
	player.h = scale;

	for (int i = 0; i < tailLength; i++) {
		
		player.x = tailX[i];
		player.y = tailY[i];
		SDL_RenderFillRect(renderer, &player);
	}



SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // RED
	player.x = x;
	player.y = y;

	SDL_RenderFillRect(renderer, &player);
}

void renderFood(SDL_Renderer* renderer, SDL_Rect food) {
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	SDL_RenderFillRect(renderer, &food);
}


bool checkCollision(int foodx, int foody, int playerx, int playery) {

	if (playerx == foodx && playery == foody){
		return true;
	}

	return false;
}

pair<int, int> getFoodSpawn(vector<int> tailX, vector<int> tailY, int playerX, int playerY, int scale, int wScale, int tailLength) {
	bool valid = false;
	int x = 0;
	int y = 0;
	srand(time(0));
	x = scale * (rand() % wScale);
	y = scale * (rand() % wScale);
	valid = true;

	for (int i = 0; i < tailLength; i++) {

		if ((x == tailX[i] && y == tailY[i]) || (x == playerX && y == playerY)) {
			valid = false;
		}

	}

	if (!valid) {
		pair<int, int> foodLoc;
		foodLoc = make_pair(-100, -100);
		return foodLoc;
	}

	pair<int, int> foodLoc;
	foodLoc = make_pair(x, y);

	return foodLoc;
}


// void youWin(SDL_Renderer* renderer, SDL_Event event, int scale, int wScale, int tailLength) {
// 	SDL_Color Red = { 255, 0, 0 };
// 	SDL_Color White = { 255, 255, 255 };
// 	SDL_Color Black = { 0, 0, 0 };
// 	SDL_Color Yellow = { 255, 255, 0 };

// 	SDL_Rect gameoverRect;
// 	SDL_Rect retryRect;
// 	SDL_Rect scoreRect;
// 	gameoverRect.w = 200;
// 	gameoverRect.h = 100;
// 	gameoverRect.x = ((scale*wScale) / 2) - (gameoverRect.w / 2);
// 	gameoverRect.y = ((scale*wScale) / 2) - (gameoverRect.h / 2) - 50;
// 	retryRect.w = 300;
// 	retryRect.h = 50;
// 	retryRect.x = ((scale*wScale) / 2) - ((retryRect.w / 2));
// 	retryRect.y = (((scale*wScale) / 2) - ((retryRect.h / 2)) + 150);
// 	scoreRect.w = 100;
// 	scoreRect.h = 30;
// 	scoreRect.x = ((scale*wScale) / 2) - (scoreRect.w / 2);
// 	scoreRect.y = 0;



// 	while (true) {
// 		SDL_RenderPresent(renderer);

// 		if (SDL_PollEvent(&event)) {

// 			if (event.type == SDL_QUIT) {
// 				exit(0);
// 			}

// 			if (event.key.keysym.scancode == SDL_SCANCODE_RETURN) {
// 				return;
// 			}

// 		}

// 	}

// }

int main(int argc, char* argv[]) {

	SDL_Init(SDL_INIT_EVERYTHING);
	bool running = true;
	bool pause = false;


	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Event event;

	// This is the player rectangle, set all values to 0
	SDL_Rect player;
	player.x = 0;
	player.y = 0;
	player.h = 0;
	player.w = 0;

	// tailLength is incremented every time the snake eats food
	int tailLength = 0;

	// Vectors for storage of tail block positions
	vector<int> tailX;
	vector<int> tailY;

	// Size of tiles
	int scale = 24;
	int wScale = 24;

	// Player position variables
	int x = 0;
	int y = 0;
	int prevX = 0;
	int prevY = 0;

	// Movement controls
	bool up = false;
	bool down = false;
	bool right = false;
	bool left = false;

	bool inputThisFrame = false;
	bool redo = false;

	// Food rectangle
	SDL_Rect food;
	food.w = scale;
	food.h = scale;
	food.x = 0;
	food.y = 0;
	
	pair<int, int> foodLoc = getFoodSpawn(tailX, tailY, x, y, scale, wScale, tailLength);
	food.x = foodLoc.first;
	food.y = foodLoc.second;

	window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, scale*wScale+1, scale*wScale+1, SDL_WINDOW_RESIZABLE);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	float time = SDL_GetTicks() / 100;

	while (running) { 

		float newTime = SDL_GetTicks() / 75; 
		float delta = newTime - time;
		time = newTime;

		inputThisFrame = false;

		// if (tailLength >= 575) {
		// 	youWin(renderer, event, scale, wScale, tailLength);
		// 	x = 0;
		// 	y = 0;
		// 	up = false;
		// 	left = false;
		// 	right = false;
		// 	down = false;
		// 	tailX.clear();
		// 	tailY.clear();
		// 	tailLength = 0;
		// 	redo = false;
		// 	foodLoc = getFoodSpawn(tailX, tailY, x, y, scale, wScale, tailLength);

		// 	if (food.x == -100 && food.y == -100) {
		// 		redo = true;
		// 	}

		// 	food.x = foodLoc.first;
		// 	food.y = foodLoc.second;
		// }


		if (SDL_PollEvent(&event)) {

			if (event.type == SDL_QUIT) {
				exit(0);
			}

			if (event.type == SDL_KEYDOWN && inputThisFrame == false) {

				if (down == false && ((event.key.keysym.scancode == SDL_SCANCODE_UP) || event.key.keysym.scancode ==SDL_SCANCODE_W)) {
					up = true;
					left = false;
					right = false;
					down = false;
					inputThisFrame = true;
				}
				else if (right == false && ((event.key.keysym.scancode == SDL_SCANCODE_LEFT) || event.key.keysym.scancode ==SDL_SCANCODE_A)) {
					up = false;
					left = true;
					right = false;
					down = false;
					inputThisFrame = true;
				}
				else if (up == false && ((event.key.keysym.scancode == SDL_SCANCODE_DOWN) || event.key.keysym.scancode ==SDL_SCANCODE_S)) {
					up = false;
					left = false;
					right = false;
					down = true;
					inputThisFrame = true;
				}
				else if (left == false && ((event.key.keysym.scancode == SDL_SCANCODE_RIGHT) || event.key.keysym.scancode ==SDL_SCANCODE_D)) {
					up = false;
					left = false;
					right = true;
					down = false;
					inputThisFrame = true;
				}
				else if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE || event.key.keysym.scancode ==SDL_SCANCODE_Q) {
					running = false; // завершаем работу программы
				}else if ( event.key.keysym.scancode == SDL_SCANCODE_E) {
					pause = !pause; 
				}

			}

		}

		if(pause == false){

		prevX = x;
		prevY = y;

		if (up) {
			y -= delta * scale;
		}
		else if (left) {
			x -= delta * scale;
		}
		else if (right) {
			x += delta * scale;
		}
		else if (down) {
			y += delta * scale;
		}

		if (redo == true) {

			redo = false;
			foodLoc = getFoodSpawn(tailX, tailY, x, y, scale, wScale, tailLength);
			food.x = foodLoc.first;
			food.y = foodLoc.second;

			if (food.x == -100 && food.y == -100) {
				redo = true;
			}

		}


		if (checkCollision(food.x, food.y, x, y)) {
			foodLoc = getFoodSpawn(tailX, tailY, x, y, scale, wScale, tailLength);
			food.x = foodLoc.first;
			food.y = foodLoc.second;

			if (food.x == -100 && food.y == -100) {
				redo = true;
			}

			tailLength++;
		}


		if (delta * scale == 24) {

			if (tailX.size() != tailLength) {
				tailX.push_back(prevX);
				tailY.push_back(prevY);
			}

			for (int i = 0; i < tailLength; i++) {

				if (i > 0) {
					tailX[i - 1] = tailX[i];
					tailY[i - 1] = tailY[i];
				}

			}

			if (tailLength > 0) {
				tailX[tailLength - 1] = prevX;
				tailY[tailLength - 1] = prevY;
			}

		}
		
		for (int i = 0; i < tailLength; i++) {

			if (x == tailX[i] && y == tailY[i]) {
				x = 0;
				y = 0;
				up = false;
				left = false;
				right = false;
				down = false;
				tailX.clear();
				tailY.clear();
				tailLength = 0;
				redo = false;

				foodLoc = getFoodSpawn(tailX, tailY, x, y, scale, wScale, tailLength);
				if (food.x == -100 && food.y == -100) {
					redo = true;
				}

				food.x = foodLoc.first;
				food.y = foodLoc.second;
			}

		}

		if (x < 0 || y < 0 || x > scale * wScale - scale || y > scale * wScale - scale) {
			x = 0;
			y = 0;
			up = false;
			left = false;
			right = false;
			down = false;
			tailX.clear();
			tailY.clear();
			tailLength = 0;
			redo = false;
			foodLoc = getFoodSpawn(tailX, tailY, x, y, scale, wScale, tailLength);
			food.x = foodLoc.first;
			food.y = foodLoc.second;

			if (food.x == -100 && food.y == -100) {
				redo = true;
			}

		}
		}


		renderFood(renderer, food);
		renderPlayer(renderer, player, x, y, scale, tailX, tailY, tailLength);

		SDL_RenderPresent(renderer);

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);
	}

	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;

}