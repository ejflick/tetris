#ifndef _SHAPES_H
#define _SHAPES_H

#include <stddef.h>
#include <stdint.h>

#define SHAPE_EMPTY_CELL '.'

typedef size_t Shape;
typedef size_t Rotation;

char ShapeCellAt(Shape shape, Rotation rotation, int x, int y);
uint32_t ShapeColor(Shape shape);

#endif