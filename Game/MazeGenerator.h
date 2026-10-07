#pragma once

#include <vector>
#include <stack>
#include <random>

enum class TileType
{
	Road = 0,
	Wall = 1
};

class MazeGenereator
{
public:
	MazeGenereator();
	void Generate(int width, int height);

	const std::vector<std::vector<TileType>>& GetMap() const;
	int GetWidth() const;
	int GetHeight() const;

private:
	int m_width;
	int m_height;
	std::vector<std::vector<TileType>> m_map;

	std::mt19937 m_rng;

	struct Point
	{
		int x;
		int y;
	};
};