#include "Utils.h"

int get_window_height(SDL_Window* window)
{
	int w, h;
	SDL_GetWindowSize(window, &w, &h);
	return h;
}

int get_window_width(SDL_Window* window)
{
	int w, h;
	SDL_GetWindowSize(window, &w, &h);
	return w;
}