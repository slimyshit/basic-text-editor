#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_video.h>
#include <stdbool.h>

int main() {
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window =
      SDL_CreateWindow("SUSSYBAKA", 1000, 1000, SDL_WINDOW_RESIZABLE);
  SDL_Event event;
  int close = 1;
  while (close == 1) {
    SDL_ShowWindow(window);
    SDL_RaiseWindow(window);
    SDL_PollEvent(&event);
    if (event.type == SDL_EVENT_QUIT) {
      close = 0;
    }
  }
  SDL_DestroyWindow(window);
  SDL_Quit();
}
