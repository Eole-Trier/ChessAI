#include "Application.h"

#include "Mountain/window.hpp"
#include "Mountain/file/file_manager.hpp"
#include "Mountain/rendering/draw.hpp"
#include "Mountain/resource/resource_manager.hpp"
#include "Mountain/utils/random.hpp"

Application::Application(const char* windowTitle)
    : Game(windowTitle, Vector2i(880))
{
}

void Application::Initialize()
{
    chessBoard = new ChessBoard();
    Game::Initialize();
}

void Application::LoadResources()
{
    Mountain::FileManager::LoadDirectory("assets");
    Mountain::ResourceManager::LoadAll();
    ChessBoard::LoadResources();
    chessBoard->Initialize();
}

void Application::Shutdown()
{
    Game::Shutdown();
    delete chessBoard;
}

void Application::Update()
{
}

void Application::Render()
{
    Mountain::Draw::Clear(Mountain::Color::Lime());
    chessBoard->Render();
}
