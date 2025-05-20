#include "ChessBoard.h"

#include "Mountain/window.hpp"
#include "Mountain/input/input.hpp"
#include "Mountain/rendering/draw.hpp"
#include "Mountain/resource/resource_manager.hpp"

#include "Piece.h"

void ChessBoard::CleanUp()
{
    pieces.Iterate([](Piece** piece){ delete *piece; });
}

void ChessBoard::Render()
{
    Mountain::Draw::Texture(*boardTexture, position, scaling);
    pieces.Iterate([](Piece** piece){ (*piece)->Render(); });
    for (const Tile* tile : availableTiles)
    {
        if (tile)
            Mountain::Draw::Circle(tile->position, Tile::size/3.f, Vector2::One(), Mountain::Color::Black());
    }
}

void ChessBoard::Initialize()
{
    boardSize = static_cast<float>(boardTexture->GetSize().x);
    InitTiles();
    InitPieces();
}

void ChessBoard::InitTiles()
{
    for (uint8_t i = 0; i < 8; i++)
    {
        for (uint8_t j = 0; j < 8; j++)
        {
            tiles[i][j] = new Tile({ i, j });
        }
    }
}

void ChessBoard::InitPieces()
{
    for (uint8_t i = 0; i < 8; i++)
    {
        Piece* wPawn = new Piece(true, PieceType::Pawn, tiles[i][6]);
        Piece* bPawn = new Piece(false, PieceType::Pawn, tiles[i][1]);
        pieces.Add(wPawn);
        pieces.Add(bPawn);
    }

    Piece* wRook1 = new Piece(true, PieceType::Rook, tiles[0][7]);
    Piece* wRook2 = new Piece(true, PieceType::Rook, tiles[7][7]);
    Piece* bRook1 = new Piece(false, PieceType::Rook, tiles[0][0]);
    Piece* bRook2 = new Piece(false, PieceType::Rook, tiles[7][0]);

    Piece* wKnight1 = new Piece(true, PieceType::Knight, tiles[1][7]);
    Piece* wKnight2 = new Piece(true, PieceType::Knight, tiles[6][7]);
    Piece* bKnight1  = new Piece(false, PieceType::Knight, tiles[1][0]);
    Piece* bKnight2  = new Piece(false, PieceType::Knight, tiles[6][0]);

    Piece* wBishop1 = new Piece(true, PieceType::Bishop, tiles[2][7]);
    Piece* wBishop2 = new Piece(true, PieceType::Bishop, tiles[5][7]);
    Piece* bBishop1  = new Piece(false, PieceType::Bishop, tiles[2][0]);
    Piece* bBishop2  = new Piece(false, PieceType::Bishop, tiles[5][0]);

    Piece* wQueen = new Piece(true, PieceType::Queen, tiles[3][7]);
    Piece* wKing = new Piece(true, PieceType::King, tiles[4][7]);
    Piece* bQueen  = new Piece(false, PieceType::Queen, tiles[3][0]);
    Piece* bKing  = new Piece(false, PieceType::King, tiles[4][0]);

    pieces.Add(wRook1);
    pieces.Add(wRook2);
    pieces.Add(bRook1);
    pieces.Add(bRook2);

    pieces.Add(wKnight1);
    pieces.Add(wKnight2);
    pieces.Add(bKnight1);
    pieces.Add(bKnight2);

    pieces.Add(wBishop1);
    pieces.Add(wBishop2);
    pieces.Add(bBishop1);
    pieces.Add(bBishop2);

    pieces.Add(wQueen);
    pieces.Add(wKing);
    pieces.Add(bQueen);
    pieces.Add(bKing);
}

void ChessBoard::Update()
{
    DragAndDrop();
}

void ChessBoard::DragAndDrop()
{
    const Vector2 mousePos = Mountain::Input::GetMousePosition();

    if (Mountain::Input::GetMouseButton(Mountain::MouseButton::Left, Mountain::MouseButtonStatus::Pressed))
    {
        const Vector2i mousePosToTiles = ToTiles(mousePos);
        draggedPiece = GetPieceFromTileSafe(mousePosToTiles);
        selectedPiece = draggedPiece;
        availableTiles.Clear();
        if (selectedPiece)
            selectedPiece->GetAvailableTiles(availableTiles);
    }

    if (Mountain::Input::GetMouseButton(Mountain::MouseButton::Left, Mountain::MouseButtonStatus::Down))
    {
        if (draggedPiece)
            draggedPiece->globalPosition = mousePos;
    }

    if (Mountain::Input::GetMouseButton(Mountain::MouseButton::Left, Mountain::MouseButtonStatus::Release))
    {
        if (draggedPiece)
        {
            const Vector2i mousePosToTiles = ToTiles(mousePos);
            if (IsOnBoard(mousePosToTiles))
            {
                Mountain::List<Tile*> pieceAvailableTiles;
                draggedPiece->GetAvailableTiles(pieceAvailableTiles);
                Tile* droppedTile = tiles[mousePosToTiles.x][mousePosToTiles.y];
                if (pieceAvailableTiles.Contains(droppedTile))
                {
                    HandleEnPassant(mousePosToTiles);
                    HandleCastle(mousePosToTiles);
                    Piece* p = GetPieceFromTile(mousePosToTiles);
                    if (p)
                    {
                        if (p->isWhite != draggedPiece->isWhite)
                        {
                            draggedPiece->UpdatePosition(droppedTile);
                            DeletePiece(p);
                        }
                    }
                    else
                    {
                        draggedPiece->UpdatePosition(droppedTile);
                    }
                    availableTiles.Clear();
                    if (draggedPiece->pieceType == PieceType::Pawn)
                    {
                        const int lastRank = draggedPiece->isWhite ? 0 : 7;
                        if (draggedPiece->tilePosition.y == lastRank)
                            HandlePromotion();
                    }
                }
            }
            draggedPiece->globalPosition = draggedPiece->tile->position;
            draggedPiece = nullptr;
            selectedPiece = nullptr;
        }
    }
}

void ChessBoard::DeletePiece(Piece* piece)
{
    pieces.Remove(piece);
    delete piece;
}

void ChessBoard::HandleEnPassant(const Vector2i mousePosToTiles)
{
    if (draggedPiece->pieceType == PieceType::Pawn && enPassantPiece)
    {
        const Vector2i forward = draggedPiece->isWhite ? -Vector2i::UnitY() : Vector2i::UnitY();
        const Vector2i topLeftTilePos = draggedPiece->tilePosition + (forward - Vector2i::UnitX());
        const Vector2i topRightTilePos = draggedPiece->tilePosition + (forward + Vector2i::UnitX());
        if (mousePosToTiles == topLeftTilePos)
        {
            const Piece* topLeftPiece = GetPieceFromTileSafe(topLeftTilePos);
            if (!topLeftPiece)
            {
                DeletePiece(enPassantPiece);
            }

        }
        else if (mousePosToTiles == topRightTilePos)
        {
            const Piece* topRightPiece = GetPieceFromTileSafe(topRightTilePos);
            if (!topRightPiece)
            {
                DeletePiece(enPassantPiece);
            }
        }
    }
    enPassantPiece = nullptr;
    if (draggedPiece->pieceType == PieceType::Pawn)
    {
        const Vector2i forward = draggedPiece->isWhite ? -Vector2i::UnitY() : Vector2i::UnitY();
        if (draggedPiece->tilePosition + forward *  2 == mousePosToTiles)
            enPassantPiece = draggedPiece;
    }
}

void ChessBoard::HandlePromotion()
{
    draggedPiece->pieceType = PieceType::Queen;
}

void ChessBoard::HandleCastle(const Vector2i newTile)
{
    if (draggedPiece->pieceType != PieceType::King)
        return;

    constexpr Vector2i right =  Vector2i::UnitX();
    constexpr Vector2i left = -right;

    if (draggedPiece->tilePosition + right * 2 == newTile)
    {
        Piece* rightRook = GetPieceFromTileSafe(draggedPiece->tilePosition + right * 3);
        if (rightRook)
            rightRook->UpdatePosition(GetTileSafe(rightRook->tilePosition + left * 2));
    }

    if (draggedPiece->tilePosition + left * 2 == newTile)
    {
        Piece* leftRook = GetPieceFromTileSafe(draggedPiece->tilePosition + left * 4);
        if (leftRook)
            leftRook->UpdatePosition(GetTileSafe(leftRook->tilePosition + right * 3));
    }
}

Vector2 ChessBoard::ToPixels(const Vector2i tilePosition)
{
    return tilePosition * Tile::size + Vector2::One() * Tile::size/2.f;
}

Vector2i ChessBoard::ToTiles(const Vector2 pixelPosition)
{
    return Vector2i((pixelPosition - Vector2::One() * Tile::size/2.f) / Tile::size);
}

Piece* ChessBoard::GetPieceFromTile(const Vector2i& tilePosition)
{
    for (Piece* piece : pieces)
    {
        if (piece->globalPosition == tiles[tilePosition.x][tilePosition.y]->position)
            return piece;
    }
    return nullptr;
}

void ChessBoard::AddTileIfInBoard(Mountain::List<Vector2i>& tilesPos, Mountain::List<Tile*>& result)
{
    for (const Vector2i t : tilesPos)
    {
        if (!IsOnBoard(t))
            continue;
        result.Add(tiles[t.x][t.y]);
    }
}

bool ChessBoard::IsTherePieceOnTile(const Vector2i& tilePosition)
{
    for (const Piece* piece : pieces)
    {
        if (IsOnBoard(tilePosition))
            if (piece->globalPosition == tiles[tilePosition.x][tilePosition.y]->position)
                return true;
    }
    return false;
}

bool ChessBoard::IsOnBoard(const Vector2i& tilePosition)
{
    if (tilePosition.x > 7 || tilePosition.x < 0 || tilePosition.y > 7  || tilePosition.y < 0)
    {
        return false;
    }
    return true;
}

Tile* ChessBoard::GetTileSafe(const Vector2i& tilePosition)
{
    if (IsOnBoard(tilePosition))
    {
        return tiles[tilePosition.x][tilePosition.y];
    }
    return nullptr;
}

Piece* ChessBoard::GetPieceFromTileSafe(const Vector2i& tilePosition)
{
    if (IsOnBoard(tilePosition))
        return GetPieceFromTile(tilePosition);
    return nullptr;
}

void ChessBoard::LoadResources()
{
    boardTexture = Mountain::ResourceManager::Get<Mountain::Texture>("assets/images/board.png");
    Piece::LoadResources();
}
