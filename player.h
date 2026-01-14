#ifndef _PLAYER_H
#define _PLAYER_H

#include <SDL3/SDL.h>
#include "shapes.h"

typedef struct {
  Shape shape;
  Rotation rotation;
  int x;
  int y;
  int lastFall;
  int rotateGracePeriod;
  bool rotationPressed;
} Player;

void InitPlayer();
void TickPlayer();
void DrawPlayer(SDL_Renderer *renderer);
void HandlePlayerInput(SDL_Event event);

#endif