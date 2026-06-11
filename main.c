#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdbool.h>
#include "gapbuffer.h"


int main()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}
	if (TTF_Init() == false) {
		printf("TTf could not initialize! SDL_Error: %s\n", SDL_GetError());
		SDL_Quit();
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


	SDL_Renderer* render;
	render = SDL_CreateRenderer(window, NULL);


	
	if (render == NULL)
	{
		printf("Render could not be created %s\n", SDL_GetError());
		SDL_DestroyWindow(window);
		SDL_Quit();

	}



	TTF_Font* font;
	TTF_TextEngine* engine;
	font = TTF_OpenFont("C:\\Windows\\Fonts\\arial.ttf", 24.0);
	engine = TTF_CreateRendererTextEngine(render);



	bool done = false;
	SDL_Event event;
	SDL_Surface* surface;
	SDL_Color color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;
	SDL_Texture* texture;
	SDL_FRect frect;
	frect.x = 10.0f;
	frect.y = 10.0f;


	SDL_StartTextInput(window);

	editorBuffer Buffer;
	Buffer = Buffer_Init();

	while (!done) {
		while (SDL_PollEvent(&event))
			if (event.type == SDL_EVENT_TEXT_INPUT) {
				Buffer = Buffer_AddChar(event.text.text, Buffer);
			}
		SDL_SetRenderDrawColor(render, 54, 56, 64, 1.0);
		SDL_RenderClear(render);

		surface = TTF_RenderText_Blended(font, Buffer.buffer, 0, color);
		frect.w = (float)surface->w;
		frect.h = (float)surface->h;
		SDL_DestroySurface(surface);
		texture = SDL_CreateTextureFromSurface(render, surface);
		SDL_RenderTexture(render, texture, NULL, &frect);
		SDL_DestroyTexture(texture);
		SDL_RenderPresent(render);



	}


	TTF_DestroyRendererTextEngine(engine);
	TTF_CloseFont(font);

	SDL_DestroyRenderer(render);
	SDL_DestroyWindow(window);

	SDL_StopTextInput(window);

	TTF_Quit();
	SDL_Quit();
	free(Buffer.buffer);
}