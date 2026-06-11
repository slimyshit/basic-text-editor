#include <stdio.h>
#include <SDL3/SDL.h>


int main()
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window;

	window = SDL_CreateWindow(
		"nigga",
		640,
		480,
		SDL_WINDOW_RESIZABLE
		);
	bool done = false;
	SDL_Event event;
	while (!done) {
		
		while (SDL_PollEvent(&event))
			if (event.type == SDL_EVENT_QUIT) {
				done == true;
			}
	}
	SDL_DestroyWindow(window);
	SDL_Quit();

}