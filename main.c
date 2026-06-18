#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdbool.h>
#include "gapbuffer.h"
#include "eventHandling.h"
#include "textureCache.h"
#include "layout.h"
#include "cursor.h"
#include "editingFile.h"

void quit(SDL_Renderer* render, SDL_Window* window, editorBuffer Buffer )
{
	SDL_DestroyRenderer(render);
	SDL_DestroyWindow(window);

	SDL_StopTextInput(window);

	TTF_Quit();
	SDL_Quit();
	free(Buffer.buffer);
}

int render_text(SDL_Renderer* render, LayoutChar* layout, LineInfo* lineData, int valid_entries, int line_offset )
{
	SDL_FRect text;
	SDL_Texture* texture;
	int temp = 0;
	int vertical_offset = 0;
	if (line_offset > 0)
	{
		temp = lineData[line_offset].start_index;
		vertical_offset = 10 + (line_offset * 20);
	}
	for (int i = temp; i < valid_entries; i++)
	{
		char c = layout[i].c;
		Glyph* glyph = layout[i].glyph;
		if (glyph != NULL){
			texture = glyph->texture;
			text.w = glyph->width;
			text.h = glyph->height;
			text.x = layout[i].x;
			text.y = layout[i].y - vertical_offset;
			SDL_RenderTexture(render, texture, NULL, &text);
		}
		
	}
	return vertical_offset;
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

	Init_Texture(render);

	SDL_StartTextInput(window);
	SDL_Event event;

	editorBuffer Buffer;
	Buffer = Buffer_Init();


	LayoutChar* layedout = NULL;
	bool layout_dirty = false;
	int valid_entries = 0;

	if (readFile(&Buffer))
	{
		layout_dirty = true;
	}


	LineInfo* lineData = NULL;
	int cursor_line = 0;
	int preferred_col = 0;
	int total_lines = 0;
	int line_offset = 0;
	int vertical_offset = 0;


	bool running = false;

	while (!running) {	
		while (SDL_PollEvent(&event)) {
			event_Handle(&event, &Buffer, lineData, &running, &layout_dirty, &preferred_col, &total_lines);
		}	
		
		SDL_SetRenderDrawColor(render, 54, 56, 64, 225);
		SDL_RenderClear(render);

		if (layout_dirty)
		{
			layedout = Build_Layout(Buffer, render, window, &valid_entries, &lineData, &total_lines, &line_offset);
			layout_dirty = false;
		}
		if (layedout != NULL && lineData != NULL)
		{
			vertical_offset = render_text(render, layedout, lineData, valid_entries, line_offset);
		}
		draw_Cursor(render, layedout, valid_entries, Buffer.cursor, vertical_offset);
		SDL_RenderPresent(render);
	}

	Destroy_Cache();
	free(layedout);
	quit(render, window , Buffer);
}