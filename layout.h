#pragma once
#ifndef LAYOUT_H
#define LAYOUT_H
#include "gapbuffer.h"
#include "textureCache.h"
#include "SDL3/SDL.h"

typedef struct {
    char c;
    int index;
    float x;
    float y;
    Glyph* glyph;
} LayoutChar;

typedef struct {
    float x;
    float y;
}CursorPos;

LayoutChar* Build_Layout (editorBuffer textbuffer, SDL_Renderer* render, SDL_Window* window , int* valid_entires, CursorPos** idk);

#endif