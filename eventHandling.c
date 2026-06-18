#include "gapbuffer.h"
#include "cursor.h"
#include "eventHandling.h"
#include <SDL3/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include "layout.h"

void event_Handle(SDL_Event* event, editorBuffer* Buffer,LineInfo* lineData, bool* done, bool* layout_dirty, int* preferredCol, int* total_lines) {
		if (event->type == SDL_EVENT_TEXT_INPUT) {
			Buffer_AddChar(event->text.text, Buffer);
			if (lineData != NULL)
			{
				int cursor_line = get_cursor_line_index(lineData, Buffer->cursor, total_lines);
				*preferredCol = Buffer->cursor - lineData[cursor_line].start_index;
			}
			*layout_dirty = true;
		}
		if (event->type == SDL_EVENT_QUIT) {
			*done = true;
		}
		if (event->type == SDL_EVENT_KEY_DOWN) {

			if (event->key.key == SDLK_BACKSPACE) {
				Buffer_DelChar(Buffer);
				if (lineData != NULL)
				{
					int cursor_line = get_cursor_line_index(lineData, Buffer->cursor, total_lines);
					*preferredCol = Buffer->cursor - lineData[cursor_line].start_index;
				}
				*layout_dirty = true;
			}
			else if (event->key.key == SDLK_RETURN || event->key.key == SDLK_KP_ENTER) {
				Buffer_NewLine(Buffer);
				if(lineData != NULL)
				{
					int cursor_line = get_cursor_line_index(lineData, Buffer->cursor, total_lines);
					*preferredCol = Buffer->cursor - lineData[cursor_line].start_index;
				}
				*layout_dirty = true;
			}
			else if(event->key.key == SDLK_RIGHT)
			{
				Buffer_navigate_cursor_x("right", Buffer);
				if (lineData != NULL)
				{
					int cursor_line = get_cursor_line_index(lineData, Buffer->cursor, total_lines);
					*preferredCol = Buffer->cursor - lineData[cursor_line].start_index;
				}
			}
			else if (event->key.key == SDLK_LEFT)
			{
				Buffer_navigate_cursor_x("left", Buffer);
				if (lineData != NULL)
				{
					int cursor_line = get_cursor_line_index(lineData, Buffer->cursor, total_lines);
					*preferredCol = Buffer->cursor - lineData[cursor_line].start_index;
					printf("cursor_line = %d    preferredCol = %d    cursor_index = %d\n", cursor_line, *preferredCol, Buffer->cursor);
				}
			
			}
			else if (event->key.key == SDLK_UP)
			{

				navigate_cursor_y("up", Buffer, lineData, preferredCol, total_lines);
			}
			else if (event->key.key == SDLK_DOWN)
			{
				navigate_cursor_y("down", Buffer, lineData, preferredCol, total_lines);
			}
		
		}
}	