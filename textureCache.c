#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "textureCache.h"


TTF_Font* font;
TTF_TextEngine* engine;
SDL_Color color;
SDL_Renderer* render;
static Glyph glyphs[256];

void generate_Texture(char character)
{
	char text[2] = { character, '\0' };
	SDL_Surface* surface = TTF_RenderText_Blended(font, text, 0, color);
	if (!surface)
	{
		printf("surface fking failed");
		return;
	}
	SDL_Texture* texture = SDL_CreateTextureFromSurface(render, surface);
	if (!texture)
	{
		printf("texture ffing failed");
		return;
	}
	SDL_DestroySurface(surface);
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	glyphs[(unsigned char)character].texture = texture;
}

void get_Metrics(char character)
{
	int character_w, character_h;
	char text[2] = { character, '\0' };
	TTF_GetStringSize(font, text, 1, &character_w, &character_h);
	glyphs[(unsigned char)character].advance = character_w;
	glyphs[(unsigned char)character].width = character_w;
	glyphs[(unsigned char)character].height = character_h;

}

void Init_Texture(SDL_Renderer* renderr)
{
	render = renderr;
	font = TTF_OpenFont("C:\\Windows\\Fonts\\arial.ttf", 16.0);
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;
}

void Destroy_Cache()
{
	TTF_DestroyRendererTextEngine(engine);
	
	for (int i = 0; i < 256; i++)
	{
		if (glyphs[i].texture)
		{
			SDL_DestroyTexture(glyphs[i].texture);
		}
	}
	TTF_CloseFont(font);
}

Glyph* get_Glyph(char character)
{
	Glyph* received = &glyphs[(unsigned char)character];
	if (received->texture == NULL)
	{
		generate_Texture(character);
		get_Metrics(character);	
	}
	return received;
}