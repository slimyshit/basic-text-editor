#include "gapbuffer.h"
#include <SDL3/SDL.h>
#include <stdbool.h>

void event_Handle(SDL_Event* event, editorBuffer* Buffer, bool* done) {
	if (event->type == SDL_EVENT_TEXT_INPUT) {
		Buffer_AddChar(event->text.text, Buffer);
	}
	if (event->type == SDL_EVENT_QUIT) {
		*done = true;
	}
	if (event->type == SDL_EVENT_KEY_DOWN) {

		if (event->key.key == SDLK_BACKSPACE) {
			Buffer_DelChar(Buffer);
		}
		if (event->key.key == SDLK_RETURN || event->key.key == SDLK_KP_ENTER) {
			Buffer_NewLine(Buffer);
		}
	}
}