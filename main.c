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
	if (text == NULL || text[0] == '\0') {
		frect->w = 0;
		frect->h = 0;
		return;
	}
	SDL_Surface* surface = TTF_RenderText_Blended_Wrapped(font, text, 0, color, 0);
	frect->w = (float)surface->w;
	frect->h = (float)surface->h;
	SDL_Texture* texture = SDL_CreateTextureFromSurface(render, surface);
	SDL_DestroySurface(surface);
	SDL_RenderTexture(render, texture, NULL, frect);
	SDL_DestroyTexture(texture);
}

void draw_Cursor(SDL_FRect* cursor, SDL_FRect* frect,SDL_Renderer* render)
{
	cursor->w = 2;
	cursor->h = 20;
	cursor->x = frect->x;
	cursor->y = frect->y;
	SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
	SDL_RenderFillRect(render, cursor);
}

void new_line(SDL_FRect* frect, float* lines, float line_h)
{
	(*lines)++;
	frect->x = 10;
	frect->y = 10.0 + (*lines * line_h);
}

char* Buffer_To_String(editorBuffer* Buffer)
{
	int text_length = Buffer->size - (Buffer->gap_end - Buffer->gap_start + 1) ;
	char* text = malloc(text_length + 1);
	if (text == NULL) {

		return "";
	}
	memmove(text, Buffer->buffer, Buffer->gap_start);
	memmove(text + Buffer->gap_start, Buffer->buffer + Buffer->gap_end + 1, Buffer->size - Buffer->gap_end + 1);
	text[text_length] = '\0';
	return text;
}

char* Build_Wrapped_Line(SDL_Renderer* render, SDL_Window* window, SDL_FRect* frect, TTF_Font* font, char* text, int* index, float* lines, float line_h)
{
	static char wrappedText[1024];
	int counter = 0;
	int character_w, character_h;
	int len = (int)strlen(text);
	int width, height;
	char temp[2];
	SDL_FRect cursor;
	frect->x = 10;
	frect->y = 10;
	bool window_end_reached;
	while (*index < len)
	{
		if(text[*index] != '\n') {
			temp[0] = text[*index];
			temp[1] = '\0';
			TTF_GetStringSize(font, temp, 1, &character_w, &character_h);
			SDL_GetWindowSize(window, &width, &height);
			if	((frect->x + character_w) < width) {
				wrappedText[counter] = temp[0];
				frect->x += character_w;
				counter++;
				(*index)++;
			}
			else {
				new_line(frect, lines, line_h);
				wrappedText[counter] = '\n';
				counter++;
				wrappedText[counter] = text[*index];
				frect->x += character_w;
				counter++;
				(*index)++;
			}
		}
		else {
			new_line(frect, lines, line_h);
			wrappedText[counter] = '\n';
			counter++;
			(*index)++;
		}
	}
	draw_Cursor(&cursor, frect, render);
	frect->x = 10;
	frect->y = 10;
	wrappedText[counter] = '\0';
	return wrappedText;
	
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
	
	SDL_FRect frect;
	SDL_Event event;
	editorBuffer Buffer;
	SDL_Color color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;;

	Buffer = Buffer_Init();

	bool done = false;
	bool move_cursor = false;
	float line_height = 15;

	while (!done) {	
 
		while (SDL_PollEvent(&event)) {
			event_Handle(&event, &Buffer, &done, &move_cursor);
		}	
		int index = 0;
		float lines = 0;

		SDL_SetRenderDrawColor(render, 54, 56, 64, 10);
		SDL_RenderClear(render);

		char* text = Buffer_To_String(&Buffer);

		char* string = Build_Wrapped_Line(render, window, &frect, font, text, &index, &lines, line_height);	
		
		create_texture(render, font, string, &frect, color);
		free(text);
		
		SDL_RenderPresent(render);
	}

	quit(engine, font, render, window , Buffer);
}