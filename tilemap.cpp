#include "tilemap.h"

namespace Tmpl8
{
	Tilemap::Tilemap() { };
	

	void Tilemap::Draw(Surface* visible_layer, const int tile_size, const vec2& camera_offset)
	{
		// For each tile on the tilemap.
		for (int tile_x{ 0 }; tile_x < m_tilemap_width; ++tile_x)
		{
			for (int tile_y{ 0 }; tile_y < m_tilemap_height; ++tile_y)
			{
				// Draw the tile.
				int sprite_frame{ m_tilemap[tile_y][tile_x] };
				
				m_tile_sprite.SetFrame(sprite_frame);
				
				m_tile_sprite.DrawScaled(
					(tile_x * tile_size) - camera_offset.x,
					(tile_y * tile_size) - camera_offset.y,
					tile_size, tile_size, visible_layer
				);
			}
		}
	}


	const bool Tilemap::IsPositionValid(const int x_position, const int y_position) const
	{
		return IsTileWalkable(m_tilemap[y_position][x_position]);		
	}


	const bool Tilemap::IsTileWalkable(const int tile_id) const
	{
		switch (tile_id)
		{
		case 23:
			return true;
		case 47:
		case 48:
			return false;
		default:
			return false;
		}
	}
}