#include "player.h"
#include "board.h"
#include "shapes.h"

#define NO_SAVED_SHAPE -1

static Player player;
static Shape shapeQueue[TOTAL_SHAPES];
static int queuePos;

static void ShuffleQueue() {
  // Queue in numerical order.
  for (int i = 0; i < TOTAL_SHAPES; i++) {
    shapeQueue[i] = i;
  }

  // Randomly swap elements.
  for (int i = 0; i < TOTAL_SHAPES; i++) {
    Sint32 swapWith = SDL_rand(TOTAL_SHAPES);
    Shape tmp = shapeQueue[i];
    shapeQueue[i] = shapeQueue[swapWith];
    shapeQueue[swapWith] = tmp;
  }

  queuePos = 0;
}

void InitPlayer() {
  ShuffleQueue();
  player = (Player){.shape = queuePos,
                    .rotation = 0,
                    .x = 0,
                    .y = 0,
                    .lastFall = 0,
                    .rotateGracePeriod = 0,
                    .canSwitch = true,
                    .saved = NO_SAVED_SHAPE};
}

static void NextShape() {
  queuePos += 1;

  if (queuePos == TOTAL_SHAPES) {
    ShuffleQueue();
  }

  player.x = 4;
  player.y = 0;
  player.shape = shapeQueue[queuePos];
  player.lastFall = 0;
  player.rotation = 0;
  player.rotateGracePeriod = 0;
  player.canSwitch = true;
}

static void SaveShape() {
  if (!player.canSwitch) return;

  if (player.saved == NO_SAVED_SHAPE) {
    player.saved = player.shape;
    NextShape();
  } else {
    Shape tmp = player.shape;
    player.shape = player.saved;
    player.saved = tmp;
    player.y = 0;
    player.canSwitch = false;
  }
}

/// Moves the player down one space.
/// @return {bool} true if player moved down, false otherwise(collided).
static bool MoveDown() {
  if (CanPlaceShapeAt(player.x, player.y + 1, player.shape, player.rotation)) {
    player.y++;
    return true;
  }

  if (player.rotateGracePeriod == 0) {
    OccupyBoardCells(player.x, player.y, player.shape, player.rotation,
                    ShapeColor(player.shape));
    NextShape();
  } else {
    player.rotateGracePeriod--;
  }
  return false;
}

/// Immediately drops the piece as low as possible.
static void Teleport() {
  while (MoveDown()) {
  }
}

void TickPlayer() {
  if (player.lastFall == 30) {
    MoveDown();
    player.lastFall = 0;
  }

  player.lastFall++;
}

void DrawPlayer(SDL_Renderer* renderer) {
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      if (ShapeCellAt(player.shape, player.rotation, x, y) !=
          SHAPE_EMPTY_CELL) {
        DrawCell(renderer, x + player.x, y + player.y,
                 ShapeColor(player.shape));
      }
    }
  }
}

void DrawHud(SDL_Renderer* renderer) {
  DrawShape(renderer, shapeQueue[(queuePos + 1) % TOTAL_SHAPES], 0, 32, 32);

  if (player.saved != NO_SAVED_SHAPE) {
    DrawShape(renderer, player.saved, 0, 32, 96);
  }
}

void HandlePlayerInput(SDL_Event event) {
  switch (event.key.scancode) {
  case SDL_SCANCODE_Z: {
    SaveShape();
  } break;

  case SDL_SCANCODE_X: {
    int nextRotation = (player.rotation + 1) % 4;
    if (CanPlaceShapeAt(player.x, player.y, player.shape, nextRotation)) {
      player.rotation = nextRotation;
    } else if (CanPlaceShapeAt(player.x - 1, player.y, player.shape, nextRotation)) {
      player.x -= 1;
      player.rotation = nextRotation;
    } else if (CanPlaceShapeAt(player.x + 1, player.y, player.shape, nextRotation)) {
      player.x += 1;
      player.rotation = nextRotation;
    }
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

  default:
    break;
  }
}