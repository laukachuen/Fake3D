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
    m_test.LoadBMP("Assets/Sprites/Test.bmp");
    m_font = std::make_unique<FreeTypeFont>("Assets/Fonts/kenney_kenney-fonts/Fonts/Kenney Mini Square Mono.ttf", 64);
    m_text = std::make_unique<Text>(*m_font, "Hello World", 200, 200, Color::Blue());
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
    m_context.gfx.GetScreenBuffer().DrawImage(m_test, 100, 100);
    m_text->Draw(m_context.gfx.GetScreenBuffer());
}

void TitleState::Finalize()
{

}