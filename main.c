#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdbool.h>
#include "gapbuffer.h"
#include "eventHandling.h"
#include "textureCache.h"
#include "layout.h"


void quit(SDL_Renderer* render, SDL_Window* window, editorBuffer Buffer )
{
	SDL_DestroyRenderer(render);
	SDL_DestroyWindow(window);

	SDL_StopTextInput(window);

	TTF_Quit();
	SDL_Quit();
	free(Buffer.buffer);
}

void render_text(SDL_Renderer* render, LayoutChar* layout, int valid_entries )
{
	SDL_FRect text;
	SDL_Texture* texture;
	for (int i = 0; i < valid_entries; i++)
	{
		char c = layout[i].c;
		Glyph* glyph = layout[i].glyph;
		texture = glyph->texture;
		text.w = glyph->width;
		text.h = glyph->height;
		text.x = layout[i].x;
		text.y = layout[i].y;
		SDL_RenderTexture(render, texture, NULL, &text);
	}
}

void draw_cursor(SDL_Renderer* render, CursorPos* cus, int valid_entries, int cursor_index)
{
	SDL_FRect cursor;
	cursor.w = 2;
	cursor.h = 15;
	cursor.x = cus[cursor_index].x;
	cursor.y = cus[cursor_index].y;
	//printf("x = %f  ,  y = %f \n", cursor.x, cursor.y);
	SDL_SetRenderDrawColor(render, 255, 255, 255, 225);
	SDL_RenderFillRect(render, &cursor);
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


	SDL_StartTextInput(window);
	
	SDL_Event event;

	editorBuffer Buffer;
	Buffer = Buffer_Init();

	LayoutChar* layedout = NULL;
	bool layout_dirty = false;

	CursorPos* cursor = NULL;

	Init_Texture(render);

	int valid_entries = 0;

	bool running = false;
	while (!running) {	
		if (SDL_PollEvent(&event)) {
			event_Handle(&event, &Buffer, &running, &layout_dirty);
		}	
		
		SDL_SetRenderDrawColor(render, 54, 56, 64, 225);
		SDL_RenderClear(render);

		if (layout_dirty)
		{
			layedout = Build_Layout(Buffer, render, window, &valid_entries, &cursor);
			layout_dirty = false;
		}
		if (layedout != NULL && cursor != NULL)
		{
			render_text(render, layedout, valid_entries);
			draw_cursor(render, cursor, valid_entries, Buffer.cursor);
		}
		
		SDL_RenderPresent(render);
	}
	Destroy_Cache();
	free(layedout);
	free(cursor);
	quit(render, window , Buffer);
}