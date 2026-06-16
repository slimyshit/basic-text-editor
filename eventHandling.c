#include "gapbuffer.h"
#include <SDL3/SDL.h>
#include <stdbool.h>

void event_Handle(SDL_Event* event, editorBuffer* Buffer, bool* done, bool* layout_dirty) {
	if (event->type == SDL_EVENT_TEXT_INPUT) {
		Buffer_AddChar(event->text.text, Buffer);
		*layout_dirty = true;
	}
	if (event->type == SDL_EVENT_QUIT) {
		*done = true;
	}
	if (event->type == SDL_EVENT_KEY_DOWN) {

		if (event->key.key == SDLK_BACKSPACE) {
			Buffer_DelChar(Buffer);
			*layout_dirty = true;
		}
		else if (event->key.key == SDLK_RETURN || event->key.key == SDLK_KP_ENTER) {
			Buffer_NewLine(Buffer);
			*layout_dirty = true;
		}
		else if(event->key.key == SDLK_RIGHT)
		{
			Buffer_navigate_cursor_x("right", Buffer);
			*layout_dirty = true;
		}
		else if (event->key.key == SDLK_LEFT)
		{
			Buffer_navigate_cursor_x("left", Buffer);
			*layout_dirty = true;
			
		}
		else if (event->key.key == SDLK_UP)
		{
			//Buffer_navigate_cursor_y("up", Buffer);
		}
		else if (event->key.key == SDLK_DOWN)
		{
			//Buffer_navigate_cursor_y("down", Buffer);
		}
		
	}
}	