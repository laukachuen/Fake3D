#define NOMINMAX
#include "GameplayState.h"
#include "TitleState.h"
#include "Color.h"

#include "Keyboard.h"
#include "GameStateManager.h"
#include "Graphics.h"

#include <memory>
#include <algorithm>
#include <string>

GameplayState::GameplayState(Context context)
    : GameState(context)
{
}

GameplayState::~GameplayState() = default;

void GameplayState::Initialize()
{
    // Map Generation
    m_mazeGenerator.Generate(17, 17);

    // Wall Textures
    std::vector<std::string> wallTexturePaths =
    {
        "Assets/Test6.bmp",
        "Assets/Test2.bmp",
        "Assets/Test3.bmp",
        "Assets/Test4.bmp",
        "Assets/Test5.bmp"
    };

    m_wallTextures.reserve(wallTexturePaths.size());

    for (const auto& path : wallTexturePaths)
    {
        m_wallTextures.emplace_back();
        m_wallTextures.back().LoadBMP(path);
    }

    // Floor Texture
    m_floorTexture.LoadBMP("Assets/Test.bmp");

    // Ceiling Texture
    m_ceilingTexture.LoadBMP("Assets/Test.bmp");

    // Sprite Textures
    std::vector<std::string> spriteTexturesPaths =
    {
        "Assets/Sprite_01.bmp",
        "Assets/Sprite_02.bmp",
        "Assets/Sprite_03.bmp"
    };

    m_spriteTextures.reserve(spriteTexturesPaths.size());

    for (const auto& path : spriteTexturesPaths)
    {
        m_spriteTextures.emplace_back();
        m_spriteTextures.back().LoadBMP(path);
    }

    // Sprite 
    m_sprites =
    {
        {2.5f, 2.5f, 0},
        {4.5f, 2.5f, 1},
        {5.5f, 4.5f, 2}
    };

    m_zBuffer.resize(800, 0.0f);
}

void GameplayState::Update(float dt)
{
    if (m_context.kbd.IsKeyHeld('A'))
    {
        float oldDirX = m_dirX;
        m_dirX = m_dirX * std::cos(-ROT_SPEED * dt) - m_dirY * std::sin(-ROT_SPEED * dt);
        m_dirY = oldDirX * std::sin(-ROT_SPEED * dt) + m_dirY * std::cos(-ROT_SPEED * dt);

        float oldPlaneX = m_planeX;
        m_planeX = m_planeX * std::cos(-ROT_SPEED * dt) - m_planeY * std::sin(-ROT_SPEED * dt);
        m_planeY = oldPlaneX * std::sin(-ROT_SPEED * dt) + m_planeY * std::cos(-ROT_SPEED * dt);
    }
    if (m_context.kbd.IsKeyHeld('D'))
    {
        float oldDirX = m_dirX;
        m_dirX = m_dirX * std::cos(ROT_SPEED * dt) - m_dirY * std::sin(ROT_SPEED * dt);
        m_dirY = oldDirX * std::sin(ROT_SPEED * dt) + m_dirY * std::cos(ROT_SPEED * dt);

        float oldPlaneX = m_planeX;
        m_planeX = m_planeX * std::cos(ROT_SPEED * dt) - m_planeY * std::sin(ROT_SPEED * dt);
        m_planeY = oldPlaneX * std::sin(ROT_SPEED * dt) + m_planeY * std::cos(ROT_SPEED * dt);
    }

    float moveX = 0.0f;
    float moveY = 0.0f;

    if (m_context.kbd.IsKeyHeld('W'))
    {
        moveX += m_dirX * MOVE_SPEED * dt;
        moveY += m_dirY * MOVE_SPEED * dt;
    }
    if (m_context.kbd.IsKeyHeld('S'))
    {
        moveX -= m_dirX * MOVE_SPEED * dt;
        moveY -= m_dirY * MOVE_SPEED * dt;
    }

    float newX = m_playerX + moveX;
    float newY = m_playerY + moveY;

    if (CanMove(newX, m_playerY))
    {
        m_playerX = newX;
    }

    if (CanMove(m_playerX, newY))
    {
        m_playerY = newY;
    }

    if (m_context.kbd.IsKeyTriggered(VK_ESCAPE))
    {
        m_context.gameStateMgr.RequestStateChange(std::make_unique<TitleState>(m_context));
    }
}

void GameplayState::Draw()
{
    m_context.gfx.GetScreenBuffer().Clear(Color::Black());

    //Draw3DWalls();
    //Draw3DFloorAndCeiling();
    //Draw3DTexWalls();
    //DrawSprites();

    Draw2DMap();
    Draw2DRays();
    Draw2DPlayer();
}

void GameplayState::Finalize()
{

}

void GameplayState::Draw2DMap()
{
    const auto& map = m_mazeGenerator.GetMap();
    int mapWidth = m_mazeGenerator.GetWidth();
    int mapHeight = m_mazeGenerator.GetHeight();

    for (int y = 0; y < mapHeight; ++y)
    {
        for (int x = 0; x < mapWidth; ++x)
        {
            if (map[y][x] == 1)
            {
                int startX = x * TILE_SIZE;
                int startY = y * TILE_SIZE;

                for (int py = startY; py < startY + TILE_SIZE - TILE_GAP; ++py)
                {
                    for (int px = startX; px < startX + TILE_SIZE - TILE_GAP; ++px)
                    {
                        m_context.gfx.GetScreenBuffer().SetPixel(px, py, Color::White());
                    }
                }
            }
        }
    }
}

void GameplayState::Draw2DPlayer()
{
    int pScreenX = static_cast<int>(m_playerX * TILE_SIZE);
    int pScreenY = static_cast<int>(m_playerY * TILE_SIZE);

    for (int sy = pScreenY - 4; sy < pScreenY + 4; ++sy)
    {
        for (int sx = pScreenX - 4; sx < pScreenX + 4; ++sx)
        {
            m_context.gfx.GetScreenBuffer().SetPixel(sx, sy, Color::Yellow());
        }
    }

    int lineEndX = pScreenX + static_cast<int>(m_dirX * 20.0f);
    int lineEndY = pScreenY + static_cast<int>(m_dirY * 20.0f);

    for (float t = 0.0f; t <= 5.0f; t += 0.1f)
    {
        int lx = pScreenX + static_cast<int>((lineEndX - pScreenX) * t);
        int ly = pScreenY + static_cast<int>((lineEndY - pScreenY) * t);
        m_context.gfx.GetScreenBuffer().SetPixel(lx, ly, Color::Red());
    }
}

void GameplayState::Draw2DRays()
{
    const int NUM_RAYS = 60;

    const auto& map = m_mazeGenerator.GetMap();
    int mapWidth = m_mazeGenerator.GetWidth();
    int mapHeight = m_mazeGenerator.GetHeight();

    for (int i = 0; i < NUM_RAYS; ++i)
    {
        float cameraX = 2.0f * static_cast<float>(i) / static_cast<float>(NUM_RAYS) - 1.0f;

        float rayDirX = m_dirX + m_planeX * cameraX;
        float rayDirY = m_dirY + m_planeY * cameraX;

        int mapX = static_cast<int>(m_playerX);
        int mapY = static_cast<int>(m_playerY);

        float deltaDistX = (rayDirX == 0) ? 1e30f : std::abs(1.0f / rayDirX);
        float deltaDistY = (rayDirY == 0) ? 1e30f : std::abs(1.0f / rayDirY);

        float sideDistX;
        float sideDistY;

        int stepX;
        int stepY;

        if (rayDirX < 0)
        {
            stepX = -1;
            sideDistX = (m_playerX - static_cast<float>(mapX)) * deltaDistX;
        }
        else
        {
            stepX = 1;
            sideDistX = (static_cast<float>(mapX + 1) - m_playerX) * deltaDistX;
        }

        if (rayDirY < 0)
        {
            stepY = -1;
            sideDistY = (m_playerY - static_cast<float>(mapY)) * deltaDistY;
        }
        else
        {
            stepY = 1;
            sideDistY = (static_cast<float>(mapY + 1) - m_playerY) * deltaDistY;
        }

        bool hit = false;
        int side = 0;

        while (!hit)
        {
            if (sideDistX < sideDistY)
            {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0;
            }
            else
            {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1;
            }

            if (mapX < 0 || mapX >= mapWidth || mapY < 0 || mapY >= mapHeight)
            {
                break;
            }

            if (map[mapY][mapX] > 0)
            {
                hit = true;
            }
        }

        if (hit)
        {
            float perpWallDist;
            if (side == 0) perpWallDist = (sideDistX - deltaDistX);
            else           perpWallDist = (sideDistY - deltaDistY);

            int startScreenX = static_cast<int>(m_playerX * TILE_SIZE);
            int startScreenY = static_cast<int>(m_playerY * TILE_SIZE);
            int endScreenX = static_cast<int>((m_playerX + rayDirX * perpWallDist) * TILE_SIZE);
            int endScreenY = static_cast<int>((m_playerY + rayDirY * perpWallDist) * TILE_SIZE);


            for (float t = 0.0f; t <= 1.0f; t += 0.01f)
            {
                int lx = startScreenX + static_cast<int>(static_cast<float>(endScreenX - startScreenX) * t);
                int ly = startScreenY + static_cast<int>(static_cast<float>(endScreenY - startScreenY) * t);

                m_context.gfx.GetScreenBuffer().SetPixel(lx, ly, Color::Cyan());
            }
        }
    }
}

void GameplayState::Draw3DWalls()
{
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;

    const auto& map = m_mazeGenerator.GetMap();
    int mapWidth = m_mazeGenerator.GetWidth();
    int mapHeight = m_mazeGenerator.GetHeight();

    for (int x = 0; x < SCREEN_WIDTH; ++x)
    {
        float cameraX = 2.0f * static_cast<float>(x) / static_cast<float>(SCREEN_WIDTH) - 1.0f;

        float rayDirX = m_dirX + m_planeX * cameraX;
        float rayDirY = m_dirY + m_planeY * cameraX;

        int mapX = static_cast<int>(m_playerX);
        int mapY = static_cast<int>(m_playerY);

        float deltaDistX = (rayDirX == 0) ? 1e30f : std::abs(1.0f / rayDirX);
        float deltaDistY = (rayDirY == 0) ? 1e30f : std::abs(1.0f / rayDirY);

        float sideDistX;
        float sideDistY;
        int stepX;
        int stepY;

        if (rayDirX < 0)
        {
            stepX = -1;
            sideDistX = (m_playerX - static_cast<float>(mapX)) * deltaDistX;
        }
        else
        {
            stepX = 1;
            sideDistX = (static_cast<float>(mapX + 1) - m_playerX) * deltaDistX;
        }

        if (rayDirY < 0)
        {
            stepY = -1;
            sideDistY = (m_playerY - static_cast<float>(mapY)) * deltaDistY;
        }
        else
        {
            stepY = 1;
            sideDistY = (static_cast<float>(mapY + 1) - m_playerY) * deltaDistY;
        }

        bool hit = false;
        int side = 0;
        while (!hit)
        {
            if (sideDistX < sideDistY)
            {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0;
            }
            else
            {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1;
            }

            if (mapX < 0 || mapX >= mapWidth || mapY < 0 || mapY >= mapHeight) break;
            if (map[mapY][mapX] > 0) hit = true;
        }

        if (hit)
        {
            float perpWallDist;
            if (side == 0) perpWallDist = (sideDistX - deltaDistX);
            else           perpWallDist = (sideDistY - deltaDistY);

            int lineHeight = static_cast<int>(640.0f / perpWallDist);

            int drawStart = (SCREEN_HEIGHT - lineHeight) / 2;
            int drawEnd = (SCREEN_HEIGHT + lineHeight) / 2;

            if (drawStart < 0) drawStart = 0;
            if (drawEnd >= SCREEN_HEIGHT) drawEnd = SCREEN_HEIGHT - 1;

            Color wallColor = (side == 0) ? Color::White() : Color::Gray();

            for (int y = 0; y < SCREEN_HEIGHT; ++y)
            {
                if (y < drawStart)
                {
                    m_context.gfx.GetScreenBuffer().SetPixel(x, y, Color::Blue());
                }
                else if (y >= drawStart && y <= drawEnd)
                {
                    m_context.gfx.GetScreenBuffer().SetPixel(x, y, wallColor);
                }
                else
                {
                    m_context.gfx.GetScreenBuffer().SetPixel(x, y, Color::Green());
                }
            }
        }
    }
}

void GameplayState::Draw3DTexWalls()
{
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;

    const auto& map = m_mazeGenerator.GetMap();
    int mapWidth = m_mazeGenerator.GetWidth();
    int mapHeight = m_mazeGenerator.GetHeight();

    for (int x = 0; x < SCREEN_WIDTH; ++x)
    {
        float cameraX = 2.0f * static_cast<float>(x) / static_cast<float>(SCREEN_WIDTH) - 1.0f;

        float rayDirX = m_dirX + m_planeX * cameraX;
        float rayDirY = m_dirY + m_planeY * cameraX;

        int mapX = static_cast<int>(m_playerX);
        int mapY = static_cast<int>(m_playerY);

        float deltaDistX = (rayDirX == 0) ? 1e30f : std::abs(1.0f / rayDirX);
        float deltaDistY = (rayDirY == 0) ? 1e30f : std::abs(1.0f / rayDirY);

        float sideDistX;
        float sideDistY;
        int stepX;
        int stepY;

        if (rayDirX < 0)
        {
            stepX = -1;
            sideDistX = (m_playerX - static_cast<float>(mapX)) * deltaDistX;
        }
        else
        {
            stepX = 1;
            sideDistX = (static_cast<float>(mapX + 1) - m_playerX) * deltaDistX;
        }

        if (rayDirY < 0)
        {
            stepY = -1;
            sideDistY = (m_playerY - static_cast<float>(mapY)) * deltaDistY;
        }
        else
        {
            stepY = 1;
            sideDistY = (static_cast<float>(mapY + 1) - m_playerY) * deltaDistY;
        }

        bool hit = false;
        int side = 0;
        int wallType = 0;

        while (!hit)
        {
            if (sideDistX < sideDistY)
            {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0;
            }
            else
            {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1;
            }

            if (mapX < 0 || mapX >= mapWidth || mapY < 0 || mapY >= mapHeight) break;
            if (map[mapY][mapX] > 0)
            {
                hit = true;
                wallType = map[mapY][mapX];
            }
        }

        if (hit)
        {
            int texIndex = std::clamp(wallType - 1, 0, static_cast<int>(m_wallTextures.size()) - 1);
            const Image& currentTex = m_wallTextures[texIndex];

            float perpWallDist;
            if (side == 0) perpWallDist = (sideDistX - deltaDistX);
            else           perpWallDist = (sideDistY - deltaDistY);

            int lineHeight = static_cast<int>(640.0f / perpWallDist);
            m_zBuffer[x] = perpWallDist;

            float hitCoord;
            if (side == 0) hitCoord = m_playerY + perpWallDist * rayDirY;
            else           hitCoord = m_playerX + perpWallDist * rayDirX;
            float wallU = hitCoord - std::floor(hitCoord);

            int texX = static_cast<int>(wallU * currentTex.GetWidth());
            if ((side == 0 && rayDirX > 0) || (side == 1 && rayDirY < 0)) texX = currentTex.GetWidth() - texX - 1;
            texX = std::clamp(texX, 0, currentTex.GetWidth() - 1);

            int rawDrawStart = (SCREEN_HEIGHT - lineHeight) / 2;
            int rawDrawEnd = (SCREEN_HEIGHT + lineHeight) / 2;

            int drawStart = std::max(0, rawDrawStart);
            int drawEnd = std::min(SCREEN_HEIGHT - 1, rawDrawEnd);

            float texStep = static_cast<float>(currentTex.GetHeight()) / static_cast<float>(lineHeight);
            float texPos = static_cast<float>(drawStart - rawDrawStart) * texStep;

            for (int y = drawStart; y < drawEnd; ++y)
            {
                int texY = static_cast<int>(texPos);
                texPos += texStep;
                texY = std::clamp(texY, 0, currentTex.GetHeight() - 1);

                Color color = currentTex.GetPixel(texX, texY);

                m_context.gfx.GetScreenBuffer().SetPixel(x, y, color);
            }
        }
    }
}

void GameplayState::Draw3DFloorAndCeiling()
{
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;

    const Image& floorTex = m_floorTexture;
    const Image& ceilingTex = m_floorTexture;

    int texWidth = floorTex.GetWidth();
    int texHeight = floorTex.GetHeight();

    for (int y = SCREEN_HEIGHT / 2 + 1; y < SCREEN_HEIGHT; ++y)
    {
        int p = y - SCREEN_HEIGHT / 2;
        float posZ = 0.5f * static_cast<float>(SCREEN_HEIGHT);
        float rowDistance = posZ / static_cast<float>(p);

        float floorX0 = m_playerX + rowDistance * (m_dirX - m_planeX);
        float floorY0 = m_playerY + rowDistance * (m_dirY - m_planeY);
        float floorX1 = m_playerX + rowDistance * (m_dirX + m_planeX);
        float floorY1 = m_playerY + rowDistance * (m_dirY + m_planeY);

        float floorStepX = (floorX1 - floorX0) / static_cast<float>(SCREEN_WIDTH);
        float floorStepY = (floorY1 - floorY0) / static_cast<float>(SCREEN_WIDTH);

        float floorX = floorX0;
        float floorY = floorY0;

        for (int x = 0; x < SCREEN_WIDTH; ++x)
        {
            int cellX = static_cast<int>(floorX);
            int cellY = static_cast<int>(floorY);

            float u = floorX - std::floor(floorX);
            float v = floorY - std::floor(floorY);

            int tx = static_cast<int>(u * texWidth);
            int ty = static_cast<int>(v * texHeight);

            tx = std::clamp(tx, 0, texWidth - 1);
            ty = std::clamp(ty, 0, texHeight - 1);

            floorX += floorStepX;
            floorY += floorStepY;

            Color floorColor = floorTex.GetPixel(tx, ty);
            Color ceilingColor = ceilingTex.GetPixel(tx, ty);

            m_context.gfx.GetScreenBuffer().SetPixel(x, y, floorColor);
            m_context.gfx.GetScreenBuffer().SetPixel(x, SCREEN_HEIGHT - y - 1, ceilingColor);
        }
    }
}

void GameplayState::DrawSprites()
{
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;

    std::vector<int> spriteOrder(m_sprites.size());
    std::vector<float> spriteDistance(m_sprites.size());

    for (size_t i = 0; i < m_sprites.size(); ++i)
    {
        spriteOrder[i] = static_cast<int>(i);
        spriteDistance[i] = (
            (m_playerX - m_sprites[i].x) * (m_playerX - m_sprites[i].x) +
            (m_playerY - m_sprites[i].y) * (m_playerY - m_sprites[i].y)
            );
    }

    std::sort(
        spriteOrder.begin(),
        spriteOrder.end(),
        [&spriteDistance](int a, int b)
        {
            return spriteDistance[a] > spriteDistance[b];
        }
    );

    for (size_t i = 0; i < m_sprites.size(); ++i)
    {
        const Sprite& sprite = m_sprites[spriteOrder[i]];

        float spriteX = sprite.x - m_playerX;
        float spriteY = sprite.y - m_playerY;

        float invDet = 1.0f / (m_planeX * m_dirY - m_dirX * m_planeY);
        float transformX = invDet * (m_dirY * spriteX - m_dirX * spriteY);
        float transformY = invDet * (-m_planeY * spriteX + m_planeX * spriteY);

        if (transformY <= 0.1f) continue;

        int spriteScreenX = static_cast<int>((SCREEN_WIDTH / 2) * (1.0f + transformX / transformY));
        int spriteHeight = std::abs(static_cast<int>(SCREEN_HEIGHT / transformY));
        int spriteWidth = spriteHeight;

        int drawStartY = -spriteHeight / 2 + SCREEN_HEIGHT / 2;
        int drawEndY = spriteHeight / 2 + SCREEN_HEIGHT / 2;
        int drawStartX = -spriteWidth / 2 + spriteScreenX;
        int drawEndX = spriteWidth / 2 + spriteScreenX;

        int clampDrawStartY = std::max(0, drawStartY);
        int clampDrawEndY = std::min(SCREEN_HEIGHT - 1, drawEndY);
        int clampDrawStartX = std::max(0, drawStartX);
        int clampDrawEndX = std::min(SCREEN_WIDTH - 1, drawEndX);

        int texIndex = std::clamp(sprite.textureID, 0, static_cast<int>(m_spriteTextures.size()) - 1);
        const Image& tex = m_spriteTextures[texIndex];


        for (int stripe = clampDrawStartX; stripe < clampDrawEndX; ++stripe)
        {
            int rawTexX = static_cast<int>((stripe - drawStartX) * tex.GetWidth() / spriteWidth);
            int texX = std::clamp(rawTexX, 0, tex.GetWidth() - 1);

            if (stripe >= 0 && stripe < static_cast<int>(m_zBuffer.size()) && transformY < m_zBuffer[stripe])
            {
                for (int y = clampDrawStartY; y < clampDrawEndY; ++y)
                {
                    int rawTexY = (y - drawStartY) * tex.GetHeight() / spriteHeight;
                    int texY = std::clamp(rawTexY, 0, tex.GetHeight() - 1);

                    Color color = tex.GetPixel(texX, texY);

                    //if (color == Color(255, 0, 255)) continue;

                    m_context.gfx.GetScreenBuffer().SetPixel(stripe, y, color);
                }
            }
        }
    }
}

bool GameplayState::IsWall(float x, float y) const
{
    int mapX = static_cast<int>(x);
    int mapY = static_cast<int>(y);

    if (mapX < 0 || mapX >= m_mazeGenerator.GetWidth() ||
        mapY < 0 || mapY >= m_mazeGenerator.GetHeight())
    {
        return true;
    }
    return m_mazeGenerator.GetMap()[mapY][mapX] != 0;
}

bool GameplayState::CanMove(float x, float y) const
{
    return
        // Top Row
        !IsWall(x - PLAYER_RADIUS, y - PLAYER_RADIUS) &&
        !IsWall(x, y - PLAYER_RADIUS) &&
        !IsWall(x + PLAYER_RADIUS, y - PLAYER_RADIUS) &&

        // Middle Row
        !IsWall(x - PLAYER_RADIUS, y) &&
        !IsWall(x + PLAYER_RADIUS, y) &&

        // Bottom Row
        !IsWall(x - PLAYER_RADIUS, y + PLAYER_RADIUS) &&
        !IsWall(x, y + PLAYER_RADIUS) &&
        !IsWall(x + PLAYER_RADIUS, y + PLAYER_RADIUS);
}