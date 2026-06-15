#pragma once
#ifndef TEXTURECACHE_H
#define TEXTURECACHE_H
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

typedef struct
{
	SDL_Texture* texture;
	int advance;
	int width;
	int height;
} Glyph;

void Init_Texture(SDL_Renderer* renderr);

Glyph* get_Glyph(char character);

void Destroy_Cache();


#endif