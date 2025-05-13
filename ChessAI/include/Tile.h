#pragma once
#include "Maths/vector2.hpp"
#include "Maths/vector2i.hpp"

class Tile
{
    friend bool operator==(const Tile& lhs, const Tile& rhs);

    friend bool operator!=(const Tile& lhs, const Tile& rhs);

public:
    Vector2 position;
    Vector2i tilePosition;
    static inline float size;

public:
    Tile();
    explicit Tile(Vector2i tilePosition);

public:
    static void Initialize();
};
