#pragma once

#include <cstdint>

#include "Mountain/resource/texture.hpp"

class Tile;

enum class PieceType : uint8_t
{
    King,
    Queen,
    Rook,
    Bishop,
    Knight,
    Pawn
};

class Piece
{
public:
    bool isWhite;
    PieceType pieceType = PieceType::Pawn;
    Vector2 globalPosition;
    Vector2i tilePosition;
    Vector2 scaling;

    Tile* tile;

    bool isMoved = false;
    bool isDragged = false;

public:
    Piece(bool isWhite, PieceType pieceType, Tile* tile);

    void Render();
    void GetAvailableTiles(Mountain::List<Tile*>& result) const;
    void GetPawnAvailableTiles(Mountain::List<Tile*>& result) const;
    void GetBishopAvailableTiles(Mountain::List<Tile*>& result) const;
    void GetKnightAvailableTiles(Mountain::List<Tile*>& result) const;
    void GetRookAvailableTiles(Mountain::List<Tile*>& result) const;
    void GetQueenAvailableTiles(Mountain::List<Tile*>& result) const;
    void GetKingAvailableTiles(Mountain::List<Tile*>& result) const;

    void Move(Tile* newTile);
    template <size_t Size>
    void AddTilesIfNoAllyOnIt(const std::array<Tile*, Size>& tiles, Mountain::List<Tile*>& result) const;
public:
    static void LoadResources();

private:
    static inline std::array<Mountain::Pointer<Mountain::Texture>, 12> piecesTextures;
};

#include "ChessBoard.h"

template <size_t Size>
void Piece::AddTilesIfNoAllyOnIt(const std::array<Tile*, Size>& tiles, Mountain::List<Tile*>& result) const
{
    for (Tile* t : tiles)
    {
        if (!t)
            continue;

        const Piece* piece = ChessBoard::GetPieceFromTileSafe(t->tilePosition);
        if (piece && piece->isWhite == isWhite)
            continue;

        result.Add(t);
    }
}
