#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdbool.h>
#include "gapbuffer.h"
#include "eventHandling.h"


void quit(TTF_TextEngine * engine, TTF_Font* font, SDL_Renderer* render, SDL_Window* window, editorBuffer Buffer )
{
	TTF_DestroyRendererTextEngine(engine);
	TTF_CloseFont(font);

	SDL_DestroyRenderer(render);
	SDL_DestroyWindow(window);

	SDL_StopTextInput(window);

	TTF_Quit();
	SDL_Quit();
	free(Buffer.buffer);
}

void render_text(SDL_Renderer* render, TTF_Font* font, const char* text, SDL_FRect* frect, SDL_Color color)
{
	SDL_Surface* surface = TTF_RenderText_Blended(font, text, 0, color);
	frect->w = (float)surface->w;
	frect->h = (float)surface->h;
	SDL_Texture* texture = SDL_CreateTextureFromSurface(render, surface);
	SDL_DestroySurface(surface);
	SDL_RenderTexture(render, texture, NULL, frect);
	SDL_DestroyTexture(texture);
}

SDL_Color set_color()
{
	SDL_Color color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;
	return color;
}




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

	if (window == NULL){
		printf("Window could not be created %s\n", SDL_GetError());
		SDL_Quit();
	}



	SDL_Renderer* render;
	render = SDL_CreateRenderer(window, NULL);

	if (render == NULL){
		printf("Render could not be created %s\n", SDL_GetError());
		SDL_DestroyWindow(window);
		SDL_Quit();
	}



	TTF_Font* font;
	font = TTF_OpenFont("C:\\Windows\\Fonts\\arial.ttf", 16.0);
	TTF_TextEngine* engine;
	engine = TTF_CreateRendererTextEngine(render);



	SDL_Color color = set_color();
	


	SDL_FRect frect;
	SDL_StartTextInput(window);
	SDL_Event event;
	editorBuffer Buffer;
	Buffer = Buffer_Init();
	bool done = false;
	int  line_height = 10;
	char temp[2];
	int w, h, glyph_width;

	while (!done) {	
		while (SDL_PollEvent(&event)) {
			event_Handle(&event, &Buffer, &done);
		}	
		int index = 0;
		frect.x = 10;
		frect.y = 10;

		SDL_SetRenderDrawColor(render, 54, 56, 64, 255);
		SDL_RenderClear(render);
		
		while(index < Buffer.size) {

			if (index == Buffer.gap_start)
			{
				index = Buffer.gap_end;
			}

			if (Buffer.buffer[index] == '\n')
			{
				frect.x = 10;
				frect.y = frect.y + line_height;
			}
			temp[0] = Buffer.buffer[index];
			temp[1] = '\0';	
			TTF_GetStringSize(font, temp, 1, &w, &h);
			glyph_width = w;
			render_text(render, font, temp, &frect, color);
			frect.x = frect.x + glyph_width;
			index++;
		}
		SDL_RenderPresent(render);
	}

	quit(engine, font, render, window , Buffer);
	
}