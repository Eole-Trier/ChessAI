#include "Tile.h"

#include "ChessBoard.h"

Tile::Tile()
{
    position = Vector2::Zero();
    size = 0;
}

Tile::Tile(const Vector2 position)
    : position(position)
{
}

void Tile::Initialize()
{
    size = static_cast<float>(ChessBoard::boardTexture->GetSize().x) / 8.f;
}

bool operator==(const Tile& lhs, const Tile& rhs)
{
    return lhs.position == rhs.position;
}

bool operator!=(const Tile& lhs, const Tile& rhs)
{
    return !(lhs == rhs);
}
