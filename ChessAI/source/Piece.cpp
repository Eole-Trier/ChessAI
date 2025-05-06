#include "Piece.h"

#include "ChessBoard.h"
#include "Mountain/window.hpp"
#include "Mountain/input/input.hpp"
#include "Mountain/rendering/draw.hpp"
#include "Mountain/resource/resource_manager.hpp"

Piece::Piece(const bool isWhite, const PieceType pieceType, const Vector2i position)
    : isWhite(isWhite), pieceType(pieceType), globalPosition(ChessBoard::ToPixels(position))
{
    scaling = Vector2(0.363f);
}

void Piece::Render()
{
    const int index = isWhite ? static_cast<int>(pieceType) : static_cast<int>(pieceType) + 6;

    Mountain::Draw::Texture(*piecesTextures[index], globalPosition, scaling);
}

void Piece::Move(const Vector2i newPosition)
{
    globalPosition = ChessBoard::ToPixels(newPosition);
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
