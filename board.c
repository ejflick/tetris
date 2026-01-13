#include "board.h"
#include <stdint.h>

#define BOARD_EMPTY_CELL 0

// If a cell is empty, it will be 0. Otherwise, it will be the 24-bit RGB color of the cell.
static uint32_t board[TOTAL_CELLS];

void InitBoard() { 
  SDL_memset(board, BOARD_EMPTY_CELL, sizeof(board));
}

uint32_t CellAt(int x, int y) { 
  if (y >= ROWS || y < 0 || x >= COLS || x < 0) return 1;
  return board[(y * COLS) + x];
}

void DrawCell(SDL_Renderer *renderer, float x, float y, uint32_t color) {
  SDL_SetRenderDrawColor(
    renderer, 
    (color >> 16) & 0xff, 
    (color >> 8) & 0xff, 
    color & 0xff, 
    0xff
  );
  SDL_FRect rect = {.x = (float)(CELL_SIZE * x) + (CELL_MARGIN * x),
                    .y = (float)(CELL_SIZE * y) + (CELL_MARGIN * y),
                    .w = CELL_SIZE - 1,
                    .h = CELL_SIZE - 1};
  SDL_RenderFillRect(renderer, &rect);
}

void DrawBoard(SDL_Renderer *renderer) {
  for (int y = 0; y < ROWS; y++) {
    for (int x = 0; x < COLS; x++) {
      uint32_t cell = CellAt(x, y);
      DrawCell(renderer, x, y, cell == 0 ? 0x111111 : cell);
    }
  }
}

bool CanPlaceShapeAt(int x, int y, Shape shape, Rotation rotation) {
  // TODO: Remove magic number. The data of all shapes is represented by a 4x4 shape.
  for (int sy = 0; sy < 4; sy++) {
    for (int sx = 0; sx < 4; sx++) {
      if (ShapeCellAt(shape, rotation, sx, sy) == SHAPE_EMPTY_CELL) continue;

      if (CellAt(x + sx, y + sy) > 0) {
        return false;
      }
    }
  }

  return true;
}

static void ClearRows() {
  for (int row = 0; row < ROWS; row++) {
    bool canBeCleared = true;
    for (int col = 0; col < COLS; col++) {
      canBeCleared = canBeCleared && (CellAt(col, row) != BOARD_EMPTY_CELL);
    }

    if (canBeCleared) {
      // Move rows above down one.
      for (int crow = row; crow > 0; crow--) {
        for (int ccol = 0; ccol < COLS; ccol++) {
          board[(crow * COLS) + ccol] = board[((crow - 1) * COLS) + ccol];
        }
      }
    }
  }
}

void OccupyBoardCells(int x, int y, Shape shape, Rotation rotation, uint32_t color) {
  // TODO: More magic numbers.
  for (int sy = 0; sy < 4; sy++) {
    for (int sx = 0; sx < 4; sx++) {
      if (ShapeCellAt(shape, rotation, sx, sy) == SHAPE_EMPTY_CELL) continue;

      board[((sy + y) * COLS) + (sx + x)] = color;
    }
  }

  ClearRows();
}