#pragma once
#ifndef CURSOR_H
#define CURSOR_H
#include <SDL3/SDL.h>
#include "layout.h"


int get_cursor_line_index(LineInfo* lineData, int cursor_index, int total_lines);

void draw_Cursor(SDL_Renderer* render, LayoutChar* layout, int valid_entries, int layout_index);


#endif