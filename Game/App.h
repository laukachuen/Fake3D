#pragma once
#include "Window.h"
#include "Timer.h"
#include "ScreenBuffer.h"
#include <memory>
#include <cmath>

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
    void DoFrame();
    void Update(float dt);
    void Render();

    void Draw2DMap();
    void Draw2DRays();

private:
    Window m_wnd;
    Timer m_timer;
    std::unique_ptr<ScreenBuffer> m_screenBuffer;

    // MAP
    static constexpr int MAP_WIDTH = 8;
    static constexpr int MAP_HEIGHT = 8;
    const int m_map[MAP_HEIGHT][MAP_WIDTH] = {
        {1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,1},
        {1,0,1,0,0,1,0,1},
        {1,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,1},
        {1,0,1,0,0,1,0,1},
        {1,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1}
    };

    static constexpr int TILE_SIZE = 64;
    static constexpr int TILE_GAP = 1;

    // PLAYER
    float m_playerX = 3.5f;
    float m_playerY = 3.5f;
    float m_playerAngle = 0.0f;

    static constexpr float MOVE_SPEED = 2.0f;
    static constexpr float ROT_SPEED = 1.5f;

};