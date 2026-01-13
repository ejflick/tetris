#include "player.h"
#include "shapes.h"
#include "board.h"

static Player player;

void InitPlayer() {
  player = (Player){.shape = T_SHAPE, .rotation = 0, .x = 0, .y = 0, .lastFall = 0};
}

static void NextShape() {
  player.x = 4;
  player.y = 0;
  player.shape = (player.shape + 1) % 5;
  player.lastFall = 0;
  player.rotation = 0;
}

static void MoveDown() {
  if (CanPlaceShapeAt(player.x, player.y + 1, player.shape, player.rotation)) {
    player.y++;
  } else {
    OccupyBoardCells(player.x, player.y, player.shape, player.rotation);
    NextShape();
  }
}

void TickPlayer() { 
  if (player.lastFall == 30) {
    MoveDown();
    player.lastFall = 0;
  }

  player.lastFall++;
}

void DrawPlayer(SDL_Renderer *renderer) {
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      if (ShapeCellAt(player.shape, player.rotation, x, y) == OCCUPIED_CELL) {
        SDL_SetRenderDrawColor(renderer, 0xed, 0x10, 0x21, 0xff);
        DrawCell(renderer, x + player.x, y + player.y);
      }
    }
  }
}

void HandlePlayerInput(SDL_Event event) {
    switch (event.key.scancode) {
        case SDL_SCANCODE_X: {
          int nextRotation = (player.rotation + 1) % 4;
          if (CanPlaceShapeAt(player.x, player.y, player.shape, nextRotation))
            player.rotation = nextRotation;
        } break;

        case SDL_SCANCODE_LEFT: {
          player.x -= 1;
        } break;

        case SDL_SCANCODE_RIGHT: {
          player.x += 1;
        } break;

        case SDL_SCANCODE_DOWN: {
          player.lastFall = 30;
        } break;

        default: break;
    }
}