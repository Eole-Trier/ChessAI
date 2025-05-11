﻿#include "ChessBoard.h"

#include "Mountain/window.hpp"
#include "Mountain/input/input.hpp"
#include "Mountain/rendering/draw.hpp"
#include "Mountain/resource/resource_manager.hpp"

ChessBoard::ChessBoard()
{
    boardSize = 0;
    position = {0, 0};
    scaling = {1, 1};
    draggedPiece = nullptr;
}

ChessBoard::~ChessBoard()
{
    pieces.Iterate([](Piece** piece){ delete *piece; });
}

void ChessBoard::Render()
{
    Mountain::Draw::Texture(*boardTexture, position, scaling);
    pieces.Iterate([](Piece** piece){ (*piece)->Render(); });
}

void ChessBoard::Initialize()
{
    boardSize = static_cast<float>(boardTexture->GetSize().x);
    InitTiles();
    InitPieces();
}

void ChessBoard::InitTiles() const
{
    const float tileSize = boardSize/8.f;
    const float halfTileSize = tileSize / 2.f;

    for (uint8_t i = 0; i < 8; i++)
    {
        for (uint8_t j = 0; j < 8; j++)
        {
            const Tile tile = Tile(Vector2(tileSize * static_cast<float>(i) + halfTileSize, tileSize * static_cast<float>(j) + halfTileSize));
            tiles[i][j] = tile;
        }
    }
}

void ChessBoard::InitPieces()
{
    for (uint8_t i = 0; i < 8; i++)
    {
        Piece* wPawn = new Piece(true, PieceType::Pawn, &tiles[i][6]);
        Piece* bPawn = new Piece(false, PieceType::Pawn, &tiles[i][1]);
        pieces.Add(wPawn);
        pieces.Add(bPawn);
    }

    Piece* wRook1 = new Piece(true, PieceType::Rook, &tiles[0][7]);
    Piece* wRook2 = new Piece(true, PieceType::Rook, &tiles[7][7]);
    Piece* bRook1 = new Piece(false, PieceType::Rook, &tiles[0][0]);
    Piece* bRook2 = new Piece(false, PieceType::Rook, &tiles[7][0]);

    Piece* wKnight1 = new Piece(true, PieceType::Knight, &tiles[1][7]);
    Piece* wKnight2 = new Piece(true, PieceType::Knight, &tiles[6][7]);
    Piece* bKnight1  = new Piece(false, PieceType::Knight, &tiles[1][0]);
    Piece* bKnight2  = new Piece(false, PieceType::Knight, &tiles[6][0]);

    Piece* wBishop1 = new Piece(true, PieceType::Bishop, &tiles[2][7]);
    Piece* wBishop2 = new Piece(true, PieceType::Bishop, &tiles[5][7]);
    Piece* bBishop1  = new Piece(false, PieceType::Bishop, &tiles[2][0]);
    Piece* bBishop2  = new Piece(false, PieceType::Bishop, &tiles[5][0]);

    Piece* wQueen = new Piece(true, PieceType::Queen, &tiles[3][7]);
    Piece* wKing = new Piece(true, PieceType::King, &tiles[4][7]);
    Piece* bQueen  = new Piece(false, PieceType::Queen, &tiles[3][0]);
    Piece* bKing  = new Piece(false, PieceType::King, &tiles[4][0]);

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
        if (mousePosToTiles != Vector2i(-1))
        {
            draggedPiece = GetPieceFromTile(mousePosToTiles);
        }
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
            if (mousePosToTiles != Vector2i(-1))
            {
                Mountain::List<Tile> availableTiles;
                draggedPiece->GetAvailableTiles(availableTiles);
                Tile& element = tiles[mousePosToTiles.x][mousePosToTiles.y];
                if (availableTiles.Contains(element))
                {
                    Piece* p = GetPieceFromTile(mousePosToTiles);
                    if (p)
                    {
                        if (p->isWhite != draggedPiece->isWhite)
                        {
                            draggedPiece->tile = &element;
                            draggedPiece->tilePos = mousePosToTiles;
                            DeletePiece(p);
                        }
                    }
                    else
                    {
                        draggedPiece->tile = &element;
                        draggedPiece->tilePos = mousePosToTiles;
                    }
                }
                draggedPiece->isMoved = true;
            }
            draggedPiece->globalPosition = draggedPiece->tile->position;
            draggedPiece = nullptr;
        }
    }
}

Piece* ChessBoard::GetPieceFromTile(const Vector2i& tilePosition)
{
    for (Piece* piece : pieces)
    {
        if (piece->globalPosition == tiles[tilePosition.x][tilePosition.y].position)
            return piece;
    }
    return nullptr;
}

void ChessBoard::DeletePiece(Piece* piece)
{
    pieces.Remove(piece);
    delete piece;
}

Vector2 ChessBoard::ToPixels(const Vector2i tilePosition)
{
    return tiles[tilePosition.x][tilePosition.y].position;
}

Vector2i ChessBoard::ToTiles(const Vector2 pixelPosition)
{
    // TODO optimize tah refactor
    const float halfSize = tiles[0][0].size/2.f;
    for (uint8_t i = 0; i < 8; i++)
    {
        for (uint8_t j = 0; j < 8; j++)
        {
            if (pixelPosition.x >= tiles[i][j].position.x - halfSize && pixelPosition.x <= tiles[i][j].position.x + halfSize &&
                pixelPosition.y >= tiles[i][j].position.y - halfSize && pixelPosition.y <= tiles[i][j].position.y + halfSize)
            {
                return Vector2i(i, j);
            }
        }
    }
    return Vector2i(-1);
}

void ChessBoard::AddTileIfInBoard(Mountain::List<Vector2i>& tilesPos, Mountain::List<Tile>& result)
{
    for (const Vector2i t : tilesPos)
    {
        if (t.x > 7 || t.x < 0 || t.y > 7  || t.y < 0)
        {
            continue;
        }
        result.Add(ChessBoard::tiles[t.x][t.y]);
    }
}

bool ChessBoard::IsTherePieceOnTile(const Vector2i& tilePosition)
{
    for (const Piece* piece : pieces)
    {
        if (IsOnBoard(tilePosition))
            if (piece->globalPosition == tiles[tilePosition.x][tilePosition.y].position)
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


void ChessBoard::LoadResources()
{
    boardTexture = Mountain::ResourceManager::Get<Mountain::Texture>("assets/images/board.png");
    Piece::LoadResources();
}
