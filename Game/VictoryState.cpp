#include "VictoryState.h"
#include "TitleState.h"
#include "GameplayState.h"

#include "Keyboard.h"
#include "GameStateManager.h"
#include "Graphics.h"

#include <memory>

VictoryState::VictoryState(Context context)
    : GameState(context)
{
}

VictoryState::~VictoryState() = default;

void VictoryState::Initialize()
{
    m_test.LoadBMP("Assets/Test.bmp");
}

void VictoryState::Update(float dt)
{
    if (m_context.kbd.IsKeyTriggered(VK_SPACE))
    {
        m_context.gameStateMgr.RequestStateChange(std::make_unique<TitleState>(m_context));
    }
    if (m_context.kbd.IsKeyTriggered(VK_ESCAPE))
    {
        m_context.gameStateMgr.RequestStateChange(std::make_unique<GameplayState>(m_context));
    }
}

void VictoryState::Draw()
{
    m_context.gfx.GetScreenBuffer().Clear(Color::Black());
    m_context.gfx.GetScreenBuffer().DrawImage(m_test, 300, 300);
}

void VictoryState::Finalize()
{

}