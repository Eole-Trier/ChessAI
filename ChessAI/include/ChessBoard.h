#pragma once
#include "Tile.h"
#include "Mountain/audio/audio.hpp"
#include "Mountain/resource/texture.hpp"
#include "Mountain/utils/pointer.hpp"

class Piece;

class ChessBoard final
{
    STATIC_CLASS(ChessBoard)

public:
    static inline float boardSize;
    static inline Vector2 position;
    static inline Vector2 scaling = Vector2::One();
    static inline Mountain::List<Piece*> pieces;
    static inline std::array<std::array<Tile*, 8>, 8> tiles;
    static inline Mountain::List<Tile*> availableTiles;
    static inline Piece* draggedPiece;
    static inline Piece* selectedPiece;
    static inline Piece* enPassantPiece;

public:
    static void CleanUp();
    static void Render();
    static void Initialize();
    static void InitTiles();
    static void InitPieces();
    static void Update();
    static void DragAndDrop();
    static void DeletePiece(Piece* piece);
    static void HandleEnPassant(Vector2i mousePosToTiles);
    static void HandlePromotion();
    static void HandleCastle(Vector2i newTile);
    static bool IsTileControlled(Vector2i newTile, bool isWhite);

public:
    static void LoadResources();
    [[nodiscard]] static Vector2 ToPixels(Vector2i tilePosition);
    [[nodiscard]] static Vector2i ToTiles(Vector2 pixelPosition);
    static Piece* GetPieceFromTile(const Vector2i& tilePosition);
    static void AddTileIfInBoard(Mountain::List<Vector2i>& tilesPos, Mountain::List<Tile*>& result);
    static bool IsTherePieceOnTile(const Vector2i& tilePosition);
    static bool IsOnBoard(const Vector2i& tilePosition);
    static Tile* GetTileSafe(const Vector2i& tilePosition);
    static Piece* GetPieceFromTileSafe(const Vector2i& tilePosition);

public:
    static inline Mountain::Pointer<Mountain::Texture> boardTexture;
};

