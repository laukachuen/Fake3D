#include "App.h"

App::App()
    : m_wnd(800, 600, L"My Custom Game Framework", L"../../GameFramework/Framework/Shader/BufferShaders.hlsl")
{
    m_screenBuffer = std::make_unique<ScreenBuffer>(800, 600);
}

int App::Run()
{
    while (true)
    {
        // 1. Process all Windows OS messages first
        if (!m_wnd.ProcessMessages())
        {
            // If WM_QUIT is received, exit the loop with standard exit code
            return 0;
        }
        DoFrame();
    }
}

void App::DoFrame()
{
    const float dt = m_timer.Mark();

    Update(dt);
    Render();
}

void App::Update(float dt)
{
    if (m_wnd.kbd.IsKeyPressed('A'))
    {
        m_playerAngle -= ROT_SPEED * dt;
    }
    if (m_wnd.kbd.IsKeyPressed('D'))
    {
        m_playerAngle += ROT_SPEED * dt;
    }

    const float PI2 = 2.0f * 3.14159265f;
    if (m_playerAngle < 0.0f) m_playerAngle += PI2;
    if (m_playerAngle > PI2)  m_playerAngle -= PI2;

    if (m_wnd.kbd.IsKeyPressed('W'))
    {
        m_playerX += std::cos(m_playerAngle) * MOVE_SPEED * dt;
        m_playerY += std::sin(m_playerAngle) * MOVE_SPEED * dt;
    }
    if (m_wnd.kbd.IsKeyPressed('S'))
    {
        m_playerX -= std::cos(m_playerAngle) * MOVE_SPEED * dt;
        m_playerY -= std::sin(m_playerAngle) * MOVE_SPEED * dt;
    }
}

void App::Render()
{
    m_screenBuffer->Clear(0xFF1A1A1A);

    Draw2DMap();

    int pScreenX = static_cast<int>(m_playerX * TILE_SIZE);
    int pScreenY = static_cast<int>(m_playerY * TILE_SIZE);

    for (int sy = pScreenY - 4; sy < pScreenY + 4; ++sy)
    {
        for (int sx = pScreenX - 4; sx < pScreenX + 4; ++sx)
        {
            m_screenBuffer->SetPixel(sx, sy, 0xFFFF0000); 
        }
    }

    int lineEndX = pScreenX + static_cast<int>(std::cos(m_playerAngle) * 20.0f);
    int lineEndY = pScreenY + static_cast<int>(std::sin(m_playerAngle) * 20.0f);

    for (float t = 0.0f; t <= 5.0f; t += 0.2f)
    {
        int lx = pScreenX + static_cast<int>((lineEndX - pScreenX) * t);
        int ly = pScreenY + static_cast<int>((lineEndY - pScreenY) * t);
        m_screenBuffer->SetPixel(lx, ly, 0xFF00FF00); 
    }

    m_wnd.Gfx().BeginFrame(0.0f, 0.0f, 1.0f);
    m_wnd.Gfx().PresentBuffer(*m_screenBuffer);
    m_wnd.Gfx().EndFrame();
}

void App::Draw2DMap()
{
    for (int y = 0; y < MAP_HEIGHT; ++y)
    {
        for (int x = 0; x < MAP_WIDTH; ++x)
        {
            if (m_map[y][x] == 1)
            {
                int startX = x * TILE_SIZE;
                int startY = y * TILE_SIZE;

                for (int py = startY; py < startY + TILE_SIZE - TILE_GAP; ++py)
                {
                    for (int px = startX; px < startX + TILE_SIZE - TILE_GAP; ++px)
                    {
                        m_screenBuffer->SetPixel(px, py, 0xFFFFFFFF);
                    }
                }
            }
        }
    }
}
