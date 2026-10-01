#define NOMINMAX
#include "App.h"
#include "TitleState.h"

App::App()
    : m_wnd(800, 600, L"My Custom Game Framework", L"../../GameFramework/Framework/Shader/BufferShaders.hlsl")
    , m_gameStateMgr(m_wnd.kbd, m_wnd.Gfx())
{
    m_gameStateMgr.RequestStateChange(std::make_unique<TitleState>(m_gameStateMgr.GetContext()));
}

int App::Run()
{
    while (true)
    {
        if (!m_wnd.ProcessMessages())
        {
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
    m_gameStateMgr.Update(dt);
}

void App::Render()
{
    m_wnd.Gfx().BeginFrame(0.0f, 0.0f, 1.0f);
    m_gameStateMgr.Draw();
    m_wnd.Gfx().PresentBuffer();
    m_wnd.Gfx().EndFrame();
}