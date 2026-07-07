#pragma once
#include "Window.h"
#include "Timer.h"

class App
{
public:
    App();
    ~App() = default;

    // Prevent copying
    App(const App&) = delete;
    App& operator=(const App&) = delete;

    // The actual infinite loop that runs the game
    int Run();

private:
    // Core game loop stages
    void Update(float dt);
    void Render();

private:
    Window m_wnd;
    Timer m_timer;
};