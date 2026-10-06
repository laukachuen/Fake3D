#pragma once
#include "GameState.h"
#include <memory>
#include "Image.h"

class Keyboard;
class GameStateManager;
class Graphics;

class VictoryState : public GameState
{
public:
    VictoryState(Context context);
    ~VictoryState();

    void Initialize() override;
    void Update(float dt) override;
    void Draw() override;
    void Finalize() override;

private:
    Image m_test;

};