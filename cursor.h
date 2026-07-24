#pragma once
#ifndef CURSOR_H
#define CURSOR_H
#include <SDL3/SDL.h>
#include "layout.h"


int get_cursor_line_index(LineInfo *lineData, int cursor_index, int *total_lines);

void draw_Cursor(SDL_Renderer* render, LayoutChar* layout, int valid_entries, int layout_index, int cursor_offset, int *line_skip, int w_height);

void navigate_cursor_y(char *key, editorBuffer *Buffer, LineInfo *lineData, int *preferredCol, int *total_lines);

#endif