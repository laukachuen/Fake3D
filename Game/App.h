#pragma once
#include "Window.h"
#include "Timer.h"
#include <memory>
#include "GameStateManager.h"
class App
{
public:
    App();
    ~App() = default;

    App(const App&) = delete;
    App& operator=(const App&) = delete;

    int Run();

private:
    void DoFrame();
    void Update(float dt);
    void Render();

private:
    Window m_wnd;
    Timer m_timer;
    GameStateManager m_gameStateMgr;

};