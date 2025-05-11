#pragma once
#include "Piece.h"
#include "Tile.h"
#include "Mountain/audio/audio.hpp"

class ChessBoard
{
public:
    float boardSize;
    Vector2 position;
    Vector2 scaling;
    static inline Mountain::List<Piece*> pieces;
    static inline std::array<std::array<Tile, 8>, 8> tiles;
    Piece* draggedPiece;

public:
    ChessBoard();
    ~ChessBoard();
    DEFAULT_COPY_MOVE_OPERATIONS(ChessBoard)

    void Render();
    void Initialize();
    void InitTiles() const;
    void InitPieces();
    void Update();
    void DragAndDrop();
    Piece* GetPieceFromTile(const Vector2i& tilePosition);
    void DeletePiece(Piece* piece);
public:
    static void LoadResources();
    [[nodiscard]] static Vector2 ToPixels(Vector2i tilePosition);
    [[nodiscard]] static Vector2i ToTiles(Vector2 pixelPosition);
    static void AddTileIfInBoard(Mountain::List<Vector2i>& tilesPos, Mountain::List<Tile>& result);
    static bool IsTherePieceOnTile(const Vector2i& tilePosition);
    static bool IsOnBoard(const Vector2i& tilePosition);

public:
    static inline Mountain::Pointer<Mountain::Texture> boardTexture;
};

