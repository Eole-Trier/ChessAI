#include "Tile.h"

Tile::Tile()
{
    position = Vector2::Zero();
    size = 0;
}

Tile::Tile(const Vector2 position, const float size)
    : position(position), size(size)
{
}
