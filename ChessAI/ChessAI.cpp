
#include "Application.h"

int main(void)
{
    Application* app = new Application("Chess");

    app->Initialize();
    app->MainLoop();
    app->Shutdown();

    delete app;
}
