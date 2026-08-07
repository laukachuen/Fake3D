#define NOMINMAX
#include "App.h"
#include "Color.h"
#include <algorithm>
#include <string>

App::App()
    : m_wnd(800, 600, L"My Custom Game Framework", L"../../GameFramework/Framework/Shader/BufferShaders.hlsl")
{
    m_screenBuffer = std::make_unique<ScreenBuffer>(800, 600);

    std::vector<std::string> texturePaths =
    {
        "Assets/Test1.bmp",
        "Assets/Test2.bmp",
        "Assets/Test3.bmp",
        "Assets/Test4.bmp",
        "Assets/Test5.bmp"
    };

    m_wallTextures.reserve(texturePaths.size());

    for (const auto& path : texturePaths)
    {
        m_wallTextures.emplace_back();
        m_wallTextures.back().LoadBMP(path);
    }
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
    if (m_wnd.kbd.IsKeyPressed('A'))
    {
        float oldDirX = m_dirX;
        m_dirX = m_dirX * std::cos(-ROT_SPEED * dt) - m_dirY * std::sin(-ROT_SPEED * dt);
        m_dirY = oldDirX * std::sin(-ROT_SPEED * dt) + m_dirY * std::cos(-ROT_SPEED * dt);

        float oldPlaneX = m_planeX;
        m_planeX = m_planeX * std::cos(-ROT_SPEED * dt) - m_planeY * std::sin(-ROT_SPEED * dt);
        m_planeY = oldPlaneX * std::sin(-ROT_SPEED * dt) + m_planeY * std::cos(-ROT_SPEED * dt);
    }
    if (m_wnd.kbd.IsKeyPressed('D'))
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

    if (m_wnd.kbd.IsKeyPressed('W'))
    {
        moveX += m_dirX * MOVE_SPEED * dt;
        moveY += m_dirY * MOVE_SPEED * dt;
    }
    if (m_wnd.kbd.IsKeyPressed('S'))
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
}

void App::Render()
{
    m_screenBuffer->Clear(Color::Black());

    Draw2DMap();
    Draw2DRays();
    Draw2DPlayer();

    //Draw3DWalls();
    Draw3DTexWalls();

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
                        m_screenBuffer->SetPixel(px, py, Color::White());
                    }
                }
            }
        }
    }
}

void App::Draw2DPlayer()
{
    int pScreenX = static_cast<int>(m_playerX * TILE_SIZE);
    int pScreenY = static_cast<int>(m_playerY * TILE_SIZE);

    for (int sy = pScreenY - 4; sy < pScreenY + 4; ++sy)
    {
        for (int sx = pScreenX - 4; sx < pScreenX + 4; ++sx)
        {
            m_screenBuffer->SetPixel(sx, sy, Color::Yellow());
        }
    }

    int lineEndX = pScreenX + static_cast<int>(m_dirX * 20.0f);
    int lineEndY = pScreenY + static_cast<int>(m_dirY * 20.0f);

    for (float t = 0.0f; t <= 5.0f; t += 0.1f)
    {
        int lx = pScreenX + static_cast<int>((lineEndX - pScreenX) * t);
        int ly = pScreenY + static_cast<int>((lineEndY - pScreenY) * t);
        m_screenBuffer->SetPixel(lx, ly, Color::Red());
    }
}

void App::Draw2DRays()
{
    const int NUM_RAYS = 60; 

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

            if (mapX < 0 || mapX >= MAP_WIDTH || mapY < 0 || mapY >= MAP_HEIGHT)
            {
                break;
            }

            if (m_map[mapY][mapX] > 0)
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

                m_screenBuffer->SetPixel(lx, ly, Color::Cyan());
            }
        }
    }
}

void App::Draw3DWalls()
{
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;

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

            if (mapX < 0 || mapX >= MAP_WIDTH || mapY < 0 || mapY >= MAP_HEIGHT) break;
            if (m_map[mapY][mapX] > 0) hit = true;
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
                    m_screenBuffer->SetPixel(x, y, Color::Blue());
                }
                else if (y >= drawStart && y <= drawEnd)
                {
                    m_screenBuffer->SetPixel(x, y, wallColor);
                }
                else
                {
                    m_screenBuffer->SetPixel(x, y, Color::Green());
                }
            }
        }
    }
}

void App::Draw3DTexWalls()
{
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;

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

            if (mapX < 0 || mapX >= MAP_WIDTH || mapY < 0 || mapY >= MAP_HEIGHT) break;
            if (m_map[mapY][mapX] > 0)
            {
                hit = true;
                wallType = m_map[mapY][mapX];
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

            for (int y = 0; y < SCREEN_HEIGHT; ++y)
            {
                if (y < drawStart)
                {
                    m_screenBuffer->SetPixel(x, y, Color::Blue());
                }
                else if (y <= drawEnd)
                {
                    int texY = static_cast<int>(texPos);
                    texPos += texStep;
                    texY = std::clamp(texY, 0, currentTex.GetHeight() - 1);

                    Color color = currentTex.GetPixel(texX, texY);

                    m_screenBuffer->SetPixel(x, y, color);
                }
                else
                {
                    m_screenBuffer->SetPixel(x, y, Color::Green());
                }
            }
        }
    }
}

bool App::IsWall(float x, float y) const
{
    int mapX = static_cast<int>(x);
    int mapY = static_cast<int>(y);

    if (mapX < 0 || mapX >= MAP_WIDTH ||
        mapY < 0 || mapY >= MAP_HEIGHT)
    {
        return true;
    }
    return m_map[mapY][mapX] != 0;
}

bool App::CanMove(float x, float y) const
{
    return
        // Top Row
        !IsWall(x - PLAYER_RADIUS, y - PLAYER_RADIUS) &&
        !IsWall(x                , y - PLAYER_RADIUS) &&
        !IsWall(x + PLAYER_RADIUS, y - PLAYER_RADIUS) &&

        // Middle Row
        !IsWall(x - PLAYER_RADIUS, y) &&
        !IsWall(x + PLAYER_RADIUS, y) &&

        // Bottom Row
        !IsWall(x - PLAYER_RADIUS, y + PLAYER_RADIUS) &&
        !IsWall(x                , y + PLAYER_RADIUS) &&
        !IsWall(x + PLAYER_RADIUS, y + PLAYER_RADIUS);
}
