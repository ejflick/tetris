#include "player.h"
#include "shapes.h"
#include "board.h"

static Player player;

void InitPlayer() {
  player = (Player){.shape = SDL_rand(TOTAL_SHAPES), .rotation = 0, .x = 0, .y = 0, .lastFall = 0};
}

static void NextShape() {
  player.x = 4;
  player.y = 0;
  player.shape = SDL_rand(TOTAL_SHAPES);
  player.lastFall = 0;
  player.rotation = 0;
}

/// Moves the player down one space.
/// @return true if player moved down, false otherwise(collided).
static bool MoveDown() {
  if (CanPlaceShapeAt(player.x, player.y + 1, player.shape, player.rotation)) {
    player.y++;
    return true;
  }

  OccupyBoardCells(player.x, player.y, player.shape, player.rotation, ShapeColor(player.shape));
  NextShape();
  return false;
}

/// Immediately drops the piece as low as possible.
static void Teleport() {
  while (MoveDown()) {}
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
      if (ShapeCellAt(player.shape, player.rotation, x, y) != SHAPE_EMPTY_CELL) {
        Uint32 color = ShapeColor(player.shape);
        SDL_SetRenderDrawColor(
          renderer, 
          (color >> 16) & 0xff, 
          (color >> 8) & 0xff, 
          color & 0xff, 
          0xff
        );
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
          if (CanPlaceShapeAt(player.x - 1, player.y, player.shape, player.rotation))
            player.x -= 1;
        } break;

        case SDL_SCANCODE_RIGHT: {
          if (CanPlaceShapeAt(player.x + 1, player.y, player.shape, player.rotation))
            player.x += 1;
        } break;

        case SDL_SCANCODE_DOWN: {
          player.lastFall = 30;
        } break;

        case SDL_SCANCODE_UP: {
          Teleport();
        } break;

        default: break;
    }
}