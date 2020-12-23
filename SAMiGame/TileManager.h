#pragma once
#include "Tile.h"


class TileManager
{
public:
	static void load();
	static void draw(float camera_x, float camera_y, sf::RenderWindow & window);
private:
	static const int c_tile_count = 6;
	static const int c_tile_width = 32;
	static const int c_tile_height = 24;
	static Tile _tiles[c_tile_count];

	static int _tileMap[c_tile_width][c_tile_height];

};

/* 



for (int x = 0; x < 16; ++x) {
	for (int y = 0; y < 12; ++y) {
		int tileIndex = (x + y + 1) % 2;
		_tiles[tileIndex].setPosition(x * 32, y * 32);
		_tiles[tileIndex].draw(_mainWindow);
	}
}

*/