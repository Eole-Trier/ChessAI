#include "Piece.h"

#include "ChessBoard.h"
#include "Mountain/window.hpp"
#include "Mountain/input/input.hpp"
#include "Mountain/rendering/draw.hpp"
#include "Mountain/resource/resource_manager.hpp"

Piece::Piece(const bool isWhite, const PieceType pieceType, Tile* tile)
    : isWhite(isWhite), pieceType(pieceType), globalPosition(tile->position), tile(tile)
{
    scaling = Vector2(0.363f);
    tilePos = ChessBoard::ToTiles(tile->position);
}

Piece::~Piece()
{
}

void Piece::Render()
{
    const int index = isWhite ? static_cast<int>(pieceType) : static_cast<int>(pieceType) + 6;

    Mountain::Draw::Texture(*piecesTextures[index], globalPosition, scaling, 0.f, Vector2(0.5f));
}

void Piece::Move(const Vector2i newPosition)
{
    globalPosition = ChessBoard::ToPixels(newPosition);
}

void Piece::GetAvailableTiles(Mountain::List<Tile>& result) const
{
    switch (pieceType)
    {
        case PieceType::King:
            GetKingAvailableTiles(result);
            break;
        case PieceType::Queen:
            GetQueenAvailableTiles(result);
            break;
        case PieceType::Rook:
            GetRookAvailableTiles(result);
            break;
        case PieceType::Bishop:
            GetBishopAvailableTiles(result);
            break;
        case PieceType::Knight:
            GetKnightAvailableTiles(result);
            break;
        case PieceType::Pawn:
            GetPawnAvailableTiles(result);
            break;
    }
}

void Piece::GetPawnAvailableTiles(Mountain::List<Tile>& result) const
{
    Mountain::List<Vector2i> tilesPos;

    if (!isMoved)
    {
        if (isWhite)
            tilesPos.Add(tilePos + Vector2i(0, -2));
        else
            tilesPos.Add(tilePos + Vector2i(0, 2));
    }
    if (isWhite)
    {
        tilesPos.Add(tilePos + Vector2i(0, -1));
        if (ChessBoard::IsTherePieceOnTile(tilePos + Vector2i(-1, -1)))
        {
            tilesPos.Add(tilePos + Vector2i(-1, -1));
        }
        if (ChessBoard::IsTherePieceOnTile(tilePos + Vector2i(1, -1)))
        {
            tilesPos.Add(tilePos + Vector2i(1, -1));
        }
    }
    else
    {
        tilesPos.Add(tilePos + Vector2i(0, 1));
        if (ChessBoard::IsTherePieceOnTile(tilePos + Vector2i(-1, 1)))
        {
            tilesPos.Add(tilePos + Vector2i(-1, 1));
        }
        if (ChessBoard::IsTherePieceOnTile(tilePos + Vector2i(1, 1)))
        {
            tilesPos.Add(tilePos + Vector2i(1, 1));
        }
    }

    if (ChessBoard::IsTherePieceOnTile(tilesPos[0]))
        tilesPos.Remove(tilesPos[0]);

    if (!isMoved)
        if (ChessBoard::IsTherePieceOnTile(tilesPos[0]))
            tilesPos.Remove(tilesPos[0]);

    ChessBoard::AddTileIfInBoard(tilesPos, result);
}

void Piece::GetKnightAvailableTiles(Mountain::List<Tile>& result) const
{
    Mountain::List<Vector2i> tilesPos;
    tilesPos.Add(tilePos + Vector2i(1, 2));
    tilesPos.Add(tilePos + Vector2i(1, -2));
    tilesPos.Add(tilePos + Vector2i(2, 1));
    tilesPos.Add(tilePos + Vector2i(2, -1));
    tilesPos.Add(tilePos + Vector2i(-1, 2));
    tilesPos.Add(tilePos + Vector2i(-1, -2));
    tilesPos.Add(tilePos + Vector2i(-2, 1));
    tilesPos.Add(tilePos + Vector2i(-2, -1));
    ChessBoard::AddTileIfInBoard(tilesPos, result);
}

void Piece::GetBishopAvailableTiles(Mountain::List<Tile>& result) const
{
}

void Piece::GetRookAvailableTiles(Mountain::List<Tile>& result) const
{
}

void Piece::GetQueenAvailableTiles(Mountain::List<Tile>& result) const
{
}

void Piece::GetKingAvailableTiles(Mountain::List<Tile>& result) const
{
}

void Piece::LoadResources()
{
    piecesTextures[0]  = Mountain::ResourceManager::Get<Mountain::Texture>("assets/images/wk.png");
    piecesTextures[1]  = Mountain::ResourceManager::Get<Mountain::Texture>("assets/images/wq.png");
    piecesTextures[2]  = Mountain::ResourceManager::Get<Mountain::Texture>("assets/images/wr.png");
    piecesTextures[3]  = Mountain::ResourceManager::Get<Mountain::Texture>("assets/images/wb.png");
    piecesTextures[4]  = Mountain::ResourceManager::Get<Mountain::Texture>("assets/images/wn.png");
    piecesTextures[5]  = Mountain::ResourceManager::Get<Mountain::Texture>("assets/images/wp.png");
    piecesTextures[6]  = Mountain::ResourceManager::Get<Mountain::Texture>("assets/images/bk.png");
    piecesTextures[7]  = Mountain::ResourceManager::Get<Mountain::Texture>("assets/images/bq.png");
    piecesTextures[8]  = Mountain::ResourceManager::Get<Mountain::Texture>("assets/images/br.png");
    piecesTextures[9]  = Mountain::ResourceManager::Get<Mountain::Texture>("assets/images/bb.png");
    piecesTextures[10] = Mountain::ResourceManager::Get<Mountain::Texture>("assets/images/bn.png");
    piecesTextures[11] = Mountain::ResourceManager::Get<Mountain::Texture>("assets/images/bp.png");
}
