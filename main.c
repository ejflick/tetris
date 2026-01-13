#include <stddef.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>

#include "board.h"
#include "player.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

int main(int argc, char *args[]) {
  SDL_Init(SDL_INIT_VIDEO);

  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_CreateWindowAndRenderer("Tetris", SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window,
                              &renderer);
  SDL_Texture *boardTexture =
      SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                        SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);

  InitBoard();
  InitPlayer();

  SDL_srand(0);

  bool quit = false;
  SDL_Event e;
  SDL_zero(e);
  Uint64 lastFrame = SDL_GetTicks();
  Uint64 elapsed = 0;
  
  while (quit == false) {
    while (SDL_PollEvent(&e) == true) {
      if (e.type == SDL_EVENT_QUIT) {
        quit = true;
      } else if (e.type == SDL_EVENT_KEY_DOWN) {
        switch (e.key.scancode) {
        case SDL_SCANCODE_ESCAPE: {
          quit = true;
        } break;
        default:
          HandlePlayerInput(e);
          break;
        }
      }
    }

    bool render = false;
    Uint64 now = SDL_GetTicks();
    elapsed += now - lastFrame;
    while (elapsed > 16) {
      TickPlayer();
      render = true;
      elapsed -= 16;
    }

    if (render) {
      SDL_SetRenderTarget(renderer, boardTexture);
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);
        SDL_RenderClear(renderer);
        DrawBoard(renderer);
        DrawPlayer(renderer);
      SDL_SetRenderTarget(renderer, NULL);

      SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);
      SDL_RenderClear(renderer);
      SDL_FRect src = {.x = 0, .y = 0, .w = SCREEN_WIDTH, .h = SCREEN_HEIGHT};
      SDL_FRect dest = src;
      dest.x += 32; dest.y += 32;

      SDL_RenderTexture(renderer, boardTexture, &src, &dest);

      SDL_RenderPresent(renderer);
    }

    lastFrame = now;
    SDL_Delay(1);
  }

  SDL_DestroyTexture(boardTexture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
