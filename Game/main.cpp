#include "App.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    // Instantiate our core App class object
    App app;

    // Execute the main loop and return its final exit code back to Windows OS
    return app.Run();
}