#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL3/SDL.h>
#include <SDL3_TTF/SDL_ttf.h>
#include "gapbuffer.h"
#include "layout.h"
#include "textureCache.h"

LayoutChar* Build_Layout(editorBuffer Buffer, SDL_Renderer* render, SDL_Window* window,  int* valid_entries, CursorPos** idk)
{
	LayoutChar* layout = malloc(Buffer.size * sizeof(LayoutChar));

	if (layout == NULL)
	{
		return NULL;
	}

	CursorPos* cursor = malloc((Buffer.size + 1) * sizeof(CursorPos));

	if (cursor == NULL)
	{
		return NULL;
	}



	cursor[0].x = 10;
	cursor[0].y = 15;

	int buffer_index = 0;
	int cursor_index = 1;
	int layout_count = 0;

	int screen_w, screen_h;

	float char_x = 10;
	float char_y = 10;
	float line_height = 15;

	Glyph* glyph;
	SDL_GetWindowSize(window, &screen_w, &screen_h);
	while (buffer_index < Buffer.size)
	{
		if (buffer_index == Buffer.gap_start)
		{
			buffer_index = Buffer.gap_end + 1;
			continue;
		}

		char c = Buffer.buffer[buffer_index];

		if (c == '\0') break;

		if (c == '\n')
		{
			char_x = 10;
			char_y += line_height;

			cursor[cursor_index].x = char_x;
			cursor[cursor_index].y = char_y + 1;
			
			cursor_index++;
			buffer_index++;
			continue;
		}
		glyph = get_Glyph((unsigned char)c);
		if (glyph != NULL) {
			if (char_x + glyph->advance > screen_w) {

				char_x = 10;
				char_y += line_height;

			}

			layout[layout_count].c = c;
			layout[layout_count].x = char_x;
			layout[layout_count].y = char_y;
			layout[layout_count].glyph = glyph;

			char_x += glyph->advance;
		
			layout_count++;


			cursor[cursor_index].x = char_x;
			cursor[cursor_index].y = char_y + 1;
			cursor_index++;
		}	
		buffer_index++;
	}
	*idk = cursor;
	*valid_entries = layout_count;
	return layout;
}