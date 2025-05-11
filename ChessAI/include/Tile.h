#pragma once
#include "Maths/vector2.hpp"

class Tile
{
    friend bool operator==(const Tile& lhs, const Tile& rhs);

    friend bool operator!=(const Tile& lhs, const Tile& rhs);

public:
    Vector2 position;
    static inline float size;

public:
    Tile();
    explicit Tile(Vector2 position);

public:
    static void Initialize();
};
