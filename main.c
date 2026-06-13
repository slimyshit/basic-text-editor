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

void create_texture(SDL_Renderer* render, TTF_Font* font, const char* text, SDL_FRect* frect, SDL_Color color)
{
	SDL_Surface* surface = TTF_RenderText_Blended(font, text, 0, color);
	frect->w = (float)surface->w;
	frect->h = (float)surface->h;
	SDL_Texture* texture = SDL_CreateTextureFromSurface(render, surface);
	SDL_DestroySurface(surface);
	SDL_RenderTexture(render, texture, NULL, frect);
	SDL_DestroyTexture(texture);
}

void draw_Cursor(SDL_FRect* cursor, SDL_FRect* frect,SDL_Renderer* render, int lines, int line_height)
{
	cursor->w = 5;
	cursor->h = 20;
	cursor->x = frect->x;
	cursor->y = frect->y;
	SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
	SDL_RenderFillRect(render, cursor);
}

void out_of_bound(SDL_Window* window, SDL_FRect* frect, int glyph_w, int* lines, int line_height)
{
	int width, height;
	SDL_GetWindowSize(window, &width, &height);
	if ((frect->x+glyph_w) >= width)
	{
		(*lines)++;
		frect->x = 10;
		frect->y = (frect->y + line_height) + 5;
	}
}

void get_character_spacing(TTF_Font *font, SDL_FRect *frect, char *temp, int i, int* character_w, int* character_h)
{
	TTF_GetStringSize(font, temp, i, character_w, character_h);
	int	glyph_width = *character_w;
	frect->x = frect->x + glyph_width;
}

void new_line(editorBuffer* Buffer, SDL_FRect* frect, int* lines, int line_height, int* index)
{
	if (Buffer->buffer[*index] == '\n')
	{
		(*lines)++;
		frect->x = 10;
		frect->y = (frect->y + line_height) + 5;
		if (++*index == Buffer->gap_start)
		{
			if (Buffer->gap_end == Buffer->size)
			{
				*index = Buffer->size;
			}
			else
			{
				*index = Buffer->gap_end + 1;
			}
		}
	}
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
	SDL_StartTextInput(window);
	

	SDL_Color color = set_color();
	SDL_FRect frect, cursor;
	SDL_Event event;
	editorBuffer Buffer;

	Buffer = Buffer_Init();

	bool done = false;
	int  line_height = 10;
	char temp[2];
	int character_w, character_h;



	while (!done) {	
		while (SDL_PollEvent(&event)) {
			event_Handle(&event, &Buffer, &done);
		}	
		int index = 0;
		frect.x = 10;
		frect.y = 10;;
		int lines = 0;

		SDL_SetRenderDrawColor(render, 54, 56, 64, 255);
		SDL_RenderClear(render);

		while(index < Buffer.size ) {
			if (index == Buffer.gap_start)
			{
				if (Buffer.gap_end == Buffer.size) {
					break;
				}
				index = Buffer.gap_end + 1;
			}

			
			new_line(&Buffer, &frect, &lines, line_height, &index);

			temp[0] = Buffer.buffer[index];
			temp[1] = '\0';	

			create_texture(render, font, temp, &frect, color);
			get_character_spacing(font, &frect, temp, 1, &character_w, &character_h);
			out_of_bound(window, &frect, character_w, &lines, line_height);

			index++;
		}
		draw_Cursor(&cursor, &frect, render, lines, line_height);
		SDL_RenderPresent(render);
	}

	quit(engine, font, render, window , Buffer);
	
}