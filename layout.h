#pragma once
#ifndef LAYOUT_H
#define LAYOUT_H
#include "gapbuffer.h"
#include "textureCache.h"
#include "SDL3/SDL.h"

typedef struct {
    char c;
    float x;
    float y;
    Glyph* glyph;
} LayoutChar;

typedef struct {
    int start_index;
    int end_index;
}LineInfo;


LayoutChar* Build_Layout(editorBuffer Buffer, SDL_Renderer* render, SDL_Window* window, int* valid_entries, LineInfo** lineData, int* totalLines, int* line_offset);

#endif