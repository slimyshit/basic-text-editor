#include "gapbuffer.h"
#include "cursor.h"
#include "eventHandling.h"
#include <SDL3/SDL.h>
#include <stdbool.h>

void event_Handle(SDL_Event* event, editorBuffer* Buffer,LineInfo* lineData, bool* done, bool* layout_dirty, int* preferredCol, int cursor_index, int total_lines) {
	if (event->type == SDL_EVENT_TEXT_INPUT) {
		Buffer_AddChar(event->text.text, Buffer);
		*layout_dirty = true;
	}
	if (event->type == SDL_EVENT_QUIT) {
		*done = true;
	}
	if (event->type == SDL_EVENT_KEY_DOWN) {

		if (event->key.key == SDLK_BACKSPACE) {
			Buffer_DelChar(Buffer);
			*layout_dirty = true;
		}
		else if (event->key.key == SDLK_RETURN || event->key.key == SDLK_KP_ENTER) {
			Buffer_NewLine(Buffer);
			*layout_dirty = true;
		}
		else if(event->key.key == SDLK_RIGHT)
		{
			Buffer_navigate_cursor_x("right", Buffer);
			if (lineData != NULL)
			{
				int cursor_line = get_cursor_line_index(lineData, cursor_index, total_lines);	
				preferredCol = cursor_index - lineData[cursor_line].start_index;
				printf("cursor_line = %d    preferredCol = %d\n", cursor_line, preferredCol);
			}
		}
		else if (event->key.key == SDLK_LEFT)
		{
			Buffer_navigate_cursor_x("left", Buffer);
			if (lineData != NULL)
			{
				int cursor_line = get_cursor_line_index(lineData, cursor_index, total_lines);
				preferredCol = cursor_index - lineData[cursor_line].start_index;
				printf("preferredCol = %d\n", preferredCol);
			}
			
		}
		else if (event->key.key == SDLK_UP)
		{
			//Buffer_navigate_cursor_y("up", Buffer);
		}
		else if (event->key.key == SDLK_DOWN)
		{
			//Buffer_navigate_cursor_y("down", Buffer);
		}
		
	}
}	