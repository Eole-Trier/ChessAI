#include "Piece.h"

#include "Mountain/window.hpp"
#include "Mountain/input/input.hpp"
#include "Mountain/rendering/draw.hpp"
#include "Mountain/resource/resource_manager.hpp"

Piece::Piece(const bool isWhite, const PieceType pieceType, Tile* tile)
    : isWhite(isWhite), pieceType(pieceType), globalPosition(tile->position), tile(tile)
{
    scaling = Vector2(0.363f);
    tilePosition = tile->tilePosition;
}

void Piece::Render()
{
    const int index = isWhite ? static_cast<int>(pieceType) : static_cast<int>(pieceType) + 6;

    Mountain::Draw::Texture(*piecesTextures[index], globalPosition, scaling, 0.f, Vector2(0.5f));
}

void Piece::GetAvailableTiles(Mountain::List<Tile*>& result) const
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

void Piece::GetPawnAvailableTiles(Mountain::List<Tile*>& result) const
{
    const Vector2i forward = isWhite ? -Vector2i::UnitY() : Vector2i::UnitY();

    const Vector2i forwardTilePosition = tilePosition + forward;
    if (!ChessBoard::IsTherePieceOnTile(forwardTilePosition))
    {
        result.Add(ChessBoard::GetTileSafe(forwardTilePosition));

        const Vector2i twoTilesForwardTilePosition = forwardTilePosition + forward;
        if (!isMoved && !ChessBoard::IsTherePieceOnTile(twoTilesForwardTilePosition))
            result.Add(ChessBoard::GetTileSafe(twoTilesForwardTilePosition));
    }

    if (ChessBoard::enPassantPiece)
    {
        const Piece* rightPiece = ChessBoard::GetPieceFromTileSafe(tilePosition + Vector2i::UnitX());
        if (rightPiece == ChessBoard::enPassantPiece)
            result.Add(ChessBoard::GetTileSafe(rightPiece->tilePosition + forward));
        else
        {
            const Piece* leftPiece = ChessBoard::GetPieceFromTileSafe(tilePosition - Vector2i::UnitX());
            if (leftPiece == ChessBoard::enPassantPiece)
                result.Add(ChessBoard::GetTileSafe(leftPiece->tilePosition + forward));
        }
    }

    const Piece* topLeftPiece = ChessBoard::GetPieceFromTileSafe(tilePosition + (forward - Vector2i::UnitX()));
    if (topLeftPiece && topLeftPiece->isWhite != isWhite)
        result.Add(topLeftPiece->tile);

    const Piece* topRightPiece = ChessBoard::GetPieceFromTileSafe(tilePosition + (forward + Vector2i::UnitX()));
    if (topRightPiece && topRightPiece->isWhite != isWhite)
        result.Add(topRightPiece->tile);
}

void Piece::GetKnightAvailableTiles(Mountain::List<Tile*>& result) const
{
    const std::array tiles = {
        ChessBoard::GetTileSafe(tilePosition + Vector2i(1, 2)),
        ChessBoard::GetTileSafe(tilePosition + Vector2i(1, -2)),
        ChessBoard::GetTileSafe(tilePosition + Vector2i(2, 1)),
        ChessBoard::GetTileSafe(tilePosition + Vector2i(2, -1)),
        ChessBoard::GetTileSafe(tilePosition + Vector2i(-1, 2)),
        ChessBoard::GetTileSafe(tilePosition + Vector2i(-1, -2)),
        ChessBoard::GetTileSafe(tilePosition + Vector2i(-2, 1)),
        ChessBoard::GetTileSafe(tilePosition + Vector2i(-2, -1))
    };

    AddTilesIfNoFreroOnIt(tiles, result);
}

void Piece::GetBishopAvailableTiles(Mountain::List<Tile*>& result) const
{
    for (int x = -1; x <= 1; x += 2)
    {
        for (int y = -1; y <= 1; y += 2)
        {
            int offset = 1;
            Tile* tileBishop = ChessBoard::GetTileSafe({tilePosition.x + x, tilePosition.y + y});

            while (tileBishop != nullptr)
            {
                if (ChessBoard::IsTherePieceOnTile(tileBishop->tilePosition))
                {
                    const Piece* p = ChessBoard::GetPieceFromTileSafe(tileBishop->tilePosition);
                    if (p->isWhite != isWhite)
                        result.Add(tileBishop);
                    break;
                }
                result.Add(tileBishop);

                offset++;
                tileBishop = ChessBoard::GetTileSafe({tilePosition.x + x * offset, tilePosition.y + y * offset});
            }
        }
    }
}

void Piece::GetRookAvailableTiles(Mountain::List<Tile*>& result) const
{
    for (int i = -1; i <= 1; i += 2)
    {
        int offset = 1;
        Tile* tileRook = ChessBoard::GetTileSafe({tilePosition.x + i, tilePosition.y});

        while (tileRook != nullptr)
        {
            if (ChessBoard::IsTherePieceOnTile(tileRook->tilePosition))
            {
                const Piece* p = ChessBoard::GetPieceFromTileSafe(tileRook->tilePosition);
                if (p->isWhite != isWhite)
                    result.Add(tileRook);
                break;
            }
            result.Add(tileRook);

            offset++;
            tileRook = ChessBoard::GetTileSafe({tilePosition.x + i * offset, tilePosition.y});
        }
    }
    for (int i = -1; i <= 1; i += 2)
    {
        int offset = 1;
        Tile* tileRook = ChessBoard::GetTileSafe({tilePosition.x, tilePosition.y + i});

        while (tileRook != nullptr)
        {
            if (ChessBoard::IsTherePieceOnTile(tileRook->tilePosition))
            {
                const Piece* p = ChessBoard::GetPieceFromTileSafe(tileRook->tilePosition);
                if (p->isWhite != isWhite)
                    result.Add(tileRook);
                break;
            }
            result.Add(tileRook);

            offset++;
            tileRook = ChessBoard::GetTileSafe({tilePosition.x, tilePosition.y + i * offset});
        }
    }
}

void Piece::GetQueenAvailableTiles(Mountain::List<Tile*>& result) const
{
    GetBishopAvailableTiles(result);
    GetRookAvailableTiles(result);
}

void Piece::GetKingAvailableTiles(Mountain::List<Tile*>& result) const
{
     const std::array tiles = {
        ChessBoard::GetTileSafe(tilePosition + Vector2i(1, 1)),
        ChessBoard::GetTileSafe(tilePosition + Vector2i(1, 0)),
        ChessBoard::GetTileSafe(tilePosition + Vector2i(1, -1)),
        ChessBoard::GetTileSafe(tilePosition + Vector2i(0, -1)),
        ChessBoard::GetTileSafe(tilePosition + Vector2i(-1, -1)),
        ChessBoard::GetTileSafe(tilePosition + Vector2i(-1, 0)),
        ChessBoard::GetTileSafe(tilePosition + Vector2i(-1, 1)),
        ChessBoard::GetTileSafe(tilePosition + Vector2i(0, 1))
    };

    if (!isMoved)
    {
        constexpr Vector2i right =  Vector2i::UnitX();
        constexpr Vector2i left = -right;
        const Piece* rightCornerPiece = ChessBoard::GetPieceFromTileSafe(tilePosition + right * 3);
        const Piece* leftCornerPiece = ChessBoard::GetPieceFromTileSafe(tilePosition + left * 4);

        if (rightCornerPiece && !rightCornerPiece->isMoved && rightCornerPiece->pieceType == PieceType::Rook
            && !ChessBoard::IsTherePieceOnTile(tilePosition + right))
        {
            const Vector2i twoRightTiles = tilePosition + right * 2;
            if (!ChessBoard::IsTherePieceOnTile(twoRightTiles))
                result.Add(ChessBoard::GetTileSafe(twoRightTiles));
        }

        if (leftCornerPiece && !leftCornerPiece->isMoved && leftCornerPiece->pieceType == PieceType::Rook
            && !ChessBoard::IsTherePieceOnTile(tilePosition + left) && !ChessBoard::IsTherePieceOnTile(tilePosition + left * 3))
            {
                const Vector2i twoLeftTiles = tilePosition + left * 2;
                if (!ChessBoard::IsTherePieceOnTile(twoLeftTiles))
                    result.Add(ChessBoard::GetTileSafe(twoLeftTiles));
            }
    }

    AddTilesIfNoFreroOnIt(tiles, result);
}

void Piece::UpdatePosition(Tile* newTile)
{
    tile = newTile;
    tilePosition = newTile->tilePosition;
    globalPosition = newTile->position;
    isMoved = true;
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
