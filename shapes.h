#ifndef _SHAPES_H
#define _SHAPES_H

#include <stddef.h>

#define T_SHAPE 0
#define LL_SHAPE 1
#define LR_SHAPE 2
#define SQUARE_SHAPE 3
#define I_SHAPE 4

#define EMPTY_CELL '.'
#define OCCUPIED_CELL '+'

typedef size_t Shape;
typedef size_t Rotation;

char ShapeCellAt(Shape shape, Rotation rotation, int x, int y);

#endif