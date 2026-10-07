#include "MazeGenerator.h"
#include <cstdlib>
#include <ctime>

MazeGenereator::MazeGenereator() 
	: m_width(0), m_height(0), m_rng(std::random_device{}())
{}

void MazeGenereator::Generate(int width, int height)
{
    m_width = (width % 2 == 0) ? width + 1 : width;
    m_height = (height % 2 == 0) ? height + 1 : height;

    m_map.clear();
    m_map.resize(m_height, std::vector<TileType>(m_width, TileType::Wall));

    int startX = 1;
    int startY = 1;
    m_map[startY][startX] = TileType::Road;

    const int dx[4] = { 0, 0, -2, 2 };
    const int dy[4] = { -2, 2, 0, 0 };

    std::stack<Point> pathStack;
    pathStack.push({ startX, startY });

    while (!pathStack.empty()) 
    {
        Point current = pathStack.top();

        std::vector<int> validDirections;
        for (int i = 0; i < 4; ++i) 
        {
            int nx = current.x + dx[i];
            int ny = current.y + dy[i];

            if (nx > 0 && nx < m_width - 1 && ny > 0 && ny < m_height - 1) 
            {
                if (m_map[ny][nx] == TileType::Wall)
                {
                    validDirections.push_back(i);
                }
            }
        }

        if (!validDirections.empty()) 
        {
            std::uniform_int_distribution<size_t> dist(0, validDirections.size() - 1);
            size_t randomIndex = dist(m_rng);
            int dirIndex = validDirections[randomIndex];

            int nextX = current.x + dx[dirIndex];
            int nextY = current.y + dy[dirIndex];

            int wallX = current.x + dx[dirIndex] / 2;
            int wallY = current.y + dy[dirIndex] / 2;

            m_map[wallY][wallX] = TileType::Road;
            m_map[nextY][nextX] = TileType::Road;

            pathStack.push({ nextX, nextY });
        }
        else 
        {
            pathStack.pop();
        }
    }

    m_map[height - 2][width - 2] = TileType::Goal;
}

const std::vector<std::vector<TileType>>& MazeGenereator::GetMap() const
{
	return m_map;
}

int MazeGenereator::GetWidth() const
{
	return m_width;
}

int MazeGenereator::GetHeight() const
{
	return m_height;
}
