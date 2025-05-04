#pragma once
#include "ChessBoard.h"
#include "Mountain/game.hpp"
#include "Mountain/resource/texture.hpp"

class Application : public Mountain::Game
{
public:
    explicit Application(const char* windowTitle);
    void Initialize() override;
    void LoadResources() override;
    void Shutdown() override;
    void Update() override;
    void Render() override;

    ChessBoard* chessBoard = nullptr;
};
