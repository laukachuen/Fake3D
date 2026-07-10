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

}

void App::Render()
{
    m_screenBuffer->Clear(0xFF1A1A1A);

    Draw2DMap();

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
