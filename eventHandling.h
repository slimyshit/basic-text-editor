#pragma once
#ifndef EVENTHANDLING_H
#define EVENTHANDLING_H	
#include "gapbuffer.h"
#include "layout.h"
#include <SDL3/SDL.h>
#include <stdbool.h>

void event_Handle(SDL_Event* event, editorBuffer* Buffer, LineInfo* lineData, bool* done, bool* layout_dirty, int* preferredCol, int cursor_index, int total_lines);
#endif // !EVENTHANDLING.H
