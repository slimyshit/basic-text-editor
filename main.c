#include <stdio.h>
#include <SDL3/SDL.h>


int main()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}

	SDL_Window* window;

	window = SDL_CreateWindow(
		"nigga",
		640,
		480,
		SDL_WINDOW_RESIZABLE
	);

	if (window == NULL)
	{
		printf("Window could not be created %s\n", SDL_GetError());
		SDL_Quit();
	}

	bool done = false;
	SDL_Event event;
	SDL_Renderer* render;
	render = SDL_CreateRenderer(window, NULL);

	if (render == NULL)
	{
		printf("Render could not be created %s\n", SDL_GetError());
		SDL_DestroyWindow(window);
		SDL_Quit();
	}


	while (!done) {
		
		while (SDL_PollEvent(&event))
			if (event.type == SDL_EVENT_QUIT) {
				done = true;
			}
		SDL_SetRenderDrawColor(render, 54, 56, 64, 1.0);
		SDL_RenderClear(render);
		SDL_RenderPresent(render);
	}
	SDL_DestroyWindow(window);
	SDL_Quit();

}