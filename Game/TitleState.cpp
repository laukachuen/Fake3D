#include "TitleState.h"
#include "GameplayState.h"
#include "VictoryState.h"
#include "FreeTypeFont.h"
#include "Text.h"

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
    m_font = std::make_unique<FreeTypeFont>("Assets/Fonts/kenney_kenney-fonts/Fonts/Kenney Mini Square Mono.ttf", 64);
    m_titleText = std::make_unique<Text>(*m_font, "Fake3D", 0, 0, Color::White());

    auto [textWidth, textHeight] = m_titleText->GetSize();

    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;

    int centerX = (SCREEN_WIDTH - textWidth) / 2;
    int centerY = (SCREEN_HEIGHT - textHeight) / 2;

    m_titleText->SetPosition(centerX, centerY - 100);
}

void TitleState::Update(float dt)
{
    if (m_context.kbd.IsKeyTriggered(VK_SPACE))
    {
        m_context.gameStateMgr.RequestStateChange(std::make_unique<GameplayState>(m_context));
    }
    if (m_context.kbd.IsKeyTriggered(VK_ESCAPE))
    {
        m_context.gameStateMgr.RequestStateChange(std::make_unique<VictoryState>(m_context));
    }
}

void TitleState::Draw()
{
    m_context.gfx.GetScreenBuffer().Clear(Color::Black());
    m_titleText->Draw(m_context.gfx.GetScreenBuffer());
}

void TitleState::Finalize()
{

}