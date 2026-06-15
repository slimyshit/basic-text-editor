#pragma once
#ifndef EVENTHANDLING_H
#define EVENTHANDLING_H	
#include "gapbuffer.h"
#include <SDL3/SDL.h>
#include <stdbool.h>

void event_Handle(SDL_Event* event, editorBuffer* Buffer, bool* done, bool* move_cursor);
#endif // !EVENTHANDLING.H
