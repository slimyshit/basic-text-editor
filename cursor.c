#include <stdio.h>
#include "cursor.h"
#include "gapbuffer.h"


int get_cursor_line_index(LineInfo* lineData, int cursor_index, int* total_lines)
{
	for (int i = 0; i < *total_lines; i++)
	{
		if (cursor_index >= lineData[i].start_index && cursor_index <= lineData[i].end_index)
		{
			//printf("cursor_index = %d    start_index = %d    end_index = %d    total_lines = %d    i = %d \n", cursor_index, lineData[i].start_index, lineData[i].end_index, total_lines, i);
			return i;
		}
	}
	return *total_lines;
}

void draw_Cursor(SDL_Renderer* render, LayoutChar* layout, int valid_entries, int layout_index, int line_offset)
{
	SDL_FRect cursor;
	cursor.w = 3;
	cursor.h = 20;
	int offset = 0;
	if (line_offset > 0) offset = line_offset;
	//printf("cursor=%d valid=%d\n", layout_index, valid_entries);
	if (layout == NULL || layout_index == 0) {
		cursor.x = 10;
		cursor.y = 10;
	}

	else if (layout[layout_index - 1].c == '\n') {
		cursor.x = layout[layout_index - 1].x;
		cursor.y = layout[layout_index - 1].y - line_offset;
	}
	else if (layout_index < valid_entries) {
		Glyph* glyph = layout[layout_index - 1].glyph;
		if (glyph != NULL) {
			int glyph_width = glyph->advance;
			cursor.x = layout[layout_index - 1].x + glyph_width;
			cursor.y = layout[layout_index - 1].y - line_offset;
		}

	}
	else {
		Glyph* glyph = layout[layout_index - 1].glyph;
		if (glyph != NULL) {
			int x = glyph->advance;
			cursor.x = layout[layout_index - 1].x + x;
			cursor.y = layout[layout_index - 1].y - line_offset;
		}
	}
	SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
	SDL_RenderFillRect(render, &cursor);
}

void navigate_cursor_y(char* key, editorBuffer* Buffer, LineInfo* lineData, int* preferredCol, int* total_lines,  int *line_skip_count)
{
	int current_line = get_cursor_line_index(lineData, Buffer->cursor, total_lines);
	
	if (strcmp(key, "up") == 0 && current_line > 0)
	{
		int new_line = current_line - 1;

		int line_len = lineData[new_line].end_index - lineData[new_line].start_index ;
		int new_cursor_col = 0;
		int temp = *preferredCol;
		printf("preferred col = %d\n", temp);
		if (*preferredCol < line_len)
		{
			new_cursor_col = lineData[new_line].start_index + *preferredCol;
		}
		else {
			new_cursor_col = lineData[new_line].end_index;
		}
		Buffer_navigate_cursor_y(Buffer, "up", new_cursor_col);
		(*line_skip_count)--;
	}

	if (strcmp(key, "down") == 0 && current_line < *total_lines)
	{
		int new_line = current_line + 1;

		int line_len = lineData[new_line].end_index - lineData[new_line].start_index;
		int new_cursor_col = 0;

		if (*preferredCol < line_len)
		{
			new_cursor_col = lineData[new_line].start_index + *preferredCol;
		}
		else {
			new_cursor_col = lineData[new_line].end_index;
		}
		Buffer_navigate_cursor_y(Buffer, "down", new_cursor_col);
		(*line_skip_count)++;
	}
}