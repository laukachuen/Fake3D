#include "TitleState.h"
#include "GameplayState.h"

#include "Keyboard.h"
#include "GameStateManager.h"
#include "Graphics.h"

#include <memory>

TitleState::TitleState(Context context)
    : GameState(context)
{}

TitleState::~TitleState() = default;

void TitleState::Initialize()
{
    m_test.LoadBMP("Assets/Test.bmp");
}

void TitleState::Update(float dt)
{
    if (m_context.kbd.IsKeyPressed(VK_SPACE))
    {
        m_context.gameStateMgr.RequestStateChange(std::make_unique<GameplayState>(m_context));
    }
}

void TitleState::Draw()
{
    m_context.gfx.GetScreenBuffer().Clear(Color::Black());
    m_context.gfx.GetScreenBuffer().DrawImage(m_test, 100, 100);
}

void TitleState::Finalize()
{

}