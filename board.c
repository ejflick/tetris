#include "board.h"

static char board[TOTAL_CELLS];

void InitBoard() { 
  SDL_memset(board, EMPTY_CELL, sizeof(board));
}

int CellAt(int x, int y) { 
  if (y >= ROWS || y < 0 || x >= COLS || x < 0) return OCCUPIED_CELL;
  return board[(y * COLS) + x];
}

void DrawCell(SDL_Renderer *renderer, float x, float y) {
  SDL_FRect rect = {.x = (float)(CELL_SIZE * x) + (CELL_MARGIN * x),
                    .y = (float)(CELL_SIZE * y) + (CELL_MARGIN * y),
                    .w = CELL_SIZE - 1,
                    .h = CELL_SIZE - 1};
  SDL_RenderFillRect(renderer, &rect);
}

void DrawBoard(SDL_Renderer *renderer) {
  for (int y = 0; y < ROWS; y++) {
    for (int x = 0; x < COLS; x++) {
      if (CellAt(x, y) == EMPTY_CELL) {
        SDL_SetRenderDrawColor(renderer, 0x11, 0x11, 0x11, 0xff);
      } else {
        SDL_SetRenderDrawColor(renderer, 0xee, 0x30, 0x11, 0xff);
      }

      DrawCell(renderer, x, y);
    }
  }
}

bool CanPlaceShapeAt(int x, int y, Shape shape, Rotation rotation) {
  // TODO: Remove magic number. The data of all shapes is represented by a 4x4 shape.
  for (int sy = 0; sy < 4; sy++) {
    for (int sx = 0; sx < 4; sx++) {
      if (ShapeCellAt(shape, rotation, sx, sy) == EMPTY_CELL) continue;

      if (CellAt(x + sx, y + sy) == OCCUPIED_CELL) {
        return false;
      }
    }
  }

  return true;
}

void OccupyBoardCells(int x, int y, Shape shape, Rotation rotation) {
  // TODO: More magic numbers.
  for (int sy = 0; sy < 4; sy++) {
    for (int sx = 0; sx < 4; sx++) {
      if (ShapeCellAt(shape, rotation, sx, sy) == EMPTY_CELL) continue;

      board[((sy + y) * COLS + (sx + x))] = OCCUPIED_CELL;
    }
  }
}