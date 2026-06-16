#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdbool.h>
#include "gapbuffer.h"
#include "eventHandling.h"
#include "textureCache.h"


void quit(SDL_Renderer* render, SDL_Window* window, editorBuffer Buffer )
{
	SDL_DestroyRenderer(render);
	SDL_DestroyWindow(window);

	SDL_StopTextInput(window);

	TTF_Quit();
	SDL_Quit();
	free(Buffer.buffer);
}

void draw_Cursor(SDL_FRect* cursor,SDL_Renderer* render)
{
	SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
	SDL_RenderFillRect(render, cursor);
}

void new_line(SDL_FRect* frect, float* lines, float line_h)
{
	(*lines)++;
	frect->x = 10;
	frect->y = 10.0 + (*lines * line_h);
}

char* Buffer_To_String(editorBuffer* Buffer, int* cursor_index)
{
	int text_length = Buffer->size - (Buffer->gap_end - Buffer->gap_start + 1) ;
	char* text = malloc(text_length + 1);
	if (text == NULL) {

		return NULL;
	}
	memmove(text, Buffer->buffer, Buffer->gap_start);
	memmove(text + Buffer->gap_start, Buffer->buffer + Buffer->gap_end + 1 , Buffer->size - Buffer->gap_end - 1);
	text[text_length] = '\0';
	*cursor_index = Buffer->cursor;
	return text;
}

char* Build_Wrapped_Line(SDL_Window* window, char* text, int* cursor_index)
{
	int counter = 0;
	int index = 0;
	int len = (int)strlen(text);
	int width, height;
	char temp[2];
	Glyph* glyph;
	SDL_FRect frect;
	frect.x = 10;

	if (len == 0) return calloc(1, sizeof(char));

	char* wrappedText = malloc((len * 2) + 1);

	if (wrappedText == NULL)
	{
		return NULL;
	}

	while (index < len)
	{
		if(text[index] != '\n') {
			temp[0] = text[index];
			temp[1] = '\0';
			glyph = get_Glyph(text[index]);
			SDL_GetWindowSize(window, &width, &height);
			if	((frect.x + glyph->width) < width) {
				wrappedText[counter] = temp[0];
				frect.x += glyph->advance;
				counter++;
				index++;
			}
			else {
				frect.x = 10;
				wrappedText[counter] = '\n';
				counter++;
				(*cursor_index)++;
				continue;
			}
		}
		else {
			frect.x = 10;
			wrappedText[counter] = '\n';
			(*cursor_index)++;
			counter++;
			index++;
			continue;
		}
	}
	wrappedText[counter] = '\0';
	return wrappedText;

}

void render_Text(const char* wrappedText, SDL_FRect* frect, SDL_Renderer* render)
{
	Glyph* glyph;
	int index = 0;
	int len = strlen(wrappedText) ;
	float lines = 0;
	float line_h = 15.0;

	while (index < len)
	{
		char c = wrappedText[index];
		if (c != '\n')
		{
			glyph = get_Glyph(c);
			frect->w = glyph->width;
			frect->h = glyph->height;
			SDL_RenderTexture(render, glyph->texture, NULL,frect);
			frect->x += glyph->advance;
		}
		else {
			new_line(frect, &lines, line_h);
		}
		
		index++;
	}
}

void Get_New_Cursor_Coord(SDL_FRect* cursor, int* cursor_index, char* text)
{
	cursor->w = 3;
	cursor->h = 20;
	cursor->x = 10;
	cursor->y = 10;
	Glyph* glyph;
	int line_count = 0;
	int index = 0;
	int len = strlen(text);
	while(index < *cursor_index && index < len)
	{
		char c = text[index];
		if (c != '\n') {
			glyph = get_Glyph(c);
			if (glyph != NULL) { 
				cursor->x += glyph->advance;
			}
		}
		else if(c == '\n')
		{
			line_count++;
			cursor->y = 10 + (line_count * 15);
			cursor->x = 10;
			index++;
			continue;
		}
		index++;
	}
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
	
	SDL_FRect frect, cursor;
	SDL_Event event;
	editorBuffer Buffer;
	Buffer = Buffer_Init();

	Init_Texture(render);

	bool done = false;
	int cursor_index;


	while (!done) {	
		while (SDL_PollEvent(&event)) {
			event_Handle(&event, &Buffer, &done);
		}	

		frect.x = 10;
		frect.y = 10;

		SDL_SetRenderDrawColor(render, 54, 56, 64, 225);
		SDL_RenderClear(render);

		char* text = Buffer_To_String(&Buffer, &cursor_index);
		char* string = Build_Wrapped_Line(window, text, &cursor_index);	
		free(text);
		
		render_Text(string, &frect, render);
		Get_New_Cursor_Coord(&cursor, &cursor_index, string);
		free(string);
		draw_Cursor(&cursor, render);

		SDL_RenderPresent(render);
	}
	Destroy_Cache();
	quit(render, window , Buffer);
}