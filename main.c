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
		if (glyph != NULL){
			texture = glyph->texture;
			text.w = glyph->width;
			text.h = glyph->height;
			text.x = layout[i].x;
			text.y = layout[i].y;
			SDL_RenderTexture(render, texture, NULL, &text);
		}
		
	}
}


void draw_Cursor(SDL_Renderer* render, LayoutChar* layout, int valid_entries, int layout_index)
{
	SDL_FRect cursor;
	cursor.w = 3;
	cursor.h = 20;
	printf("cursor=%d valid=%d\n", layout_index, valid_entries);
	if (layout == NULL || layout_index == 0) {
		cursor.x = 10;
		cursor.y = 10;
	}
	
	else if (layout[layout_index - 1].c == '\n') {
		cursor.x = layout[layout_index - 1].x;
		cursor.y = layout[layout_index - 1].y;
	}
	else if (layout_index < valid_entries) {
		Glyph* glyph = layout[layout_index - 1].glyph;
		if (glyph != NULL) {
			int x = glyph->advance;
			cursor.x = layout[layout_index - 1].x + x;
			cursor.y = layout[layout_index - 1].y;
		}

	}
	else {
		Glyph* glyph = layout[layout_index - 1].glyph;
		if (glyph != NULL) {
			int x = glyph->advance;
			cursor.x = layout[layout_index - 1].x + x;
			cursor.y = layout[layout_index - 1].y;
		}
	}
	SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
	SDL_RenderFillRect(render, &cursor);
}


int main()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		SDL_Quit();
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

	LineInfo* lineData = NULL;
	int cursor_line;

	Init_Texture(render);

	int valid_entries = 0;

	bool running = false;
	while (!running) {	
		while (SDL_PollEvent(&event)) {
			event_Handle(&event, &Buffer, &running, &layout_dirty);
		}	
		
		SDL_SetRenderDrawColor(render, 54, 56, 64, 225);
		SDL_RenderClear(render);

		if (layout_dirty)
		{
			layedout = Build_Layout(Buffer, render, window, &valid_entries, &lineData);
			layout_dirty = false;
		}
		if (layedout != NULL )
		{
			render_text(render, layedout, valid_entries);
		}
		draw_Cursor(render, layedout, valid_entries, Buffer.cursor);
		SDL_RenderPresent(render);
	}
	Destroy_Cache();
	free(layedout);
	quit(render, window , Buffer);
}