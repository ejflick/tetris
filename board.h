#ifndef _BOARD_H
#define _BOARD_H

#include <SDL3/SDL.h>
#include "shapes.h"

#define ROWS 20
#define COLS 15
#define TOTAL_CELLS (ROWS * COLS)

#define CELL_MARGIN 3
#define CELL_SIZE 24

void InitBoard();

uint32_t CellAt(int x, int y);

void DrawCell(SDL_Renderer *renderer, float x, float y);
void DrawBoard(SDL_Renderer *renderer);

bool CanPlaceShapeAt(int x, int y, Shape shape, Rotation rotation);
void OccupyBoardCells(int x, int y, Shape shape, Rotation rotation, uint32_t color);

#endif