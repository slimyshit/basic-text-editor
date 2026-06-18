#include <stdio.h>
#include "cursor.h"

int get_cursor_line_index(LineInfo* lineData, int cursor_index, int total_lines)
{
	for (int i = 0; i <= total_lines; i++)
	{
		if (cursor_index >= lineData[i].start_index && cursor_index <= lineData[i].end_index)
		{
			printf("cursor_index = %d    start_index = %d    end_index = %d    total_lines = %d    i = %d \n", cursor_index, lineData[i].start_index, lineData[i].end_index, total_lines, i);
			return i;
		}
	}
	return 0;
}

void draw_Cursor(SDL_Renderer* render, LayoutChar* layout, int valid_entries, int layout_index)
{
	SDL_FRect cursor;
	cursor.w = 3;
	cursor.h = 20;
	//printf("cursor=%d valid=%d\n", layout_index, valid_entries);
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