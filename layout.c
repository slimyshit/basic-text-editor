#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL3/SDL.h>
#include <SDL3_TTF/SDL_ttf.h>
#include "gapbuffer.h"
#include "layout.h"
#include "textureCache.h"

LayoutChar* Build_Layout(editorBuffer Buffer, SDL_Renderer* render, SDL_Window* window,  int* valid_entries, LineInfo** lineData, int* totalLines)
{
	LayoutChar* layout = malloc(Buffer.size * sizeof(LayoutChar));
	if (layout == NULL)
	{
		return NULL;
	}

	LineInfo* lines = malloc((Buffer.size + 1)* sizeof(LineInfo));
	if (lines == NULL) return NULL;

	int buffer_index = 0;
	int layout_count = 0;

	int line_count = 0;
	int start_index = 0;

	int screen_w, screen_h;

	float char_x = 10;
	float char_y = 10;
	float line_height = 20;

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
			layout[layout_count].c = '\n';
			layout[layout_count].x = char_x;
			layout[layout_count].y = char_y;
			layout[layout_count].glyph = NULL;
			buffer_index++;

			lines[line_count].end_index = layout_count;
			lines[line_count].start_index = start_index;

			//printf("line = %d    start_index = %d    end_index = %d\n", line_count, lines[line_count].start_index, lines[line_count].end_index);

			layout_count++;
			line_count++;
			start_index = layout_count;
			continue;
		}

		glyph = get_Glyph((unsigned char)c);

		if (glyph != NULL) {
			if ((char_x + glyph->advance + 10) > screen_w) {

				char_x = 10;
				char_y += line_height;

			}

			layout[layout_count].c = c;
			layout[layout_count].x = char_x;
			layout[layout_count].y = char_y;
			layout[layout_count].glyph = glyph;

			char_x += glyph->advance;

			layout_count++;
		}
		buffer_index++;
	}
	lines[line_count].end_index = layout_count - 1;
	lines[line_count].start_index = start_index;
	*totalLines = line_count;
	*lineData = lines;
	*valid_entries = layout_count;
	return layout;
}