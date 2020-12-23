#include "stdafx.h"
#include "TileManager.h"


void TileManager::load()
{
	_tiles[0].load("images/tiles/tile1.bmp");
	_tiles[1].load("images/tiles/tile2.bmp");
	_tiles[2].load("images/tiles/tile3.bmp");
	_tiles[3].load("images/tiles/tile4.bmp");
	_tiles[4].load("images/tiles/tile5.bmp");
	_tiles[5].load("images/tiles/tile6.bmp");

	
	for (int x = 0; x < c_tile_width; ++x) {
		for (int y = 0; y < c_tile_height; ++y) {
			_tileMap[x][y] = 0;
		}
	}
	int seed = 0;

	for (int i = 0; i < 100; ++i) {
		seed = (3 * seed + 10) % 177;
		int x = seed % c_tile_width;
		seed = (3 * seed + 10) % 177;
		int y = seed % c_tile_height;
		
		_tileMap[x][y] = 1;
	}

	for (int i = 0; i < 3; ++i) {
		seed = (3 * seed + 10) % 177;
		int x = seed % c_tile_width-1;
		seed = (3 * seed + 10) % 177;
		int y = seed % c_tile_height-1;

		_tileMap[x][y] = 2;
		_tileMap[x+1][y] = 3;
		_tileMap[x][y+1] = 4;
		_tileMap[x+1][y+1] = 5;
	}
	
}

void TileManager::draw(float camera_x, float camera_y, sf::RenderWindow & window)
{
	// TODO: make this better
	for (int x = std::max(0, (int)(camera_x/32)); x < std::min(c_tile_width, (int)(camera_x / 32 + 17)); ++x) {
		for (int y = std::max(0, (int)(camera_y / 32)); y < std::min(c_tile_height, (int)(camera_y / 32 + 13)); ++y) {
			_tiles[_tileMap[x][y]].setPosition(x * 32.0 - camera_x, y * 32.0 - camera_y);
			_tiles[_tileMap[x][y]].draw(window);
		}
	}
}




Tile TileManager::_tiles[c_tile_count];
int TileManager::_tileMap[c_tile_width][c_tile_height];