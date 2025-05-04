#pragma once
#include "Maths/vector2.hpp"

class Tile
{
public:
    Vector2 position;
    float size;

public:
    Tile();
    Tile(Vector2 position, float size);
};
