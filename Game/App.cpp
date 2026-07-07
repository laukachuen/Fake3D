#include "App.h"

App::App()
    : m_wnd(800, 600, L"My Custom Game Framework")
{
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

        // 2. Get the exact elapsed time since the last frame (deltaTime)
        float dt = m_timer.Mark();

        // 3. Update game simulation and then render graphics
        Update(dt);
        Render();
    }
}

void App::Update(float dt)
{
    // ====== GAME PLAY LOGIC ======
    // Everything that moves or changes must multiply by 'dt'

    // Example: Test input using our window's input components
    if (m_wnd.kbd.IsKeyPressed('W'))
    {
        // Player moves forward based on time (dt), not frame rate!
    }
}

void App::Render()
{
    // ====== GRAPHICS RENDERING ======
    // Clear screen with a dark blue background color
    m_wnd.Gfx().BeginFrame(0.0f, 0.0f, 1.0f);

    // TODO: Issue your DirectX 11 draw calls here

    // Swap buffer to show the frame
    m_wnd.Gfx().EndFrame();
}