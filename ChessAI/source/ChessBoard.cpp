#include "ChessBoard.h"

#include "Mountain/window.hpp"
#include "Mountain/rendering/draw.hpp"
#include "Mountain/resource/resource_manager.hpp"

ChessBoard::ChessBoard()
{
    boardSize = 0;
    position = {0, 0};
    scaling = {1, 1};
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
    for (uint8_t i = 0; i < 8; i++)
    {
        for (uint8_t j = 0; j < 8; j++)
        {
            const Tile tile = Tile(Vector2(tileSize * static_cast<float>(i), tileSize * static_cast<float>(j)), tileSize);
            tiles[i][j] = tile;
        }
    }
}

void ChessBoard::InitPieces()
{
    for (uint8_t i = 0; i < 8; i++)
    {
        Piece* wPawn = new Piece(true, PieceType::Pawn, Vector2i(i, 6));
        Piece* bPawn = new Piece(false, PieceType::Pawn, Vector2i(i, 1));
        pieces.Add(wPawn);
        pieces.Add(bPawn);
    }

    Piece* wRook1 = new Piece(true, PieceType::Rook, Vector2i(0, 7));
    Piece* wRook2 = new Piece(true, PieceType::Rook, Vector2i(7, 7));
    Piece* bRook1 = new Piece(false, PieceType::Rook, Vector2i(0, 0));
    Piece* bRook2 = new Piece(false, PieceType::Rook, Vector2i(7, 0));

    Piece* wKnight1 = new Piece(true, PieceType::Knight, Vector2i(1, 7));
    Piece* wKnight2 = new Piece(true, PieceType::Knight, Vector2i(6, 7));
    Piece* bKnight1  = new Piece(false, PieceType::Knight, Vector2i(1, 0));
    Piece* bKnight2  = new Piece(false, PieceType::Knight, Vector2i(6, 0));

    Piece* wBishop1 = new Piece(true, PieceType::Bishop, Vector2i(2, 7));
    Piece* wBishop2 = new Piece(true, PieceType::Bishop, Vector2i(5, 7));
    Piece* bBishop1  = new Piece(false, PieceType::Bishop, Vector2i(2, 0));
    Piece* bBishop2  = new Piece(false, PieceType::Bishop, Vector2i(5, 0));

    Piece* wQueen = new Piece(true, PieceType::Queen, Vector2i(3, 7));
    Piece* wKing = new Piece(true, PieceType::King, Vector2i(4, 7));
    Piece* bQueen  = new Piece(false, PieceType::Queen, Vector2i(3, 0));
    Piece* bKing  = new Piece(false, PieceType::King, Vector2i(4, 0));

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

Vector2 ChessBoard::ToPixels(const Vector2i tilePosition)
{
    return tiles[tilePosition.x][tilePosition.y].position;
}

Vector2i ChessBoard::ToTiles(const Vector2 pixelPosition)
{
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
    return Vector2i(-1, -1);
}

void ChessBoard::LoadResources()
{
    boardTexture = Mountain::ResourceManager::Get<Mountain::Texture>("assets/images/board.png");
    Piece::LoadResources();
}
