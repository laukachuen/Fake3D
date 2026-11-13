#pragma once
#include "GameState.h"
#include <memory>
#include "Image.h"
#include "MazeGenerator.h"

class Keyboard;
class GameStateManager;
class Graphics;

struct Sprite
{
    float x;
    float y;
    int textureID;
};

class GameplayState : public GameState
{
public:
    GameplayState(Context context);
    ~GameplayState();

    void Initialize() override;
    void Update(float dt) override;
    void Draw() override;
    void Finalize() override;

private:
    void Draw2DMap();
    void Draw2DPlayer();
    void Draw2DRays();

    void Draw3DWalls();
    void Draw3DTexWalls();
    void Draw3DFloorAndCeiling();

    void DrawSprites();

    bool IsWall(float x, float y) const;
    bool CanMove(float x, float y) const;

    void UpdateMinimap();
    void DrawMinimap();

private:
    // MAP
    static constexpr int TILE_SIZE = 15;
    static constexpr int TILE_GAP = 1;

    MazeGenereator m_mazeGenerator;

    int m_totalFloorTiles = 0;
    int m_exploredFloorTiles = 0;

    // MINIMAP
    std::vector<std::vector<bool>> m_minimap;
    static constexpr int MINIMAP_WIDTH = 160;
    static constexpr int MINIMAP_HEIGHT = 160;
    static constexpr int PLAYER_MARKER_SIZE = 5;

    // PLAYER
    float m_playerX = 1.5f;
    float m_playerY = 1.5f;

    float m_dirX = 1.0f;
    float m_dirY = 0.0f;

    float m_planeX = 0.0f;
    float m_planeY = 0.66f;

    static constexpr float MOVE_SPEED = 2.5f;
    static constexpr float ROT_SPEED = 2.0f;
    static constexpr float PLAYER_RADIUS = 0.25f;

    // TEXTURES
    std::vector<Image> m_wallTextures;
    Image m_floorTexture;
    Image m_ceilingTexture;

    std::vector<Image> m_spriteTextures;
    std::vector<Sprite> m_sprites;
    std::vector<float> m_zBuffer;

};